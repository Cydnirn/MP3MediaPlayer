#include <iostream>
#include <string>
#include <memory>
#include "queue.h"
#include "library.h"
#include "Files.h"
#include "Menu.h"
#include "MP3MediaPlayer/PlayMP3.h"

void LibraryMenu(const std::unique_ptr<Menu>& menu,
                 const std::unique_ptr<Library>& library,
                 const std::unique_ptr<Queue>& playlist,
                 const std::unique_ptr<MP3MediaPlayer::PlayMP3>& player,
                 bool& sortYear, bool& sortName, bool& musicMenu,
                 std::string& keyword)
{
    uint64_t libIndex;
    int libChoice;
    menu->clearScreen();
    menu->displayMusicList(library->getMusicList());
    menu->displayMusicMenu(sortYear, sortName);
    std::cin >> libChoice;
    switch (libChoice)
    {
    case 1:
        library->sortYear(sortYear);
        sortYear = !sortYear;
        break;
    case 2:
        library->sortHuruf(sortName);
        sortName = !sortName;
        break;
    case 3:
        menu->clearScreen();
        std::cout << "Enter search keyword: ";
        std::cin.ignore();
        std::getline(std::cin, keyword);
        menu->displayMusicSearch(std::move(library), keyword);
        std::cin.ignore();
        break;
    case 4:
        menu->clearScreen();
        menu->displayMusicList(library->getMusicList());
        std::cout << "\n\nEnter the index number of the song to add to playlist: ";
        std::cin >> libIndex;
        if (libIndex < 1 || libIndex > library->getMusicList().size()) {
            std::cout << "Invalid index! Please try again.\n";
        } else {
            // Add the selected music to the playlist
            const Music music = library->getMusicList().at(libIndex - 1);
            playlist->addMusic(music);
            std::cout << "Added to playlist: " << music.title << " by " << music.artist << "\n";
            std::cin.ignore();
            std::cin.get();
        }
        if (!player->isPlaying())
        {
            player->music(playlist->currentMusic()->music.path.c_str());
            player->play();
        }
        break;
    case 5:
        musicMenu = false;
        break; // Back to main menu
    default:
        std::cout << "Invalid option. Press Enter to continue...";
        std::cin.ignore();
        std::cin.get();
        break;
    }
}

void PlaylistMenu(const std::unique_ptr<Menu>& menu,
    const std::unique_ptr<Queue>& playlist, bool& playMenu)
{
    int playChoice;
    menu->clearScreen();
    menu->displayPlaylist(playlist);
    std::cout << "| 1. Exit\n ";
    std::cin >> playChoice;
    switch (playChoice)
    {
    case 1:
        playMenu = false;
        break;
    default:
        std::cout << "Invalid option. Press Enter to continue...";
        std::cin.ignore();
        std::cin.get();
        break;
    }
}

void PlaybackMenu(const std::unique_ptr<Menu>& menu,
    const std::unique_ptr<Queue>& playlist,
    const std::unique_ptr<MP3MediaPlayer::PlayMP3>& player,
    bool& controlMenu)
{
    menu->clearScreen();
    menu->displayCurrentPlaying(playlist);
    menu->displayPlaybackControl(player);
    int controlChoice;
    std::cin >> controlChoice;
    switch (controlChoice)
    {
    case 1: // Play
        if (!player->isLoaded())
        {
            player->music(playlist->currentMusic()->music.path.c_str());
        }
        if (!player->isPlaying() && !playlist->isEmpty())
        {
            player->play();
        } else if (player->isPlaying())
        {
            player->pause();
        }
        break;
    case 2: // Stop
        player->stop();
        break;
    case 3: // Next
        player->stop(); // Stop current playback before playing next
        playlist->playNext();
        if (!playlist->isEmpty() && playlist->currentMusic())
        {
            player->music(playlist->currentMusic()->music.path.c_str());
            player->play();
        }
        break;
    case 4: // Next
        controlMenu = false;
        break;
    default:
        std::cout << "Invalid option. Press Enter to continue...";
        std::cin.ignore();
        std::cin.get();
        break;
    }
}

void ConsumePlaylist(const std::unique_ptr<Queue>& playlist, const std::unique_ptr<MP3MediaPlayer::PlayMP3>& player)
{
    if (!playlist->isEmpty()) {
        if (const auto currentMusic = playlist->currentMusic(); currentMusic && !player->isPlaying())
        {
            player->music(currentMusic->music.path.c_str());
            player->play();
        }
        if (player->isDone())
        {
            playlist->playNext();
        }
    }
}

int main()
{
    const auto player = std::make_unique<MP3MediaPlayer::PlayMP3>();
    const auto playlist = std::make_unique<Queue>();
    const auto library = std::make_unique<Library>();
    const auto menu = std::make_unique<Menu>();
    const std::vector<std::string> mp3Entry = files.findMp3("");
    library->setMusicList(files.getMusic(mp3Entry));
    std::string keyword, filename, x,y;
    int choice;
    bool sortYear = true,  sortName = true,
    start = true, musicMenu = false, playMenu = false, controlMenu = false;

    while(start)
    {
        ConsumePlaylist(playlist, player);
        menu->clearScreen();
            // Normal menu display
        std::cout << "___  ___       ___________ _                       \n";
        std::cout << "|  \\/  |      |____ | ___ \\ |                      \n";
        std::cout << "| .  . |_ __      / / |_/ / | __ _ _   _  ___ _ __ \n";
        std::cout << "| |\\/| | '_ \\     \\ \\  __/| |/ _` | | | |/ _ \\ '__|\n";
        std::cout << "| |  | | |_) |.___/ / |   | | (_| | |_| |  __/ |   \n";
        std::cout << "\\_|  |_/ .__/ \\____/\\_|   |_|\\__,_|\\__, |\\___|_|   \n";
        std::cout << "       | |                          __/ |          \n";
        std::cout << "       |_|                         |___/           \n";
        menu->displayMainMenu();
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            // Display music library
            musicMenu = true;
            while (musicMenu)
            {
                LibraryMenu(menu, library, playlist, player,
                    sortYear, sortName, musicMenu,
                    keyword);
            }
            break;
        case 2:
            // Display playlist menu
            playMenu = true;
            while (playMenu)
            {
                PlaylistMenu(menu, playlist, playMenu);
            }
            break;
        case 3:
            // Display playback control menu
            controlMenu = true;
            while (controlMenu)
            {
                PlaybackMenu(menu, playlist, player, controlMenu);
            }
            break;
        case 4:
            start = false;
            break;
        default:
            std::cout << "Invalid option. Press Enter to continue...";
            std::cin.get();
            break;
        }
    }
    // Make sure to stop playback before exiting
    player->stop();
    return 0;
}
