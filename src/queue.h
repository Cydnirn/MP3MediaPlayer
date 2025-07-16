//
// Created by rei on 7/8/25.
//

#ifndef QUEUE_H
#define QUEUE_H
#include "library.h"
#include "PlaylistManager/MusicObserver.h"

enum playStatus {
    notPlaying,
    playing,
    paused
};


class Queue
{
    struct MusicNode
    {
        Music music;
        MusicNode* next;
    };
    MusicNode * head = nullptr, * tail = nullptr;
    std::vector<MusicObserver*> observers;
public:
    Queue()
    {
        head = nullptr;
    }
    ~Queue() {
        clearPlaylist();
    }
    void addObserver(MusicObserver *observer);
    void removeObserver(MusicObserver *observer);
    void notifyObservers() const;
    bool isEmpty() const;
    int countList() const;
    void addMusic(const Music& music);
    void playlistIterate() const;
    void playNext();
    void removeMusic(int pos);
    MusicNode* currentMusic() const;
    void clearPlaylist();
    void addMusicAtPos(int pos, const std::vector<Music>& Library,int libIndex);
};
#endif //QUEUE_H
