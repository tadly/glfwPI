#include "audio_player.hpp"

#include <math.h>

using namespace asplib;


AudioPlayer::AudioPlayer(asplib::CPaDeviceInfoVector_t &Devices) {

    this->get_AvailableDevices(Devices);
}


AudioPlayer::~AudioPlayer() {

    pi = nullptr;
}


int AudioPlayer::AudioCallback(const void *inputBuffer, void *outputBuffer,
                               unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags statusFlags,
                               void *userData) {
  
    return 0;
}

