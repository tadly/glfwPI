#pragma once

#include <PictureIt/pictureit.hpp>

#include <functional>
#include <signal.h>
#include <thread>

#include <sndfile.h>
#include <asplib/AudioInterfaces/PortAudio/asplib_IPortAudio.h>
#include <asplib/Core/Buffers/asplib_BufferTypes.hpp>


class AudioPlayer : asplib::IPortAudio {
    private:
        PictureIt *pi;
        asplib::FrameBuffer_NativeFloat *audioBuffer;

    public:
        AudioPlayer(asplib::CPaDeviceInfoVector_t &Devices, PictureIt *pi);
        ~AudioPlayer();

        bool Create(int AudioDeviceIndex, std::string AudioFilePath);
        bool Start();

        virtual int AudioCallback(const void *inputBuffer, void *outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo* timeInfo,
                                  PaStreamCallbackFlags statusFlags,
                                  void *userData);
};
