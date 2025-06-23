//
// Created by rei on 6/23/25.
//

#include "PlayMP3.h"

namespace MP3MediaPlayer {
    PlayMP3::PlayMP3() {
        // Initialize mpg123
        if (mpg123_init() != MPG123_OK) {
            std::cerr << "Failed to initialize mpg123" << std::endl;
            throw std::runtime_error("mpg123 initialization failed");
        }

        // Create a new mpg123 handle
        mh = mpg123_new(nullptr, &err);
        if (!mh) {
            std::cerr << "Failed to create mpg123 handle: " << mpg123_plain_strerror(err) << std::endl;
            throw std::runtime_error("mpg123 handle creation failed");
        }

        // Initialize PortAudio
        if (Pa_Initialize() != paNoError) {
            std::cerr << "Failed to initialize PortAudio" << std::endl;
            throw std::runtime_error("PortAudio initialization failed");
        }
        const PaDeviceInfo* deviceInfo;
        int numDevices = Pa_GetDeviceCount();

        for( int DevIndex=0; DevIndex<numDevices; DevIndex++ )
        {
            deviceInfo = Pa_GetDeviceInfo( DevIndex );

            std::string str = Pa_GetHostApiInfo(deviceInfo->hostApi)->name;

            std::cout << "DEV: ApiInfo: " << str;
            std::cout << "defaultSampleRate = " << deviceInfo->defaultSampleRate;
            std::cout << "maxInputChannels = " << deviceInfo->maxInputChannels;
            std::cout << "maxOutputChannels = " << deviceInfo->maxOutputChannels;
        }
        driver = Pa_GetDefaultOutputDevice();

        // Set up the buffer size and other parameters
        buffer_size = mpg123_outblock(mh);
        buffer = std::shared_ptr<char>(new char[buffer_size], std::default_delete<char[]>());
    }

    void PlayMP3::music(char* mp3) {
        track = mp3;
        mpg123_open(mh, mp3);
        mpg123_getformat(mh, &rate, &channels, &encoding);

        // Set up PortAudio stream parameters
        param.device = driver;
        param.channelCount = channels;
        param.sampleFormat = paFloat32; // Use float32 format for PortAudio
        param.suggestedLatency = Pa_GetDeviceInfo(driver)->defaultLowOutputLatency;
        param.hostApiSpecificStreamInfo = nullptr;
        err = Pa_OpenStream(&stream, nullptr, &param, rate, buffer_size, paClipOff, nullptr, nullptr);
        if (err != paNoError) {
            std::cerr << "Failed to open PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
            throw std::runtime_error("PortAudio stream opening failed");
        }
    }

    void PlayMP3::play() {
        std::cout << "\033[33;1m\u25B6 Playing the song: \033[35;1m ";
        std::cout << track << "\033[m\n";

        err = Pa_StartStream(stream);
        if (err != paNoError) {
            std::cerr << "Failed to start PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
            throw std::runtime_error("PortAudio stream starting failed");
        }
        while(true){
            err = mpg123_read(mh, reinterpret_cast<unsigned char*>(buffer.get()), buffer_size, nullptr);
            if (err == MPG123_DONE) {
                std::cout << "\033[33;1m\u25B6 Finished playing the song: \033[35;1m ";
                std::cout << track << "\033[m\n";
                break;
            } else if (err < 0) {
                std::cerr << "Error reading MP3 data: " << mpg123_strerror(mh) << std::endl;
                break;
            }
            err = Pa_WriteStream(stream, buffer.get(), buffer_size / (sizeof(float) * channels));
            if( err != paNoError) {
                std::cerr << "Failed to write to PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
                break;
            }
        }
        err = Pa_StopStream(stream);
        if (err != paNoError) {
            std::cerr << "Failed to stop PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
            throw std::runtime_error("PortAudio stream stopping failed");
        }
    }

    PlayMP3::~PlayMP3() {
        err = Pa_CloseStream(stream);
        if (err != paNoError) {
            std::cerr << "Failed to close PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
        }
        mpg123_close(mh);
        mpg123_delete(mh);
        mpg123_exit();
        err = Pa_Terminate();
        if (err != paNoError) {
            std::cerr << "Failed to terminate PortAudio: " << Pa_GetErrorText(err) << std::endl;
        }
    }
} // MP3MediaPlayer