#pragma once

#include <PictureIt/pictureit.hpp>

#include <functional>
#include <signal.h>
#include <thread>

#include <sndfile.h>
#include <asplib/AudioInterfaces/PortAudio/asplib_IPortAudio.h>


class AudioPlayer : asplib::CPortAudioHandle {
    private:
        PictureIt *pi;

    public:
        AudioPlayer(PictureIt *PI, std::string Filename);
        ~AudioPlayer();

        virtual int AudioCallback(const void *inputBuffer, void *outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo* timeInfo,
                                  PaStreamCallbackFlags statusFlags,
                                  void *userData);
};
