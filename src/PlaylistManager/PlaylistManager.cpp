//
// Created by rei on 7/16/25.
//

#include "PlaylistManager.h"

void PlaylistManager::start()
{
    stopFlag.store(false);
    if (!workerThread.joinable())
    {
        workerThread = std::thread(&PlaylistManager::ConsumePlaylist, this);
    }
}

void PlaylistManager::stop()
{
    stopFlag.store(true);
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void PlaylistManager::ConsumePlaylist() const
{
    while (!stopFlag.load()) {
        if (!playlist->isEmpty()) {
            // Check if there's music to play and player isn't already playing
            if (const auto currentMusic = playlist->currentMusic(); currentMusic && !player->isPlaying())
            {
                player->music(currentMusic->music.path.c_str());
                player->play();
            }
            // Check if current track is finished, then move to next
            if (player->isDone())
            {
                playlist->playNext();
                // After moving to next track, start playing if there's still music in the playlist
                if (const auto nextMusic = playlist->currentMusic(); nextMusic)
                {
                    player->music(nextMusic->music.path.c_str());
                    player->play();
                }
            }
        }
        // Sleep to avoid high CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
