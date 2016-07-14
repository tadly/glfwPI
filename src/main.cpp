#include <iostream>
#include <math.h>

#include <GLFW/glfw3.h>
#include <PictureIt/pictureit.h>

int WINDOW_WIDTH  = 1280;
int WINDOW_HEIGHT = 720;

EFX TRANSITION_EFFECT = EFX::SLIDE;
MODE IMAGE_MODE = MODE::CENTER;
int IMG_UPDATE_INTERVAL = 300;
int SPECTRUM_BAR_COUNT = 64;
int TRANSITION_TIME_MS = 1000;
int SLIDE_DIRECTION = 1;

PictureIt *pi = NULL;
float *audio;

static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT)
        return;

    switch (key) {
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
            if ( mods == GLFW_MOD_CONTROL )
                pi->spectrum_flip_vertical = !pi->spectrum_flip_vertical;
            else
                pi->spectrum_mirror_vertical = !pi->spectrum_mirror_vertical;
            break;
        case GLFW_KEY_H:
            if ( mods == GLFW_MOD_CONTROL )
                pi->spectrum_flip_horizontal = !pi->spectrum_flip_horizontal;
            else
                pi->spectrum_mirror_horizontal = !pi->spectrum_mirror_horizontal;
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
            break;
        case GLFW_KEY_2:
            pi->set_img_transition_efx( EFX::SLIDE );
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

void audio_data(int buffer[], int buffer_size) {
    for (int i = 0; i < buffer_size && i < SPECTRUM_BAR_COUNT; i++) {
        audio[i] = (float)buffer[i];
    }

    pi->audio_data(audio, buffer_size);
}

int main(int argc, char **argv) {
    if ( argc < 2 ) {
        std::cout<<"usage: "<< argv[0]<< " directory [spectrum-bar-count]"<< std::endl;
        return 1;
    }

    // Print keyboard shortcuts
    std::cout<<"Available keyboard shortcuts:"<< std::endl;
    std::cout<<"    esc: Exit PictureIt"<< std::endl;
    std::cout<<"      n: Display next image"<< std::endl;
    std::cout<<"      r: Toggle pick random image"<< std::endl;
    std::cout<<"      1: Activate Crossfade transition effect"<< std::endl;
    std::cout<<"      2: Activate Slide transition effect"<< std::endl;
    std::cout<<"      s: Enable/Disable Spectrum"<< std::endl;
    std::cout<<"      v: Toggle Spectrum vertical mirror"<< std::endl;
    std::cout<<"      h: Toggle Spectrum horizontal mirror"<< std::endl;
    std::cout<<" ctrl+v: Toggle Spectrum flip vertical"<< std::endl;
    std::cout<<" ctrl+h: Toggle Spectrum flip horizontal"<< std::endl;
    std::cout<<"     up: Move Spectrum up"<< std::endl;
    std::cout<<"   down: Move Spectrum down"<< std::endl;
    std::cout<<"   left: Move Spectrum left"<< std::endl;
    std::cout<<"  right: Move Spectrum right"<< std::endl;
    std::cout<<"      +: Increase Spectrum size"<< std::endl;
    std::cout<<"      -: Decrease Spectrum size"<< std::endl;
    std::cout<<"      b: Position Spectrum at the bottom"<< std::endl;
    std::cout<<"      c: Position Spectrum at the center"<< std::endl;
    std::cout<<"      t: Position Spectrum at the top"<< std::endl;
    std::cout<<"      7: Image mode \"Stretch\""<< std::endl;
    std::cout<<"      8: Image mode \"Center\""<< std::endl;
    std::cout<<"      9: Image mode \"Scale\""<< std::endl;
    std::cout<<"      0: Image mode \"Zoom\""<< std::endl;

    GLFWwindow *window;
    // Initialize the library
    if (!glfwInit())
        exit(EXIT_FAILURE);

    // Create a windowed mode window and its OpenGL context

    /*
    * Windowed full screen
    */
    //GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    //const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    //window = glfwCreateWindow(mode->width, mode->height, "PictureIt", monitor, NULL);

    /*
    * Full screen
    */
    //window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "PictureIt", glfwGetPrimaryMonitor(), NULL);

    /*
    * Window
    */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "PictureIt", NULL, NULL);

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glfwSetWindowPos(window, 2048, 1152);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(2);

    glfwSetKeyCallback(window, keyboard);

    if (argc > 2)
        SPECTRUM_BAR_COUNT = atoi(argv[2]);

    /*
    * Start PictureIt
    */
    pi = new PictureIt(SPECTRUM_BAR_COUNT);

    pi->set_img_transition_efx(TRANSITION_EFFECT);
    pi->efx->configure("fade_time_ms",  &TRANSITION_TIME_MS);
    pi->efx->configure("slide_time_ms", &TRANSITION_TIME_MS);
    pi->efx->configure("slide_direction", &SLIDE_DIRECTION);
    pi->efx->image_mode = IMAGE_MODE;

    pi->window_width = width;
    pi->window_height = height;
    pi->img_pick_random = false;
    pi->img_update_interval = IMG_UPDATE_INTERVAL;
    pi->spectrum_animation_speed = 0.008f;
    pi->spectrum_position_vertical = 0.0f;
    pi->spectrum_enabled = true;

    for (int i = 0; i < SPECTRUM_BAR_COUNT; i++)
        pi->set_bar_color(i, 0.01f*i, 0.03f*i, 0.5f);

    // Load images from the directory passed as argument
    pi->load_images(argv[1]);
    /*
    * End PictureIt
    */

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // pictureit->audio_data(pAudioData, iAudioDataLength);

        // Render PictureIt frame
        pi->render();

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
