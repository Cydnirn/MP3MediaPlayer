#include <iostream>
#include <string>
#include <memory>
#include "queue.h"
#include "library.h"
#include "Files.h"
#include "Menu.h"
#include "MP3MediaPlayer/PlayMP3.h"
#include "PlaylistManager/PlaylistManager.h"

void LibraryMenu(const std::unique_ptr<Menu>& menu,
                 const std::unique_ptr<Library>& library,
                 const std::unique_ptr<Queue>& playlist,
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
        menu->displayMusicSearch(keyword);
        std::cin.ignore();
        break;
    case 4:
        menu->clearScreen();
        menu->displayMusicList(library->getMusicList());
        std::cout << "\n\nEnter the index number of the song to add to playlist: ";
        std::cin >> libIndex;
        if (libIndex < 1 || libIndex > library->getMusicList().size()) {
            std::cout << "Invalid index! Please try again.\n";
        }
        else{
            // Add the selected music to the playlist
            const Music music = library->getMusicList().at(libIndex - 1);
            playlist->addMusic(music);
            std::cout << "Added to playlist: " << music.title << " by " << music.artist << "\n";
            std::cin.ignore();
            std::cin.get();
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
bool& playMenu)
{
    int playChoice;
    menu->clearScreen();
    menu->displayPlaylist();
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
    const std::unique_ptr<PlaylistManager>& playlist_manager,
    bool& controlMenu)
{
    menu->clearScreen();
    menu->displayCurrentPlaying();
    menu->displayPlaybackControl();
    int controlChoice;
    std::cin >> controlChoice;
    switch (controlChoice)
    {
    case 1: // Play
        if (!player->isPlaying() && !playlist->isEmpty())
        {
            player->play();
            //playlist_manager->stop();
        } else if (player->isPlaying()) // Pause
        {
            //playlist_manager->stop();
            player->pause();
        }
        break;
    case 2: // Next
        playlist_manager->stop(); // Stop the current playback to allow the next song to play
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        player->stop();
        playlist->playNext();
        if (const auto nextMusic = playlist->currentMusic(); nextMusic)
        {
            player->music(nextMusic->music.path.c_str());
            player->play();
        }
        playlist_manager->start();
        break;
    case 3:// Quit
        controlMenu = false;
        break;
    default:
        std::cout << "Invalid option. Press Enter to continue...";
        std::cin.ignore();
        std::cin.get();
        break;
    }
}



int main()
{
    const auto player = std::make_unique<MP3MediaPlayer::PlayMP3>();
    const auto playlist = std::make_unique<Queue>();
    const auto library = std::make_unique<Library>();
    const auto menu = std::make_unique<Menu>(playlist, library, player);
    // Initialize Files and get the list of MP3 files
    const std::vector<std::string> mp3Entry = files.findMp3("");
    library->setMusicList(files.getMusic(mp3Entry));

    // Initialize PlaylistManager with the playlist and player
    const auto playlist_manager = std::make_unique<PlaylistManager>(playlist, player);

    // Initialize the observer pattern
    playlist->addObserver(menu.get());

    std::string keyword, filename, x,y;
    int choice;
    bool sortYear = true,  sortName = true,
    start = true, musicMenu = false, playMenu = false, controlMenu = false;

    playlist_manager->start();
    while(start)
    {
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
                LibraryMenu(menu, library, playlist,
                    sortYear, sortName, musicMenu,
                    keyword);
            }
            break;
        case 2:
            // Display playlist menu
            playMenu = true;
            while (playMenu)
            {
                PlaylistMenu(menu, playMenu);
            }
            break;
        case 3:
            // Display playback control menu
            controlMenu = true;
            while (controlMenu)
            {
                PlaybackMenu(menu, playlist, player, playlist_manager, controlMenu);
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
    if (playlist_manager->isRunning())
    {
        playlist_manager->stop();
    }
    playlist->clearPlaylist();
    player->stop();
    return 0;
}
