#include "audio_player.hpp"

#include <math.h>

AudioPlayer::AudioPlayer(std::function<void(float*, int)> audio_data_callback) {
    this->audio_callback = audio_data_callback;

    this->frame_size = PLAY_FRAMES_PER_BUFFER / sizeof(float);
    this->size_one_sec = (PLAY_FRAMES_PER_BUFFER * 2) * sizeof(float);
    this->audio_data_size = this->size_one_sec / 2;
}


AudioPlayer::~AudioPlayer() {
    // sf_close deallocates "this->buffer"
    // so we don'T have to
    sf_close(this->file);

    if (this->pulse != nullptr) {
        pa_simple_free(this->pulse);
    };
}


bool AudioPlayer::init_pulse() {
    static const pa_sample_spec sample_spec = {
         .format = PA_SAMPLE_FLOAT32,
         .rate = 44100,
         .channels = 2
    };

    int err = 0;
    if (!(this->pulse = pa_simple_new(NULL, "glfwPI", PA_STREAM_PLAYBACK, NULL, "Playback", &sample_spec, NULL, NULL, &err))) {
       fprintf(stderr, "main: pa_simple_new() failed: %s\n", pa_strerror(err));
       return false;
    }

    return true;
}


bool AudioPlayer::load(const char *file_path) {
    if(this->buffer != nullptr) {
        delete [] this->buffer;
    }

    this->buffer = new float[this->size_one_sec];

    if (! (this->file = sf_open(file_path, SFM_READ, &this->info))) {
        fprintf(stderr, "Unable to open input file %s.\n", file_path) ;

        return false;
    }

    return this->init_pulse();
}


void AudioPlayer::start_playback() {
    long read_count = 0;

    int err = 0;
    while (!this->should_exit) {
        read_count = sf_read_float(this->file, this->buffer, this->size_one_sec / 4);

        // End of file
        if(read_count <= 0) {
            break;
        }

        std::thread cbt(this->audio_callback, this->buffer, this->audio_data_size);
        cbt.detach();

        if (pa_simple_write(this->pulse, this->buffer, read_count * 4, &err) < 0) {
            fprintf(stderr, "main: pa_simple_write() failed: %s (%ld)\n", pa_strerror(err), read_count / 2);
        }
    }

    if (pa_simple_drain(this->pulse, &err) < 0) {
        fprintf(stderr, "main: pa_simple_drain() failed: %s\n", pa_strerror(err));
    }
}


void AudioPlayer::play() {
    this->player = std::thread(&AudioPlayer::start_playback, this);
}

void AudioPlayer::exit() {
    this->should_exit = true;
    this->player.join();
}
