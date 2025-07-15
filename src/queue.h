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

struct MusicNode
{
    Music& music;
    MusicNode* next;
    explicit MusicNode(Music& music) : music(music), next(nullptr) {}
    explicit MusicNode(std::vector<Music>::const_reference value);
};

class Queue
{
    MusicNode * head = nullptr, * tail = nullptr, * newNode = nullptr, * bantu = nullptr;
    void awal();
    bool isEmpty() const;
    ~Queue() {
        clearPlaylist();
    }
public:
    int countList();
    void addMusic(const uint64_t pos, const std::vector<Music>& Library);
    void playlistIterate();
    void playNext(bool skip = false);
    void removeMusic(int pos);
    void nowPlaying();
    void clearPlaylist();
};
#endif //QUEUE_H
