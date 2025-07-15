//
// Created by rei on 7/15/25.
//

#include "Menu.h"

#include <iostream>

void Menu::displayMainMenu() {
    std::cout << "\n====================    Menu    ====================\n";
    std::cout << "| 1.See Library| 2.See Playlist | 3.Playback Control | 4. Exit";
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

void Menu::displayMusicSearch(const std::unique_ptr<Library>& library, const std::string& keyword)
{
    auto musicList = library->searchTitle(keyword);
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

void Menu::displayPlaylist(std::unique_ptr<Queue> playlist)
{
    std::cout << "===================== Playlist ====================\n";
    if (playlist->isEmpty()) {
        std::cout << "Playlist kosong!\n";
        return;
    }
    playlist->playlistIterate();
}

void Menu::displayCurrentPlaying(std::unique_ptr<Queue> queue)
{
    if (const auto current = queue->currentMusic(); current == nullptr) {
        std::cout << "No music is currently playing.\n";
        return;
    }
    std::cout << "\n====================    Now Playing    ====================\n";
    std::cout << "Now playing: " << queue->currentMusic()->music->title << " - "
              << queue->currentMusic()->music->artist << std::endl;
}

