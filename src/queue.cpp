#include <iostream>
#include <vector>
#include <string>
#include "queue.h"

bool Queue::isEmpty() const
{
    return head == nullptr;
}

int Queue::countList() const
{
    auto bantu = head;
    int jumlah = 0;
    while (bantu != nullptr) {
        jumlah++;
        bantu = bantu->next;
    }
    return jumlah;
}

void Queue::addMusic(const Music& music)
{
    MusicNode *newNode = new MusicNode{music, nullptr};
    if (isEmpty()) {
        head=tail=newNode;
    } else {
        tail->next = newNode;
        tail = newNode;      
    }
}

void Queue::playlistIterate() const
{
    MusicNode *bantu = head;
    int index = 1;
    while (bantu != nullptr) {
        std::cout << index++ << "." << bantu->music.title << " - " << bantu->music.artist << std::endl;
        bantu = bantu->next;
    }
}

void Queue::playNext(bool skip) {
    if (isEmpty()) {
        std::cout << "Playlist kosong!" << std::endl;
        return;
    }
    MusicNode *bantu = head;

    head = head->next;
    delete bantu;

    if (head == nullptr) {
        tail = nullptr;
    }
}


void Queue::removeMusic(const int pos) {
  if(isEmpty()) {
    std::cout << "Playlist kosong!\n";
  }

  int total = countList();
  if (pos < 1 || pos > total) {
    std::cout<< "index tidak valid! " << total << std::endl;
    return;
  }

  MusicNode *del;

  if (pos == 1) {
    del = head;
    head = head->next;
    delete del;

    if (head == nullptr) {
        tail = nullptr;
    }

    return;
  }

  MusicNode* prev = head;
  for(int i = 1; i < pos - 1; i++) {
    prev = prev->next;
  }

  del = prev->next;
  prev->next = del->next;

  if (del == tail) {
    tail = prev;
  }

  delete del;
}

Queue::MusicNode* Queue::currentMusic() const
{
    return head;
}

void Queue::clearPlaylist() {
    while(!isEmpty()) {
        playNext();
    }
}

void Queue::addMusicAtPos(const int pos, const std::vector<Music>& Library,int libIndex) {
    if (isEmpty()) {
        std::cout << " Playlist Kosong!" << std::endl;
        return;
    }

    if (int total = countList(); pos < 1 || pos > total) {
        std::cout << "index tidak valid! Total Musik: " << total << std::endl;
        return;
    }

    if (libIndex < 1 || libIndex > Library.size()) {
        std::cout << "index tidak valid! Harus antara 1 dan" << Library.size() << std::endl;
        return;
    }

    const auto it = Library.at(libIndex - 1);
    // Create a new node with the music reference
    auto newNode =  new MusicNode(it);
    // Handle insertion at the beginning
    if (pos == 1) {
        newNode->next = head;
        head = newNode;
        return;
    }

    // Find the node before the insertion point
    auto bantu = head;
    for (int i = 1; i < pos - 1; i++) {
        bantu = bantu->next;
    }

    // Insert the new node,
    // reference the prev next node to the current node
    newNode->next = bantu->next;
    bantu->next = newNode;

    // Update tail if we're inserting at the end
    if (newNode->next == nullptr) {
        tail = newNode;
    }
}