#include <iostream>
#include <vector>
#include <string>
#include "library.h"

#include <algorithm>

#include "Menu.h"
//#Sortir Tahun 0-9

std::vector<Music> Library::getMusicList() const {
    return musicList;
}

void Library::setMusicList(const std::vector<Music>& list)
{
    musicList = list;
}

void Library::sortByYearDes() {
    const int size = musicList.size();
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

void Library::sortByYearAsc() {
    const int size = musicList.size();
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

void Library::sortByHurufAsc() {
    const int size = musicList.size();
    for (int i = 0; i < size -1; i++) {
        bool swapped = false;
        for (int j = 0; j < size - i - 1; j++) {
            if (musicList[j].title < musicList[j + 1].title) {
            std::swap(musicList[j], musicList[j + 1]);
            swapped = true;
            }
        }
            if (!swapped) break;
    }
}

//#Sortir Huruf A-Z

void Library::sortByHurufDes() {
    const int size = musicList.size();
    for (int i = 0; i < size -1; i++) {
        bool swapped = false;
        for (int j = 0; j < size - i - 1; j++) {
            if (musicList[j].title > musicList[j + 1].title) {
            std::swap(musicList[j], musicList[j + 1]);
            swapped = true;
            }
        }
            if (!swapped) break;
    }
}

void Library::sortYear(bool desc) {
    if (desc) {
        sortByHurufDes();
    } else {
        sortByYearAsc();
    }
}

void Library::sortHuruf(bool desc) {
    if (desc) {
        sortByHurufDes();
    } else {
        sortByHurufAsc();
    }
}

//Mencari musik berdasarkan judul
std::vector<Music> Library::searchTitle(const std::string& keyword) const
{
    std::vector<Music> result;
    for (const auto& music : musicList) {
        std::string lowerTitle = music.title;
        std::string lowerKeyword = keyword;
        std::ranges::transform(lowerTitle, lowerTitle.begin(), ::tolower);
        std::ranges::transform(lowerKeyword, lowerKeyword.begin(), ::tolower);
        if (lowerTitle.find(lowerKeyword) != std::string::npos) {
            result.emplace_back(music);
        }
    }
    return result;
}
