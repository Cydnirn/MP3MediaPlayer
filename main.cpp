#include <iostream>
#include "queue.cpp"
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