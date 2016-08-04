#include "audio_player.hpp"
#include <sndfile.hh>

#include <math.h>

using namespace asplib;

// helper functions
void show_audio_file_info(SndfileHandle &WavFile, string FileName);


AudioPlayer::AudioPlayer(asplib::CPaDeviceInfoVector_t &Devices) {
    audioBuffer = nullptr;
    this->get_AvailableDevices(Devices);
}


AudioPlayer::~AudioPlayer() {

    delete pi;
    pi = nullptr;
    
    delete audioBuffer;
    audioBuffer = nullptr;
}

bool AudioPlayer::Create(int AudioDeviceIndex, std::string AudioFilePath) {
    if (AudioFilePath.empty()) {
        cout << "No valid audio file path! Please select an audio file with \"-a\" <file path>." << endl;
        return false;
    }
  
    SndfileHandle wavFile(AudioFilePath.c_str(), SFM_READ);
    if (wavFile.error() != SF_ERR_NO_ERROR) {
        cout << "Failed to open audio file \"" << AudioFilePath << " with sndfile error \"" << wavFile.strError() << "\"" << endl;
        return false;
    }

    show_audio_file_info(wavFile, AudioFilePath);

    audioBuffer = new asplib::FrameBuffer_NativeFloat(wavFile.frames()*wavFile.channels(), 1);
    sf_count_t maxSamples = wavFile.read(audioBuffer->get_Frame(0), wavFile.frames());
    if (maxSamples != wavFile.frames())
    {
        cout << "Couldn't read entire audio file" << endl;
        return false;
    }

    return true;
}


int AudioPlayer::AudioCallback(const void *inputBuffer, void *outputBuffer,
                               unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags statusFlags,
                               void *userData) {
  
    return 0;
}


void show_audio_file_info(SndfileHandle &WavFile, string FileName)
{
  cout << "audio file information: " << endl;
  cout << "file name=" << FileName << endl;
  cout << "format=" << WavFile.format() << endl;
  cout << "channels=" << WavFile.channels() << endl;
  cout << "frames=" << WavFile.frames() << endl;
  cout << "sample rate=" << WavFile.samplerate() << endl;
  cout << endl << endl;
}
