//
// Created by rei on 7/15/25.
//

#include "Menu.h"

#include <iostream>

void Menu::displayMainMenu() {
    std::cout << "\n====================    Menu    ====================\n";
    std::cout << "| 1. See Library\n| 2. See Playlist \n| 3. Playback Control \n| 4. Exit\n";
}
void Menu::displayMusic(const Music& music)
{
    std::cout << ". " << music.title << " - " << music.artist << ", release " << music.year << std::endl;
}

void Menu::displayMusicList(const std::vector<Music>& musicList)
{
    int i = 1;
    std::cout << "\n====================    Music List    ====================\n";
    for (const auto& music : musicList) {
        std::cout << i;
        displayMusic(music);
        i++;
    }
}
void Menu::displayMusicMenu(const bool& sortYear, const bool& sortName)
{
    std::cout << "\n====================    Menu    ====================\n";
    if (sortYear)
    {
        std::cout << "| 1. Sort by Year (A)\n";
    } else
    {
        std::cout << "| 1. Sort by Year (V)\n";
    }
    if (sortName)
    {
        std::cout << "| 2. Sort by Title (Z-A)\n";
    } else
    {
        std::cout << "| 2. Sort by Title (A-Z)\n";
    }
    std::cout << "| 3. Search Music\n| 4. Add to Playlist\n| 5. Back to Main Menu\n";
}

void Menu::displayMusicSearch(const std::unique_ptr<Library>& lib, const std::string& keyword)
{
    const auto musicList = lib->searchTitle(keyword);
    std::cout << "\n====================    Search Results for \"" << keyword << "\"    ====================\n";
    if (musicList.empty())
    {
        std::cout << "No musics found!\n";
    } else
    {
        int i = 1;
        for (const auto& music : musicList) {
            std::cout << i;
            displayMusic(music);
            i++;
        }
    }
}

void Menu::displayPlaylist(const std::unique_ptr<Queue>& playlist)
{
    std::cout << "===================== Playlist ====================\n";
    if (playlist->isEmpty()) {
        std::cout << "Playlist empty!\n";
        return;
    }
    playlist->playlistIterate();
}

void Menu::displayCurrentPlaying(const std::unique_ptr<Queue>& queue)
{
    if (const auto current = queue->currentMusic(); current == nullptr) {
        std::cout << "No music is currently playing.\n";
        return;
    }
    std::cout << "\n====================    Now Playing    ====================\n";
    std::cout << "Now playing: " << queue->currentMusic()->music.title << " - "
              << queue->currentMusic()->music.artist << std::endl;
}

void Menu::displayPlaybackControl(const std::unique_ptr<MP3MediaPlayer::PlayMP3>& player)
{
    std::cout << "\n====================== Playback Control ===============\n";
    if (player->isPlaying())
    {
        std::cout << "| 1. Pause ";
    } else
    {
        std::cout << "| 1. Play ";
    }
    std::cout << "| 2. Stop | 3. Next | 4. Quit \n";
}
