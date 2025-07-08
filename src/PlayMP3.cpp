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

    void PlayMP3::music(const char* mp3) {
        // Stop any current playback first
        stop();

        track = mp3;
        if (const int openResult = mpg123_open(mh, mp3); openResult != MPG123_OK) {
            std::cerr << "Failed to open MP3 file: " << mpg123_strerror(mh) << std::endl;
            throw std::runtime_error("Failed to open MP3 file");
        }

        if (const int formatResult = mpg123_getformat(mh, &rate, &channels, &encoding); formatResult != MPG123_OK) {
            std::cerr << "Failed to get MP3 format: " << mpg123_strerror(mh) << std::endl;
            throw std::runtime_error("Failed to get MP3 format");
        }
        std::cout << "MP3 Format: " << rate << " Hz, " << channels << " channels, encoding: " << encoding << std::endl;
        if (rate < 48000) {
            std::cerr << "Warning: Sample rate is below 48 kHz, which may affect audio quality." << std::endl;
            rate = 48000; // Ensure minimum sample rate
        }

        // Set up PortAudio stream parameters
        param.device = driver;
        param.channelCount = channels;
        param.sampleFormat = paFloat32; // Use float32 format for PortAudio
        param.suggestedLatency = Pa_GetDeviceInfo(driver)->defaultLowOutputLatency;
        param.hostApiSpecificStreamInfo = nullptr;
    }

    void PlayMP3::play() {
        //std::cout << "\033[33;1m\u25B6 Playing the song: \033[35;1m ";
        //std::cout << track << "\033[m\n";

        // Reset control flags
        shouldStop = false;
        isPaused = false;

        // Start a new thread for playback if not already running
        if (!playbackThread.joinable()) {
            playbackThread = std::thread(&PlayMP3::playbackLoop, this);
        } else {
            // If thread is running but paused, unpause it
            isPaused = false;
        }
    }

    void PlayMP3::playbackLoop() {
        // Use a smaller, fixed buffer size for lower latency
        constexpr size_t frames = 1024; // Smaller buffer for reduced latency

        // Open stream with optimized settings
        {
            std::lock_guard<std::mutex> lock(mutex);
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
                return;
            }

            err = Pa_StartStream(stream);
            if (err != paNoError) {
                std::cerr << "Failed to start PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
                return;
            }
        }

        // Allocate buffers
        floatBuffer.resize(frames * channels);
        shortBuffer.resize(frames * channels);

        // Read and write loop with more efficient processing
        while (!shouldStop) {
            if (isPaused) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            // Read directly into the short buffer
            size_t bytesToRead = shortBuffer.size() * sizeof(short);
            if (mpg123_read(mh, reinterpret_cast<unsigned char*>(shortBuffer.data()),
                            bytesToRead, &done) != MPG123_OK) {
                break;
            }

            // Calculate actual frames read
            const size_t framesRead = done / (sizeof(short) * channels);
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

        // Clean up stream
        std::lock_guard<std::mutex> lock(mutex);
        if (stream) {
            err = Pa_StopStream(stream);
            if (err != paNoError) {
                std::cerr << "Failed to stop PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
            }
            err = Pa_CloseStream(stream);
            if (err != paNoError) {
                std::cerr << "Failed to close PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
            }
            stream = nullptr;
        }
    }

    void PlayMP3::pause()
    {
        if (playbackThread.joinable()) {
            isPaused = true;
            //std::cout << "\033[33;1m\u23F8 Paused the song: \033[35;1m " << track << "\033[m\n";
        }
    }

    bool PlayMP3::isPlaying() const
    {
        return playbackThread.joinable() && !isPaused;
    }

    void PlayMP3::stop()
    {
        if (playbackThread.joinable()) {
            // Signal thread to stop
            shouldStop = true;

            // Wait for thread to finish
            if (playbackThread.joinable()) {
                playbackThread.join();
            }

            // Reset the playback state without fully cleaning up the library resources
            std::lock_guard<std::mutex> lock(mutex);
            if (mh) {
                mpg123_close(mh);
            }

            std::cout << "\033[33;1m\u25A0 Stopped the song: \033[35;1m " << track << "\033[m\n";
        }
    }

    PlayMP3::~PlayMP3() {
        // Make sure playback thread is stopped
        stop();

        mpg123_close(mh);
        mpg123_delete(mh);
        mpg123_exit();
        err = Pa_Terminate();
        if (err != paNoError) {
            std::cerr << "Failed to terminate PortAudio: " << Pa_GetErrorText(err) << std::endl;
        }
    }
} // MP3MediaPlayer