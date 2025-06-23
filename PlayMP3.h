//
// Created by rei on 6/23/25.
//

#ifndef MP3MEDIAPLAYER_PLAYMP3_H
#define MP3MEDIAPLAYER_PLAYMP3_H
#include <mpg123.h>
#include <portaudio.h>
#include <memory>
#include <iostream>
#include <vector>

namespace MP3MediaPlayer {
//
// Created by rei on 6/23/25.
//
    class PlayMP3 {
        std::size_t buffer_size, done;
        int driver, err{}, channels{}, encoding{};
        long rate{};
        std::string track;

        mpg123_handle *mh;
        std::shared_ptr<char> buffer;



        PaStreamParameters param{};
        PaStream *stream{};

    public:
        PlayMP3();

        ~PlayMP3();

        void music(char *);

        void play();
    };
}

#endif //MP3MEDIAPLAYER_PLAYMP3_H
