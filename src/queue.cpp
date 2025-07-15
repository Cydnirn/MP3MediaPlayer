#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include "queue.h"

Music *head, *tail, *newNode, *bantu;

void awal() 
{
    head=NULL;
} 

bool isEmpty() 
{
    if (head == NULL) {
        return true;
    } else {
        return false;
    }
}

int countList() {
    bantu = head;
    int jumlah = 0;
    while (bantu != NULL) {
        jumlah++;
        bantu = bantu->next;
    }
    return jumlah;
}

void addMusic(const uint64_t pos, const std::vector<musicLib>& Library)
{
    if (pos == 0 || pos > Library.size()) {
        std::cout << "Invalid position! Please enter a position between 1 and " << Library.size() << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
        std::cin.get();
        return;
    }
    newNode = new Music();
    newNode->title = Library[pos-1].title;
    newNode->artist = Library[pos-1].artist;
    newNode->year = Library[pos-1].year;
    newNode->path = Library[pos-1].path;
    newNode->next = NULL;
    if (isEmpty()) {
        head=tail=newNode;
    } else {
        tail->next = newNode;
        tail = newNode;      
    }
    std::cout << "Ditambahkan ke playlist " << newNode->title << " - " << newNode->artist << "\n\n";
}

void printPlaylist() 
{
        std::cout << "===================== Playlist ====================\n";
    if (isEmpty()) {
        std::cout << "Playlist kosong!\n";
        return;
    }
    bantu = head;
    int index = 1;

    while (bantu != NULL) {
        std::cout << index++ << "." << bantu->title << " - " << bantu->artist << std::endl;
        bantu = bantu->next;
    }
}

void playNext() {
    if (isEmpty()) {
        std::cout << "Playlist kosong!" << std::endl;
        return;
    }

    bantu = head;
    std::cout << "Selesai memutar : " << bantu->title << " - "<< bantu->artist <<std::endl;

    head = head->next;
    delete bantu;

    if (head == NULL) {
        tail = NULL;
    }
}


void removeMusic(int pos) {
  if(isEmpty()) {
    std::cout << "Playlist kosong!\n";
  }

  int total = countList();
  if (pos < 1 || pos > total) {
    std::cout<< "index tidak valid! " << total << std::endl;
    return;
  }

  Music *del;

  if (pos == 1) {
    del = head;
    std::cout << "Menghapus : " << del->title << " - " << del->artist << std::endl;
    head = head->next;
    delete del;

    if (head == NULL) {
        tail = NULL;
    }

    return;
  }

  Music* prev = head;
  for(int i = 1; i < pos - 1; i++) {
    prev = prev->next;
  }

  del = prev->next;
  std::cout<< "Menghapus : " << del->title << " - " << del->artist << std::endl;

  prev->next = del->next;

  if (del == tail) {
    tail = prev;
  }

  delete del;
}

void nowPlaying() {
    if (head == NULL) {
        std::cout << "\ntidak ada lagi musik di playlist.\n";
    } else {
        std::cout << "\nMemutarkan : " << head->title << " - " << head->artist << std::endl;
    }
}

void clearPlaylist() {
    while(!isEmpty()) {
        playNext();
    }
}

void moveLeft(int pos) {
    if ((isEmpty()) || pos <= 1 || pos > countList()) {
    std::cout << "Posisi Tidak Valid untuk dipindahkan ke kiri!\n";
        return;
        }
    Music *prev = nullptr, *curr = head, *beforePrev = nullptr;

    for (int i = 1; i < pos; i++) {
        beforePrev = prev;
        prev = curr;
        curr = curr->next;
    }

    if (beforePrev == nullptr) {
        return;
    }

    prev->next = curr->next;
    curr->next = prev;

    if (beforePrev != nullptr) {
        beforePrev->next = curr;
    } else {
        head = curr;
    }

    if (prev->next != nullptr) {
        tail = prev;
    }

    std::cout << "Memindahkan ke Kiri: " << curr->title << " - " << curr->artist << std::endl;
}

void moveRight(int pos) {
    int total = countList();
    if (isEmpty() || pos < 1 || pos >= total) {
        std::cout << "Posisi tidak valid untuk dipindahkan ke kanan!\n";
        return;
    }

    Music *prev = nullptr, *curr = head, *next = nullptr;

    for (int i = 1; i < pos; i++) {
        prev = curr;
        curr = curr->next;
    }

    next = curr->next;

    if (next == nullptr) {
        return;
    }

    curr->next = next->next;
    next->next = curr;

    if (prev != nullptr) {
        prev->next = next;
    } else {
        head = next;
    }

    if (curr->next == nullptr) {
        tail = prev;
    }

    std::cout << "Memindah ke kanan: " << next->title << " - " << next->artist << std::endl;
}
