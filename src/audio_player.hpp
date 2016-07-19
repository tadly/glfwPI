#pragma once

#include <functional>
#include <signal.h>
#include <thread>

#include <sndfile.h>
#include <pulse/pulseaudio.h>
#include <pulse/simple.h>


#define PLAY_FRAMES_PER_BUFFER 44100

class AudioPlayer {
    private:
        bool should_exit = false;

        std::thread player;

        SNDFILE *file;
        SF_INFO info;

        pa_simple *pulse = NULL;

        int frame_size;
        long size_one_sec;
        int audio_data_size;
        int driver_id;

        float *buffer = nullptr;

        std::function<void(float*, int)> audio_callback;

        bool init_pulse();
        void start_playback();

    public:
        AudioPlayer(std::function<void(float*, int)> audio_data_callback);
        ~AudioPlayer();

        bool load(const char *file_path);
        void play();
        void exit();
};
