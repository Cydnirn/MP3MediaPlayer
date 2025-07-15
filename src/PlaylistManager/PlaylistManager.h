//
// Created by rei on 7/16/25.
//

#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H
#include <thread>
#include <atomic>
#include <chrono>
#include "../queue.h"
#include "../MP3MediaPlayer/PlayMP3.h"

class PlaylistManager {
private:
    const std::unique_ptr<Queue>& playlist;
    const std::unique_ptr<MP3MediaPlayer::PlayMP3>& player;
    std::atomic<bool> stopFlag;
    std::thread workerThread;

public:
    PlaylistManager(const std::unique_ptr<Queue>& playlist,
                   const std::unique_ptr<MP3MediaPlayer::PlayMP3>& player)
        : playlist(playlist), player(player), stopFlag(false) {}

    void start();

    void stop();

    ~PlaylistManager() {
        stop();
    }

private:
    void ConsumePlaylist() const;
};
#endif //PLAYLISTMANAGER_H
