//
// Created by rei on 7/4/25.
//

#ifndef LIBRARY_H
#define LIBRARY_H
#include <mpg123.h>

struct musicLib {
    std::string title, artist;
    uint64_t year;
    std::string path;
};

bool compareMusicByTitle(const musicLib &a, const musicLib &b);
void sortByYearDes(std::vector<musicLib>& musicList);
void sortByYearAsc(std::vector<musicLib>& musicList);
void sortByHurufAsc(std::vector<musicLib>& musicList);
void sortByHurufDes(std::vector<musicLib>& musicList);
void printMusic(const musicLib& music);
void printMusicList(std::vector<musicLib>& musicList);
void searchTitle(const std::vector<musicLib>& musicList, const std::string& keyword);

#endif //LIBRARY_H