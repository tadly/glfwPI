#include <iostream>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <thread>

#include <GLFW/glfw3.h>
#include <PictureIt/pictureit.hpp>

#include "audio_player.hpp"


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define IMG_UPDATE_INTERVAL 30


int SPECTRUM_BAR_COUNT = 64;
int TRANSITION_TIME_MS = 500;
int slide_direction = 1;

const char *image_dir_path = nullptr;
const char *audio_file_path = nullptr;

GLFWwindow *window = nullptr;
PictureIt *pi = nullptr;
AudioPlayer *player = nullptr;

int current_windowed_width = WINDOW_WIDTH;
int current_windowed_height = WINDOW_HEIGHT;
int current_windowed_pos_x = 0;
int current_windowed_pos_y = 0;

bool fullscreen = false;
bool quit = false;

static void signal_handler(int sig, siginfo_t *si, void *unused) {
    quit = true;
}

static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT)
        return;

    switch (key) {
        case GLFW_KEY_F11:
            fullscreen = !fullscreen;

            if (fullscreen) {
                // Remember position and size
                const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
                glfwGetWindowPos(window, &current_windowed_pos_x, &current_windowed_pos_y);
                glfwGetWindowSize(window, &current_windowed_width, &current_windowed_height);

                glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, 0);
            } else {
                glfwSetWindowMonitor(window, NULL, current_windowed_pos_x, current_windowed_pos_y, current_windowed_width, current_windowed_height, 0);
            }
            break;

        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;

        case GLFW_KEY_N:
            pi->update_image();
            break;

        case GLFW_KEY_S:
            pi->spectrum_enabled = !pi->spectrum_enabled;
            break;

        case GLFW_KEY_V:
            if (mods == GLFW_MOD_CONTROL) {
                pi->spectrum_flip_vertical = !pi->spectrum_flip_vertical;
            } else {
                pi->spectrum_mirror_vertical = !pi->spectrum_mirror_vertical;
            }
            break;

        case GLFW_KEY_H:
            if (mods == GLFW_MOD_CONTROL) {
                pi->spectrum_flip_horizontal = !pi->spectrum_flip_horizontal;
            } else {
                pi->spectrum_mirror_horizontal = !pi->spectrum_mirror_horizontal;
            }
            break;

        case GLFW_KEY_R:
            pi->img_pick_random = !pi->img_pick_random;
            break;

        case GLFW_KEY_B:
            pi->spectrum_position_vertical = 1.0f;
            break;

        case GLFW_KEY_C:
            pi->spectrum_position_vertical = 0.0f;
            break;

        case GLFW_KEY_T:
            pi->spectrum_position_vertical = -1.0f;
            break;

        case GLFW_KEY_UP:
            pi->spectrum_position_vertical = pi->spectrum_position_vertical - 0.01f;
            break;

        case GLFW_KEY_DOWN:
            pi->spectrum_position_vertical = pi->spectrum_position_vertical + 0.01f;
            break;

        case GLFW_KEY_LEFT:
            pi->spectrum_position_horizontal = pi->spectrum_position_horizontal - 0.01f;
            break;

        case GLFW_KEY_RIGHT:
            pi->spectrum_position_horizontal = pi->spectrum_position_horizontal + 0.01f;
            break;

        case GLFW_KEY_RIGHT_BRACKET:
            pi->spectrum_width = pi->spectrum_width + 0.015f;
            break;

        case GLFW_KEY_SLASH:
            pi->spectrum_width = pi->spectrum_width - 0.015f;
            break;

        case GLFW_KEY_1:
            pi->set_img_transition_efx( EFX::CROSSFADE );
            pi->efx->configure("fade_time_ms",  &TRANSITION_TIME_MS);
            break;

        case GLFW_KEY_2:
            pi->set_img_transition_efx( EFX::SLIDE );
            pi->efx->configure("slide_time_ms", &TRANSITION_TIME_MS);
            pi->efx->configure("slide_direction", &slide_direction);
            break;

        case GLFW_KEY_3:
            //pi->set_img_transition_efx( EFX::FLIP );
            break;

        case GLFW_KEY_7:
            pi->efx->image_mode = MODE::STRETCH;
            break;

        case GLFW_KEY_8:
            pi->efx->image_mode = MODE::CENTER;
            break;

        case GLFW_KEY_9:
            pi->efx->image_mode = MODE::SCALE;
            break;

        case GLFW_KEY_0:
            pi->efx->image_mode = MODE::ZOOM;
            break;
    }
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    pi->window_width = width;
    pi->window_height = height;
}


