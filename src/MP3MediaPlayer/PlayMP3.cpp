//
// Created by rei on 6/23/25.
//

#include "PlayMP3.h"
#include <samplerate.h>

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
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(driver);
        if (const int openResult = mpg123_open(mh, mp3); openResult != MPG123_OK) {
            std::cerr << "Failed to open MP3 file: " << mpg123_strerror(mh) << std::endl;
            throw std::runtime_error("Failed to open MP3 file");
        }

        if (const int formatResult = mpg123_getformat(mh, &rate, &channels, &encoding); formatResult != MPG123_OK) {
            std::cerr << "Failed to get MP3 format: " << mpg123_strerror(mh) << std::endl;
            throw std::runtime_error("Failed to get MP3 format");
        }
        //std::cout << "MP3 Format: " << rate << " Hz, " << channels << " channels, encoding: " << encoding << std::endl;
        if (rate < deviceInfo->defaultSampleRate)
        {
            std::cerr << "Warning: Sample rate is below device sample rate, which may affect audio quality." << std::endl;
            std::cerr << "Device sample rate: " << deviceInfo->defaultSampleRate << " Hz" << std::endl;
        }
        // Set up PortAudio stream parameters
        param.device = driver;
        param.channelCount = channels;
        param.sampleFormat = paFloat32; // Use float32 format for PortAudio
        param.suggestedLatency = deviceInfo->defaultLowOutputLatency;
        param.hostApiSpecificStreamInfo = nullptr;
    }

    void PlayMP3::play() {
        //std::cout << "\033[33;1m\u25B6 Playing the song: \033[35;1m ";
        //std::cout << track << "\033[m\n";

        // Reset control flags
        shouldStop = false;
        isPaused = false;
        songFinished = false;

        // Start a new thread for playback if not already running
        if (!playbackThread.joinable()) {
            playbackThread = std::thread(&PlayMP3::playbackLoop, this);
        } else {
            // If thread is running but paused, unpause it
            isPaused = false;
        }
    }

    void PlayMP3::playbackLoop() {
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(param.device);
        const double deviceSampleRate = deviceInfo->defaultSampleRate;
        // Use a smaller, fixed buffer size for lower latency
        // Calculate proper MP3 frame size based on bitrate and sample rate
        long bitrate = 0;
        mpg123_getformat(mh, &rate, &channels, &encoding); // Ensure we have current format
        // mpg123_format_none(mh); // Reset format to allow reformatting
        // mpg123_format(mh, rate, channels, MPG123_ENC_FLOAT_32); // Set desired output format

        // Initiate libsamplerate
        int error;
        SRC_STATE* srcState = src_new(SRC_SINC_BEST_QUALITY, channels, &error);
        if(srcState == nullptr) {
            std::cerr << "Failed to create SRC state: " << src_strerror(error) << std::endl;
            return;
        }
        // Calculate resampling ratio
        mpg123_frameinfo2 info;
        const double resampleRatio = deviceSampleRate / static_cast<double>(rate);
        if (mpg123_info2(mh, &info) == MPG123_OK)
        {
            bitrate = info.bitrate;
        }else
        {
            bitrate = 128; // Default to 128 kbps if bitrate cannot be determined
        }// Get bitrate in kbps
        const int padding = 0; // Would need mpg123 header analysis to determine padding
        size_t frames = (144 * bitrate * 1000 / rate) + padding;
        frames = frames > 0 ? frames : 1024; // Fallback to 1024 if calculation fails
       // Open stream with optimized settings
        {
            std::lock_guard lock(mutex);
            err = Pa_OpenStream(
                    &stream,
                    nullptr,
                    &param,
                    deviceSampleRate,
                    frames,
                    paClipOff,
                    nullptr,
                    nullptr
            );

            if (err != paNoError) {
                std::cerr << "Failed to open PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
                std::cerr << "Frames " << frames << std::endl;
                std::cerr << "Sample Rate " << rate << " channels " << channels << std::endl;
                return;
            }

            err = Pa_StartStream(stream);
            if (err != paNoError) {
                std::cerr << "Failed to start PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
                return;
            }
        }

        // Allocate buffers for processing and resampling
        inputBuffer.resize(frames * channels);
        outputBuffer.resize(frames * channels);
        resampledBuffer.resize(frames * channels * 2); // Extra space for resampling

        SRC_DATA srcData;
        srcData.src_ratio = resampleRatio;
        srcData.output_frames = frames * 2; // Allow for rate increase
        srcData.data_in = inputBuffer.data();
        srcData.data_out = resampledBuffer.data();

        // Read and write loop with resampling
        while (!shouldStop) {
            if (isPaused) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            // Read MP3 data into the short buffer
            size_t bytesToRead = outputBuffer.size() * sizeof(short);
            if (mpg123_read(mh, outputBuffer.data(), bytesToRead, &done) != MPG123_OK) {
                songFinished = true;
                break;
            }

            // Calculate actual frames read
            const size_t framesRead = done / (sizeof(short) * channels);
            if (framesRead == 0) {
                songFinished = true;
                break;
            }

            // Convert samples from short to float format for resampling
            for (size_t i = 0; i < framesRead * channels; ++i) {
                inputBuffer[i] = static_cast<float>(outputBuffer[i]) / 32768.0f; // Magic number, nah it is divided by a 16 bit signed integer max value
            }

            // Set up resampling parameters
            srcData.input_frames = framesRead;
            int srcResult = src_process(srcState, &srcData);
            if (srcResult != 0) {
                std::cerr << "Resampling error: " << src_strerror(srcResult) << std::endl;
                break;
            }

            // Write resampled data to stream
            err = Pa_WriteStream(stream, resampledBuffer.data(), srcData.output_frames_gen);
            if (err != paNoError) {
                std::cerr << "Failed to write to stream: " << Pa_GetErrorText(err) << std::endl;
                break;
            }
        }
        // Clean up stream
        src_delete(srcState);

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
        if (mh)
        {
            mpg123_close(mh);
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

    bool PlayMP3::isDone() const
    {
        // Check if the playback thread is done
        return songFinished || !playbackThread.joinable() || shouldStop;
    }

    bool PlayMP3::isLoaded() const
    {
        return mh != nullptr && mpg123_getformat(mh, nullptr, nullptr, nullptr) == MPG123_OK;
    }


    void PlayMP3::stop()
    {
        songFinished = true;
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