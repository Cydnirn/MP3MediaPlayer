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
    bool& sortYear, bool& sortName, bool& musicMenu,
    std::string& keyword, int& libChoice)
{
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
        std::cout << "Enter search keyword: ";
        std::cin.ignore();
        std::getline(std::cin, keyword);
        menu->displayMusicSearch(std::move(library), keyword);
        std::cin.ignore();
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

int main()
{
    const auto player = std::make_unique<MP3MediaPlayer::PlayMP3>();
    auto playlist = std::make_unique<Queue>();
    const auto library = std::make_unique<Library>();
    const auto menu = std::make_unique<Menu>();
    const std::vector<std::string> mp3Entry = files.findMp3("");
    library->setMusicList(files.getMusic(mp3Entry));
    std::string keyword, filename, x,y;
    int pos,  choice, libChoice;
    bool sortYear = true,  sortName = true,
    start = true, musicMenu = false, manual = false;
    std::string currentSong;

    while(start)
    {
        menu->clearScreen();
            // Normal menu display
        std::cout << "\n====================    MP3 Player    ====================\n";
        menu->displayMainMenu();
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            // Display music library
            musicMenu = true;
            while (musicMenu)
            {
                LibraryMenu(menu, std::move(library),
                    sortYear, sortName, musicMenu,
                    keyword, libChoice);
            }
            break;
        case 2:
                // Handle search functionality if needed
                std::cout << "Enter search keyword: ";
                std::cin.ignore();
                std::getline(std::cin, keyword);
                menu->displayMusicSearch(std::move(library), keyword);
                std::cin.ignore();
                break;
        case 4:
                std::cout << "Enter the index number of the song: ";
                std::cin.ignore();
                std::cin >> pos;
                playlist->addMusic(pos, library->getMusicList());
                break;
            case 6:
                start = false;
                break;
            case 7:
                std::cout << "Enter the path to the MP3 file: ";
                std::cin.ignore();
                std::getline(std::cin, filename);
                try {
                    player->music(filename.c_str());
                    player->play();
                    currentSong = filename;
                    manual = true;
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
