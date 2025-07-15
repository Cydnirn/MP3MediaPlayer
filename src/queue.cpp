#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include "queue.h"

void Queue::awal()
{
    head = nullptr;
} 

bool Queue::isEmpty() const
{
    return head == nullptr;
}

int Queue::countList() {
    bantu = head;
    int jumlah = 0;
    while (bantu != nullptr) {
        jumlah++;
        bantu = bantu->next;
    }
    return jumlah;
}

void Queue::addMusic(const uint64_t pos, const std::vector<Music>& Library)
{
    if (pos == 0 || pos > Library.size()) {
        std::cout << "Invalid position! Please enter a position between 1 and " << Library.size() << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
        std::cin.get();
        return;
    }
    newNode = new MusicNode(Library.at(pos - 1));
    if (isEmpty()) {
        head=tail=newNode;
    } else {
        tail->next = newNode;
        tail = newNode;      
    }
}

void Queue::playlistIterate()
{
    std::cout << "===================== Playlist ====================\n";
    if (isEmpty()) {
        std::cout << "Playlist kosong!\n";
        return;
    }
    bantu = head;
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
    bantu = head;
    std::cout << "Selesai memutar : " << bantu->music.title << " - "<< bantu->music.artist <<std::endl;

    head = head->next;
    delete bantu;

    if (head == nullptr) {
        tail = nullptr;
    }
}


void Queue::removeMusic(int pos) {
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
    std::cout << "Menghapus : " << del->music.title << " - " << del->music.artist << std::endl;
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
  std::cout<< "Menghapus : " << del->music.title << " - " << del->music.artist << std::endl;

  prev->next = del->next;

  if (del == tail) {
    tail = prev;
  }

  delete del;
}

void Queue::nowPlaying() {
    if (head == nullptr) {
        std::cout << "\ntidak ada lagi musik di playlist.\n";
    } else {
        std::cout << "\nMemutarkan : " << head->music.title << " - " << head->music.artist << std::endl;
    }
}

void Queue::clearPlaylist() {
    while(!isEmpty()) {
        playNext();
    }
}