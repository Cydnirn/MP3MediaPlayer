//
// Created by rei on 7/15/25.
//

#ifndef MENU_H
#define MENU_H
#include <memory>
#include <string>
#include <vector>

#include "library.h"
#include "queue.h"


class Menu {
public:
    Menu(){};
    ~Menu() = default;
    static void displayMainMenu();
    static void displayMusic(const Music& music);
    static void displayMusicList(const std::vector<Music>& musicList);
    static void displayMusicSearch(const std::unique_ptr<Library>& lib, const std::string& keyword);
    static void displayPlaylist(std::unique_ptr<Queue> queue);
    static void displayCurrentPlaying(std::unique_ptr<Queue> queue);
};
#endif //MENU_H
