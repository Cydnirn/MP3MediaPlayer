cmake_minimum_required(VERSION 3.31)
project(MP3MediaPlayer)

set(CMAKE_CXX_STANDARD 20)

find_package(mpg123 CONFIG REQUIRED)
find_package(portaudio CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(taglib CONFIG REQUIRED)

add_executable(MP3MediaPlayer src/main.cpp
        src/MP3MediaPlayer/PlayMP3.cpp
        src/MP3MediaPlayer/PlayMP3.h
        src/PlaylistManager/PlaylistManager.cpp
        src/PlaylistManager/PlaylistManager.h
        src/PlaylistManager/MusicObserver.h
        src/Files.cpp
        src/Files.h
        src/library.h
        src/library.cpp
        src/queue.h
        src/queue.cpp
        src/Menu.cpp
        src/Menu.h

)

target_link_libraries(MP3MediaPlayer PRIVATE MPG123::libmpg123 MPG123::libout123 MPG123::libsyn123)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(MP3MediaPlayer PRIVATE portaudio_static)
else()
    target_link_libraries(MP3MediaPlayer PRIVATE portaudio)
endif()
target_link_libraries(MP3MediaPlayer PRIVATE TagLib::tag TagLib::tag_c TagLib::TagLib)