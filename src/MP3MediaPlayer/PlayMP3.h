//
// Created by rei on 6/23/25.
//

#ifndef MP3MEDIAPLAYER_PLAYMP3_H
#define MP3MEDIAPLAYER_PLAYMP3_H
#include <mpg123.h>
#include <portaudio.h>
#include <memory>
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

namespace MP3MediaPlayer {
//
// Created by rei on 6/23/25.
//
    class PlayMP3 {
        std::size_t buffer_size, done{};
        int driver, err{}, channels{}, encoding{};
        long rate{};
        std::string track;

        mpg123_handle *mh;
        std::shared_ptr<char> buffer;

        PaStreamParameters param{};
        PaStream *stream{};

        std::vector<float> inputBuffer; // Input buffer for PortAudio
        std::vector<short> outputBuffer; // Buffer for reading samples from mpg123
        std::vector<float> resampledBuffer; // Buffer for resampled output

        // Thread-related members
        std::thread playbackThread;
        std::atomic<bool> shouldStop{false};
        std::atomic<bool> isPaused{false};
        std::atomic<bool> songFinished{false};
        std::mutex mutex;

    public:
        PlayMP3();
        ~PlayMP3();
        void music(const char *);
        void play();
        void pause();
        void stop();
        bool isLoaded() const;
        bool isDone() const;
        bool isPlaying() const;

    private:
        // Private method for threaded playback
        void playbackLoop();
    };
}

#endif //MP3MEDIAPLAYER_PLAYMP3_H
