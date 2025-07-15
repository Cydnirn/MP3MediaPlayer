//
// Created by rei on 7/4/25.
//

#ifndef LIBRARY_H
#define LIBRARY_H
#include <mpg123.h>

struct Music {
    std::string title, artist;
    uint64_t year;
    std::string path;
};

class Library
{
    std::vector<Music> musicList;
public:
    Library(){};
    ~Library()
    {
        musicList.clear();
    };
    std::vector<Music> getMusicList() const;
    // Setter for musicList
    void setMusicList(const std::vector<Music>& list);
    //bool compareMusicByTitle(const Music &a, const Music &b);
    void sortByYearDes();
    void sortByYearAsc();
    void sortByHurufAsc();
    void sortByHurufDes();
    std::vector<Music> searchTitle(const std::string& keyword) const;
};
#endif //LIBRARY_H