#include <iostream>
#include <string>
#include <limits>
#include "queue.cpp"
#include "library.cpp"
#include <memory>
#include "PlayMP3.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    std::vector<musicLib> music = musicSample();
    std::string keyword;
    std::string x,y;
    int pos;
    int menu;
    int counterX = 0;
    int counterY = 0;
    bool start = true;

    while(true) {

        printMusicList(music);
        printPlaylist();
        std::cout << "====================    Menu    ====================\n";
        std::cout << "| 1.Sort Tahun | 2.Sort Nama | 3.Search Musik | 4.Tambah Musik | 5.Exit : ";
        std::cin >> menu; 
        switch (menu) {
            case 1:
            counterX++;
            if (counterX / 2 == 0) {
                sortByYearDes(music);
            } else {
                sortByYearAsc(music);
            }
            break;

            case 2:
            counterY++;
            if (counterY / 2 == 0) {
                sortByHurufDes(music);
            } else {
                sortByHurufAsc(music);
            }
        }
}
    
    //std::cout << "====================================================\n";