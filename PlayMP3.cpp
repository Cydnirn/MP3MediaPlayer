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
        driver = Pa_GetDefaultOutputDevice();

        // Set up the buffer size and other parameters
        buffer_size = mpg123_outblock(mh);
        buffer = std::shared_ptr<char>(new char[buffer_size], std::default_delete<char[]>());
    }

    void PlayMP3::music(char* mp3) {
        track = mp3;
        int openResult = mpg123_open(mh, mp3);
        if (openResult != MPG123_OK) {
            std::cerr << "Failed to open MP3 file: " << mpg123_strerror(mh) << std::endl;
            throw std::runtime_error("Failed to open MP3 file");
        }

        int formatResult = mpg123_getformat(mh, &rate, &channels, &encoding);
        if (formatResult != MPG123_OK) {
            std::cerr << "Failed to get MP3 format: " << mpg123_strerror(mh) << std::endl;
            throw std::runtime_error("Failed to get MP3 format");
        }

        // Set up PortAudio stream parameters
        param.device = driver;
        param.channelCount = channels;
        param.sampleFormat = paFloat32; // Use float32 format for PortAudio
        param.suggestedLatency = Pa_GetDeviceInfo(driver)->defaultLowOutputLatency;
        param.hostApiSpecificStreamInfo = nullptr;
    }

    void PlayMP3::play() {
        std::cout << "\033[33;1m\u25B6 Playing the song: \033[35;1m ";
        std::cout << track << "\033[m\n";

        // Use a smaller, fixed buffer size for lower latency
        const size_t frames = 1024; // Smaller buffer for reduced latency

        // Open stream with optimized settings
        err = Pa_OpenStream(
                &stream,
                nullptr,
                &param,
                rate,
                frames,
                paClipOff,
                nullptr,
                nullptr
        );

        if (err != paNoError) {
            std::cerr << "Failed to open PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
            throw std::runtime_error("PortAudio stream opening failed");
        }

        err = Pa_StartStream(stream);
        if (err != paNoError) {
            std::cerr << "Failed to start PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
            throw std::runtime_error("PortAudio stream starting failed");
        }

        // Allocate buffers
        std::vector<float> floatBuffer(frames * channels);
        std::vector<short> shortBuffer(frames * channels);

        // Read and write loop with more efficient processing
        while (true) {
            // Read directly into the short buffer
            size_t bytesToRead = shortBuffer.size() * sizeof(short);
            if (mpg123_read(mh, reinterpret_cast<unsigned char*>(shortBuffer.data()),
                            bytesToRead, &done) != MPG123_OK) {
                break;
            }

            // Calculate actual frames read
            size_t framesRead = done / (sizeof(short) * channels);
            if (framesRead == 0) break;

            // Convert samples more efficiently
            for (size_t i = 0; i < framesRead * channels; ++i) {
                floatBuffer[i] = static_cast<float>(shortBuffer[i]) / 32768.0f;
            }

            // Write data to stream
            err = Pa_WriteStream(stream, floatBuffer.data(), framesRead);
            if (err != paNoError) {
                std::cerr << "Failed to write to stream: " << Pa_GetErrorText(err) << std::endl;
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