void audio_data(float *data, int data_length) {
    pi->audio_data(data, data_length);
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

void show_help() {
    std::cout << "Available keyboard shortcuts:" << std::endl;
    std::cout << "    esc: Exit PictureIt" << std::endl;
    std::cout << "    f11: Toggle fullscreen" << std::endl;
    std::cout << "      n: Display next image" << std::endl;
    std::cout << "      r: Toggle pick random image" << std::endl;
    std::cout << "      1: Activate Crossfade transition effect" << std::endl;
    std::cout << "      2: Activate Slide transition effect" << std::endl;
    std::cout << "      s: Enable/Disable Spectrum" << std::endl;
    std::cout << "      v: Toggle Spectrum vertical mirror" << std::endl;
    std::cout << "      h: Toggle Spectrum horizontal mirror" << std::endl;
    std::cout << " ctrl+v: Toggle Spectrum flip vertical" << std::endl;
    std::cout << " ctrl+h: Toggle Spectrum flip horizontal" << std::endl;
    std::cout << "     up: Move Spectrum up" << std::endl;
    std::cout << "   down: Move Spectrum down" << std::endl;
    std::cout << "   left: Move Spectrum left" << std::endl;
    std::cout << "  right: Move Spectrum right" << std::endl;
    std::cout << "      +: Increase Spectrum size" << std::endl;
    std::cout << "      -: Decrease Spectrum size" << std::endl;
    std::cout << "      b: Position Spectrum at the bottom" << std::endl;
    std::cout << "      c: Position Spectrum at the center" << std::endl;
    std::cout << "      t: Position Spectrum at the top" << std::endl;
    std::cout << "      7: Image mode \"Stretch\"" << std::endl;
    std::cout << "      8: Image mode \"Center\"" << std::endl;
    std::cout << "      9: Image mode \"Scale\"" << std::endl;
    std::cout << "      0: Image mode \"Zoom\"" << std::endl;
}


int main(int argc, char **argv) {
    // Signal handling
    if(!add_signal_handler()) {
        exit(EXIT_FAILURE);
    }

    // Print CLI usage if not enough sys args
    if (argc < 2) {
        printf("usage: %s img-directory [-b spectrum-bar-count] [-a audio-file]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Some simple argparsing
    // Not super failsafe but'll do :)
    for (int i = 1; i < argc; i++) {
        if(strncmp(argv[i], "-b", 2) == 0) {
            SPECTRUM_BAR_COUNT = atoi(argv[i+1]);
            i++;

        } else if(strncmp(argv[i], "-a", 2) == 0) {
            audio_file_path = argv[i+1];
            i++;

        } else {
            image_dir_path = argv[i];
        }
    }

    // Print keyboard shortcuts
    show_help();


    // Initialize glfw
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "glfwPI", NULL, NULL);

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(2);

    glfwSetKeyCallback(window, keyboard);


    // Create PictureIt and set some intial properties
    pi = new PictureIt(SPECTRUM_BAR_COUNT);

    pi->set_img_transition_efx(EFX::CROSSFADE);
    pi->efx->configure("fade_time_ms",  &TRANSITION_TIME_MS);

    pi->efx->image_mode = MODE::SCALE;

    pi->window_width = WINDOW_WIDTH;
    pi->window_height = WINDOW_HEIGHT;
    pi->img_pick_random = true;
    pi->img_update_interval = IMG_UPDATE_INTERVAL;
    pi->spectrum_animation_speed = 0.008f;
    pi->spectrum_position_vertical = 0.0f;
    pi->spectrum_enabled = true;

    // Get some sexy pink going :)
    // That's "E91E63" in hex for anyone wondering ;)
    for (int i = 0; i < SPECTRUM_BAR_COUNT; i++) {
        pi->set_bar_color(i, 0.914, 0.118, 0.388);
    }

    // Load images from the directory passed as argument
    pi->load_images(image_dir_path);


    // Make the window's context current
    glfwMakeContextCurrent(window);


    // Create an AudioPlayer so we have something
    // to feed the spectrum with
    player = new AudioPlayer(audio_data);
    if (audio_file_path != nullptr && player->load(audio_file_path)) {
        player->play();
    }


    // Mainloop which renders PictureIt
    while (!glfwWindowShouldClose(window)) {
        if (quit) {
            break;
        }

        // Render PictureIt frame
        pi->render();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Cleanup
    player->exit();

    delete player;
    delete pi;

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
