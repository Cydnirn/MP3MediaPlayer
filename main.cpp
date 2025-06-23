#include <iostream>
#include <memory>
#include "PlayMP3.h"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        auto p = std::make_unique<MP3MediaPlayer::PlayMP3>();
        p->music(argv[1]);
        p->play();
    } else {
        std::cerr << "Usage: " << argv[0] << " <mp3_file>" << std::endl;
        return 1;
    }
    return 0;
}