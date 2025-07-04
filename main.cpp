#include <iostream>
#include <string>
#include <limits>
#include "queue.cpp"
#include <memory>

#include "library.h"
#include "Files.h"
#include "PlayMP3.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main()
{
    auto player = std::make_unique<MP3MediaPlayer::PlayMP3>();
    std::vector<std::string> mp3Entry = files.findMp3("");
    auto music = files.getMusic(mp3Entry);
    std::string keyword;
    std::string x,y;
    int pos;
    int menu;
    bool sortYear = true;
    bool sortName = true;
    bool start = true;

    while(start) {

        printMusicList(music);
        printPlaylist();
        std::cout << "====================    Menu    ====================\n";
        std::cout << "| 1.Sort Tahun | 2.Sort Nama | 3.Search Musik | 4.Tambah Musik | 5.Exit : ";
        std::cin >> menu;
        switch (menu) {
        case 1:
            if (sortYear) {
                sortByYearDes(music);
                sortYear = !sortYear;
            } else {
                sortByYearAsc(music);
                sortYear = !sortYear;
            }
            break;

        case 2:
            if (sortName) {
                sortByHurufDes(music);
                sortName = !sortName;
            } else {
                sortByHurufAsc(music);
                sortName = !sortName;
            }
        case 5:
            start = false;
            break;
        default:
            break;
        }
    }
    return 0;
}

    //std::cout << "====================================================\n";
