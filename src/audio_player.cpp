#include "audio_player.hpp"

#include <math.h>

AudioPlayer::AudioPlayer(PictureIt *PI, std::string Filename) {

    this->pi = PI;
}


AudioPlayer::~AudioPlayer() {

    pi = nullptr;
}


int AudioPlayer::AudioCallback(const void *inputBuffer, void *outputBuffer,
                               unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags statusFlags,
                               void *userData)
{
  return 0;
}

