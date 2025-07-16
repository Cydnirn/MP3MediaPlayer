//
// Created by rei on 7/4/25.
//

#ifndef LIBRARY_H
#define LIBRARY_H
#include <mpg123.h>
#include <vector>

struct Music {
    std::string title, artist;
    uint64_t year;
    std::string path;
};

class Library
{
    std::vector<Music> musicList;
    void sortByYearDes();
    void sortByYearAsc();
    void sortByHurufAsc();
    void sortByHurufDes();
public:
    Library()= default;
    ~Library()
    {
        musicList.clear();
    };
    std::vector<Music> getMusicList() const;
    // Setter for musicList
    void setMusicList(const std::vector<Music>& list);
    void sortYear(bool des);
    void sortHuruf(bool des);
    //bool compareMusicByTitle(const Music &a, const Music &b);
    std::vector<Music> searchTitle(const std::string& keyword) const;
};
#endif //LIBRARY_H