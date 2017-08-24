#include <iostream>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <thread>
#include <atomic>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <PictureIt/pictureit.hpp>

#include <GLFW/glfw3.h>

#include "pulse.hpp"


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

const char *image_dir_path = nullptr;

PictureIt *pi = nullptr;

int current_windowed_width = WINDOW_WIDTH;
int current_windowed_height = WINDOW_HEIGHT;
int current_windowed_pos_x = 0;
int current_windowed_pos_y = 0;

bool fullscreen = false;
bool quit = false;

const std::chrono::time_point<std::chrono::high_resolution_clock> t_start =
    std::chrono::high_resolution_clock::now();


class Input_thread {
private:
    std::atomic<bool> running;
    std::thread th_input;

public:
    template <typename Buf> Input_thread(Input &i, Buf &buffer):
        running(true), th_input([&] {
            while(running) {
                i.read(buffer);
            };
        }){};

    ~Input_thread(){
        running = false;
        th_input.join();
    };
};



static void signal_handler(int sig, siginfo_t *si, void *unused) {
    quit = true;
}

static void keyboard(GLFWwindow* window, int key, int scancode, int action,
                    int mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT)
        return;

    switch (key) {
        case GLFW_KEY_F11:
            fullscreen = !fullscreen;

            if (fullscreen) {
                // Remember position and size
                const GLFWvidmode *mode = glfwGetVideoMode(
                    glfwGetPrimaryMonitor());
                glfwGetWindowPos(window, &current_windowed_pos_x,
                                 &current_windowed_pos_y);
                glfwGetWindowSize(window, &current_windowed_width,
                                  &current_windowed_height);

                glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0,
                                     mode->width, mode->height, 0);
            } else {
                glfwSetWindowMonitor(window, NULL, current_windowed_pos_x,
                                     current_windowed_pos_y,
                                     current_windowed_width,
                                     current_windowed_height, 0);
            }
            break;

        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;

        case GLFW_KEY_N:
            pi->next_image();
            break;

        case GLFW_KEY_S:
            break;
    }
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    pi->set_window_size(width, height);
}


bool add_signal_handler() {
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = signal_handler;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        printf("Can't set SIGINT handler!\n");
        return false;
    }

    if (sigaction(SIGHUP, &sa, NULL) == -1) {
        printf("Can't set SIGHUP handler!\n");
        return false;
    }

    return true;
}


int main(int argc, char **argv) {
    // Signal handling
    if(!add_signal_handler()) {
        exit(EXIT_FAILURE);
    }

    // Print CLI usage if not enough sys args
    if (argc < 2) {
        printf("usage: %s [img-directory]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Get a image directory
    for (int i = 1; i < argc; i++) {
        image_dir_path = argv[i];
    }


    // Initialize glfw
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // Create a windowed mode window and its OpenGL context
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                          "glfwPI", NULL, NULL);

    if (!window) {
        fprintf(stderr, "Couldn't create window, exiting!\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, keyboard);

    fprintf(stdout, "GL: %s, GLSL: %s\n", glGetString(GL_VERSION),
            glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Create PictureIt and set some initial properties
    PI::Config::Transition pi_trans;
    pi_trans.speed = 50;

    PI::Config::PictureIt pi_cfg = PI::Config::PictureIt();
    pi_cfg.max_fps = 120;
    pi_cfg.update_interval = 15;
    pi_cfg.pick_random = true;
    pi_cfg.recursive = true;
    pi_cfg.display_mode = PI::Config::Texture::Mode::STRETCH;
    pi_cfg.transition = pi_trans;
    pi_cfg.image_dir = image_dir_path;

    pi = new PictureIt(pi_cfg);
    pi->set_window_size(WINDOW_WIDTH, WINDOW_HEIGHT);

    PI::Config::FFT fft_cfg = PI::Config::FFT();
    fft_cfg.window = PI::Config::FFT::Window::EXACT_BLACKMAN;

    // Spectrum one on the left
    PI::Config::Spectrum spectrum_cfg1 = PI::Config::Spectrum(fft_cfg);
    spectrum_cfg1.gradient = -2;
    spectrum_cfg1.bot_color[0] = 1;
    spectrum_cfg1.bot_color[1] = 1;
    spectrum_cfg1.bot_color[2] = 1;
    spectrum_cfg1.pos.Ymax = 3.5;
    spectrum_cfg1.pos.Xmin = -1.0;
    spectrum_cfg1.pos.Xmax = 3.0005;
    spectrum_cfg1.set_gravity(2.5);
    spectrum_cfg1.set_db_range(-45, 45);
    spectrum_cfg1.set_freq_range(0, 1000);
    pi->add_spectrum(spectrum_cfg1);

    // Mirrored spectrum one on the right
    spectrum_cfg1.pos.Ymax = 3.5;
    spectrum_cfg1.pos.Xmin = 3.0005;
    spectrum_cfg1.pos.Xmax = -1.0;
    pi->add_spectrum(spectrum_cfg1);


    FFT fft(fft_cfg);
    Buffer<int16_t> buffer(fft_cfg.get_buffer_size());

    // start input thread
    Input::Ptr input = make_unique<Pulse>("", fft_cfg.get_sample(), 220,
                                          false);
    Input_thread inth(*input, buffer);


    // Mainloop which renders PictureIt
    while (! glfwWindowShouldClose(window)) {
        if (quit) break;

        fft.calculate(buffer);

        // Render PictureIt frame
        pi->draw(fft);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    delete pi;

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
