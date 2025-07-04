#include <iostream>
#include <queue>
#include <vector>
#include <string>


struct musicLib {
    std::string judul, artist;
    int year;
};

//#Sortir Tahun 0-9

void sortByYearDes(std::vector<musicLib>& musicList) {
    int size = musicList.size();
    for (int i = 0; i < size - 1;i++) {
        bool swapped = false;
        for (int j = 0; j < size - i - 1; j++) {
            if(musicList[j].year < musicList[j + 1].year) {
                std::swap(musicList[j], musicList[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

//#Sortir Tahun 9-0

void sortByYearAsc(std::vector<musicLib>& musicList) {
    int size = musicList.size();
    for (int i = 0; i < size - 1;i++) {
        bool swapped = false;
        for (int j = 0; j < size - i - 1; j++) {
            if(musicList[j].year > musicList[j + 1].year) {
                std::swap(musicList[j], musicList[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

//#Sortir Huruf Z-A

void sortByHurufAsc(std::vector<musicLib>& musicList) {
    int size = musicList.size();
    for (int i = 0; i < size -1; i++) {
        bool swapped = false;
        for (int j = 0; j < size - i - 1; j++) {
            if (musicList[j].judul < musicList[j + 1].judul) {
            std::swap(musicList[j], musicList[j + 1]);
            swapped = true;
            }
        }
            if (!swapped) break;
    }
}

//#Sortir Huruf A-Z

void sortByHurufDes(std::vector<musicLib>& musicList) {
    int size = musicList.size();
    for (int i = 0; i < size -1; i++) {
        bool swapped = false;
        for (int j = 0; j < size - i - 1; j++) {
            if (musicList[j].judul > musicList[j + 1].judul) {
            std::swap(musicList[j], musicList[j + 1]);
            swapped = true;
            }
        }
            if (!swapped) break;
    }
}

void printMusic(const musicLib& music) {
std::cout << ". " << music.judul << " - " << music.artist << ", release " << music.year << std::endl;
}

//##Display list musik

void printMusicList(std::vector<musicLib>& musicList) {
    int i = 1;
    std::cout << "==================== Music List ====================\n";
    for (const auto& music : musicList) {
      std::cout << i; 
      printMusic(music);  
      i++; 
    }
}

//Mencari musik berdasarkan judul
void searchTitle(const std::vector<musicLib>& musicList, const std::string& keyword) {
    std::vector<musicLib> result;
    for (const auto& music : musicList) {
        if (music.judul.find(keyword) != std::string::npos) {
            result.push_back(music);
        }
    }

    std::cout << "\nHasil pencarian judul \"" << keyword << "\":\n";
    if (result.empty()) {
        std::cout << "Tidak ditemukan lagu dengan judul tersebut.\n";
    } else {
        printMusicList(const_cast<std::vector<musicLib>&>(result));
    }
}

//sample doang buat test output 

std::vector<musicLib> musicSample() {
    musicLib music1 = {"Heaven and Back", "Chase Atlantic", 2022};
    musicLib music2 = {"Castle of Glass", "Linkin Park", 2011};
    musicLib music3 = {"Bloodline", "Ariana Grande", 2020};
    musicLib music4 = {"8 Letters", "Why Don't we", 2018};

    std::vector<musicLib> musicList = {music1, music2, music3, music4};

    return {music1, music2, music3, music4};
}