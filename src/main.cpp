#include <iostream>
#include <string>
#include <memory>
#include "queue.h"
#include "library.h"
#include "Files.h"
#include "Menu.h"
#include "PlayMP3.h"

void LibraryMenu(const std::unique_ptr<Menu>& menu,
    const std::unique_ptr<Library>& library,
    const std::unique_ptr<Queue>& playlist,
    const std::unique_ptr<MP3MediaPlayer::PlayMP3>& player,
    bool& sortYear, bool& sortName, bool& musicMenu,
    std::string& keyword, int& libChoice)
{
    uint64_t libIndex;
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
    const std::unique_ptr<Queue>& playlist, int& playChoice, bool& playMenu)
{
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

int main()
{
    const auto player = std::make_unique<MP3MediaPlayer::PlayMP3>();
    const auto playlist = std::make_unique<Queue>();
    const auto library = std::make_unique<Library>();
    const auto menu = std::make_unique<Menu>();
    const std::vector<std::string> mp3Entry = files.findMp3("");
    library->setMusicList(files.getMusic(mp3Entry));
    std::string keyword, filename, x,y;
    int pos, choice, libChoice, playChoice;
    bool sortYear = true,  sortName = true,
    start = true, musicMenu = false, playMenu = false;

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
                LibraryMenu(menu, library, playlist, player,
                    sortYear, sortName, musicMenu,
                    keyword, libChoice);
            }
            break;
        case 2:
            playMenu = true;
            while (playMenu)
            {
                PlaylistMenu(menu, playlist, playChoice, playMenu);
            }
            break;
        case 4:
                start = false;
                break;
            case 7:
                std::cout << "Enter the path to the MP3 file: ";
                std::cin.ignore();
                std::getline(std::cin, filename);
                try {
                    player->music(filename.c_str());
                    player->play();
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
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
