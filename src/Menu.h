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
#include "MP3MediaPlayer/PlayMP3.h"
#include "PlaylistManager/MusicObserver.h"


class Menu final : public MusicObserver {
    bool inNowPlaying = false;
    const std::unique_ptr<Queue>& playlist;
    const std::unique_ptr<Library>& lib;
    const std::unique_ptr<MP3MediaPlayer::PlayMP3>& player;
public:
    Menu(const std::unique_ptr<Queue>& queue,
         const std::unique_ptr<Library>& lib,
         const std::unique_ptr<MP3MediaPlayer::PlayMP3>& player)
        : playlist(queue), lib(lib), player(player){}
    ~Menu() override = default;
    static void clearScreen()
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
    void displayMainMenu();
    static void displayMusic(const Music& music);
    void displayMusicList(const std::vector<Music>& musicList);
    static void displayMusicMenu(const bool& sortYear, const bool& sortName);
    void displayMusicSearch(const std::string& keyword) const;
    void displayPlaylist();
    void displayPlaylistMenu();
    void displayCurrentPlaying();
    void displayPlaybackControl() const;
    void onMusicChanged() override;
};
#endif //MENU_H
