#include <iostream>
#include <queue>
#include <vector>
#include <string>


enum playStatus {
    notPlaying,
    playing,
    paused
};

struct Music 
{
    std::string title, artist;
    Music* next;
};

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

void addMusic(std::string title, std::string artist) 
{
    newNode = new Music();
    newNode->title = title;
    newNode->artist = artist;
    newNode->next = NULL;
    if (isEmpty()) {
        head=tail=newNode;
    } else {
        tail->next = newNode;
        tail = newNode;      
    }
    std::cout << "Ditambahkan ke playlist " << title << " - " << artist;
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

void printMusicList() 
{
    if (isEmpty()) {
        std::cout << "Playlist kosong!\n";
        return;
    }

    std::cout << "\n Sedang memutar :\n";
    bantu = head;
    int index = 1;

    while (bantu != NULL) {
        std::cout << index++ << ". " << bantu->title << " - " << bantu->artist << std::endl;
        bantu = bantu->next;
    }
    std::cout << std::endl;
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