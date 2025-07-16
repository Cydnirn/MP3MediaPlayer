//
// Created by rei on 7/15/25.
//

#include "Menu.h"

#include <iostream>

void Menu::displayMainMenu() {
    inNowPlaying = false;
    std::cout << "\n====================    Menu    ====================\n";
    std::cout << "| 1. See Library\n| 2. See Playlist \n| 3. Playback Control \n| 4. Exit\n";
}
void Menu::displayMusic(const Music& music)
{
    std::cout << ". " << music.title << " - " << music.artist << ", release " << music.year << std::endl;
}

void Menu::displayMusicList(const std::vector<Music>& musicList)
{
    inNowPlaying = false;
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

void Menu::displayMusicSearch(const std::string& keyword) const
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

void Menu::displayPlaylist()
{
    inNowPlaying = false;
    std::cout << "===================== Playlist ====================\n";
    if (playlist->isEmpty()) {
        std::cout << "Playlist empty!\n";
        return;
    }
    playlist->playlistIterate();
}

void Menu::displayPlaylistMenu()
{
    inNowPlaying = false;
    std::cout << "\n====================    Playlist Menu    ====================\n";
    std::cout << "| 1. Remove Music\n| 2. Move Music Left\n| 3. Move Music Right\n| 4. Back to Main Menu\n";
}


void Menu::displayCurrentPlaying()
{
    inNowPlaying = true;
    if (const auto current = playlist->currentMusic(); current == nullptr) {
        std::cout << "No music is currently playing.\n";
        return;
    }
    std::cout << "\n====================    Now Playing    ====================\n";
    std::cout << "Now playing: " << playlist->currentMusic()->music.title << " - "
              << playlist->currentMusic()->music.artist << std::endl;
}

void Menu::displayPlaybackControl() const
{
    std::cout << "\n====================== Playback Control ===============\n";
    if (player->isPlaying())
    {
        std::cout << "| 1. Pause ";
    } else if (!player->isPlaying() && !playlist->isEmpty())
    {
        std::cout << "| 1. Play ";
    } else
    {
        std::cout << "| 1. Play (No music in playlist) ";
    }
    std::cout << "| 2. Next | 3. Quit \n";
}

void Menu::onMusicChanged()
{
    if (inNowPlaying)
    {
        clearScreen();
        displayCurrentPlaying();
        displayPlaybackControl();
    }
}
