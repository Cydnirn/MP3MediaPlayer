#include <iostream>
#include "queue.cpp"
#include <memory>
#include "PlayMP3.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    std::string x,y;
    int pos;
    int menu;

    while(true) {

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
        }

    for (int i = 1; i <= 5; i++) {
        // TIP Press <shortcut actionId="Debug"/> to start debugging your code. We have set one <icon src="AllIcons.Debugger.Db_set_breakpoint"/> breakpoint for you, but you can always add more by pressing <shortcut actionId="ToggleLineBreakpoint"/>.
        std::cout << "i = " << i << std::endl;
    }

    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}