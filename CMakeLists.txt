cmake_minimum_required(VERSION 3.31)
project(MP3MediaPlayer)

set(CMAKE_CXX_STANDARD 20)

find_package(mpg123 CONFIG REQUIRED)
find_package(portaudio CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(taglib CONFIG REQUIRED)

add_executable(MP3MediaPlayer src/main.cpp
        src/PlayMP3.cpp
        src/PlayMP3.h
        src/Files.cpp
        src/Files.h
        src/library.h
        src/library.cpp
)

target_link_libraries(MP3MediaPlayer PRIVATE MPG123::libmpg123 MPG123::libout123 MPG123::libsyn123)
target_link_libraries(MP3MediaPlayer PRIVATE portaudio_static)
target_link_libraries(MP3MediaPlayer PRIVATE TagLib::tag TagLib::tag_c TagLib::TagLib)