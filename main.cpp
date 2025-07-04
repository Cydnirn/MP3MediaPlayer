#include <iostream>
#include <string>
#include <limits>
#include "queue.cpp"
<<<<<<< HEAD
#include "library.cpp"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    std::vector<musicLib> music = musicSample();
    std::string keyword;
    int pos;
    int menu;
    int counterX = 0;
    int counterY = 0;

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
            break;

            case 3:
            std::cout << " Masukkan Judul Musik : ";
            std::cin.ignore();
            std::getline(std::cin, keyword);
            searchTitle(music, keyword);
            break;

            case 4:
            int pilihan;
            std::cout << "Masukkan Nomer musik yang ingin ditambahkan ke playlist: ";
            std::cin >> pilihan;

            if(!(std::cin >> pilihan)) {
                std::cout << "Pilihan harus berupa angka!\n\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }

            if (pilihan >= 1 && pilihan <= music.size()) {
                musicLib selected = music[pilihan - 1];
                addMusic(selected.judul, selected.artist, selected.year);
            } else {
                std::cout << "Nomer tidak valid!\n";
            }
            break;
            
            case 5:
            return 0;
        }
    }
}
    
    //std::cout << "====================================================\n";
=======
#include <memory>
#include "PlayMP3.h"

int main()
{
    std::string x,y;
    int pos;
    int menu;
    bool start = true;

    while(start) {
        nowPlaying();
        std::cout << std::endl;
        std::cout << "Pilih Menu dibawah : \n";
        std::cout << "1. Tambah Music\n";
        std::cout << "2. Hapus Music\n";
        std::cout << "3. Next Music\n";
        std::cout << "4. Tampilkan Playlist\n";
        std::cout << "5. Clear Playlist\n";
        std::cout << "6. Exit\n";
        std::cout << "Masukkan Nomer pada Menu [1..6] : ";
        std::cin >> menu;

        switch (menu)
        {
        case 1:
            std::cout << "Input Judul Music : ";
            std::cin >> x;
            std::cout << "Input Artist : ";
            std::cin >> y;
            addMusic(x,y);
            break;
        case 2:
            std::cout << "Nomer urutan musik yang ingin dihapus : ";
            std::cin >> pos;
            removeMusic(pos);
            break;
        case 6:
            start = false;
        default:
            break;
        }
    }
    return 0;
}
>>>>>>> 24e756c7c238d07998031604e0d9395ba2bc5bbd
