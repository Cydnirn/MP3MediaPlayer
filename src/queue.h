//
// Created by rei on 7/8/25.
//

#ifndef QUEUE_H
#define QUEUE_H
#include "library.h"

enum playStatus {
    notPlaying,
    playing,
    paused
};

struct Music
{
    std::string title, artist, path;
    uint year;
    Music* next;
};

void awal();
bool isEmpty();
int countList();
void addMusic(const uint pos, const std::vector<musicLib>& Library);
void printPlaylist();
void playNext();
void removeMusic(int pos);
void nowPlaying();
void clearPlaylist();
#endif //QUEUE_H
