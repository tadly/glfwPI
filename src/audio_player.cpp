#include "audio_player.hpp"
#include <sndfile.hh>

#include <math.h>

using namespace asplib;

// helper functions
void show_audio_file_info(SndfileHandle &WavFile, string FileName);


AudioPlayer::AudioPlayer(asplib::CPaDeviceInfoVector_t &Devices, PictureIt *pi) {
    this->pi = nullptr;
    if (!pi) {
        throw exception("FATAL ERROR! Invalid PictureIt object pointer!");
    }
    this->pi = pi;
  
    audioBuffer = nullptr;
    silencAudioBuffer = nullptr;
    this->get_AvailableDevices(Devices);
}


AudioPlayer::~AudioPlayer() {  
    delete audioBuffer;
    audioBuffer = nullptr;

    delete silencAudioBuffer;
    silencAudioBuffer = nullptr;

    this->stop_Device();
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
    sf_count_t maxSamples = 0;
    if (wavFile.channels() > 1)
      maxSamples = wavFile.readf(audioBuffer->get_Frame(0), wavFile.frames());
    else
      maxSamples = wavFile.read(audioBuffer->get_Frame(0), wavFile.frames());

    if (maxSamples != wavFile.frames())
    {
        cout << "Couldn't read entire audio file" << endl;
        return false;
    }
    this->maxOutChannels = wavFile.channels();

    PaError paErr = this->configure_Device(0, this->maxOutChannels, wavFile.samplerate(), -1, AudioDeviceIndex, paFloat32, -1, pi->fft_frame_size);
    if (paErr != paNoError) {
        cout << "FATAL ERROR! Failed to configure portaudio device \"" << this->get_PortAudioErrStr(paErr) << "\"" << endl;
        return false;
    }
    paErr = this->create_Device();

    return true;
}

bool AudioPlayer::Start() {
  PaError err = this->start_Device();
  if (err != paNoError) {
    cout << "Failed to start audio player with portaudio error \"" << this->get_PortAudioErrStr(err) << "\"" << endl;
    return false;
  }

  return true;
}


int AudioPlayer::AudioCallback(const void *inputBuffer, void *outputBuffer,
                               unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags statusFlags,
                               void *userData) {

    static unsigned long long count = 0;

    if (count*framesPerBuffer*maxOutChannels < audioBuffer->get_MaxFrameLength() - framesPerBuffer*maxOutChannels)
    {
        memcpy(outputBuffer, (void*)((float*)audioBuffer->get_Frame(0) + count*framesPerBuffer*maxOutChannels), sizeof(float)*framesPerBuffer*maxOutChannels);
        pi->audio_data(audioBuffer->get_Frame(0) + count*framesPerBuffer*maxOutChannels, framesPerBuffer);
        count++;
    }
    else
    {
        memset(outputBuffer, 0, sizeof(float)*framesPerBuffer*maxOutChannels);
        pi->audio_data(silencAudioBuffer->get_Frame(0), framesPerBuffer);
    }

    return paContinue;
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
