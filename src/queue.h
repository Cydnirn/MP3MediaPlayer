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
    struct MusicNode
    {
        const Music* music;
        MusicNode* next;
        explicit MusicNode(const Music* music) : music(music), next(nullptr) {}
    };
    MusicNode * head = nullptr, * tail = nullptr, * newNode = nullptr, * bantu = nullptr;
public:
    Queue()
    {
        head = nullptr;
    }
    ~Queue() {
        clearPlaylist();
    }
    bool isEmpty() const;
    int countList();
    void addMusic(uint64_t pos, const std::vector<Music>& Library);
    void playlistIterate();
    void playNext(bool skip = false);
    void removeMusic(int pos);
    MusicNode* currentMusic() const;
    void clearPlaylist();
    void updateMusic(int pos, const std::vector<Music>& Library,int libIndex);
};
#endif //QUEUE_H
