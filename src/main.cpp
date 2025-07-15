#include <iostream>
#include <string>
#include <memory>
#include <chrono>
#include "queue.h"
#include "library.h"
#include "Files.h"
#include "Menu.h"
#include "PlayMP3.h"

int main()
{
    const auto player = std::make_unique<MP3MediaPlayer::PlayMP3>();
    auto playlist = std::make_unique<Queue>();
    const auto library = std::make_unique<Library>();
    const auto menu = std::make_unique<Menu>();
    const std::vector<std::string> mp3Entry = files.findMp3("");
    library->setMusicList(files.getMusic(mp3Entry));
    std::string keyword, filename;
    std::string x,y;
    int pos;
    int choice = 0;
    bool sortYear = true;
    bool sortName = true;
    bool start = true;
    bool manual = false;
    std::string currentSong;
    bool isPlaying = false;

    while(start)
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
            // Normal menu display
        menu->displayMusicList(library->getMusicList());
        menu->displayPlaylist(std::move(playlist));
        if (manual)
        {
            std::cout << "\n====================    Manual Menu    ====================\n";
            std::cout << "| 1. Pause | 2. Stop | 3. Resume | 4. Exit Manual Mode |\n";
            std::cout << "Current Song: " << currentSong << "\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;
            switch (choice)
            {
            case 1:
                player->pause();
                std::cout << "Paused: " << currentSong << "\n";
                break;
            case 2:
                player->stop();
                isPlaying = false;
                manual = false;
                std::cout << "Stopped: " << currentSong << "\n";
                currentSong.clear();
                break;
            case 3:
                player->play();
                isPlaying = true;
                std::cout << "Resumed: " << currentSong << "\n";
                break;
            default:
                std::cout << "Invalid Choice\n";
                std::cout << "Press Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            }
        } else
        {
            std::cout << "\n====================    Menu    ====================\n";
            std::cout << "| 1.Sort Tahun | 2.Sort Nama | 3.Search Musik | \n "
                         "4.Tambah Playlist | 5.Next Song | 6. Exit  | \n"
                         "7. Play Manual | 8. Enter Manual Mode: ";
            std::cin >> choice;
            switch (choice)
            {
            case 1:
                if (sortYear) {
                    library->sortByYearDes();
                    sortYear = !sortYear;
                } else {
                    library->sortByYearAsc();
                    sortYear = !sortYear;
                }
                break;
            case 2:
                if (sortName) {
                    library->sortByHurufDes();
                    sortName = !sortName;
                } else {
                    library->sortByHurufAsc();
                    sortName = !sortName;
                }
                break;
            case 3:
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
                    isPlaying = true;
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
    }

    // Make sure to stop playback before exiting
    player->stop();
    return 0;
}
