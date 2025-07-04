//
// Created by rei on 7/1/25.
//

#ifndef FILES_H
#define FILES_H
#include <filesystem>
#include <string>
#include <vector>

#include "library.h"


class Files {
    std::string directory;
    std::vector<std::filesystem::path> dirEntry;
public:
    Files();
    std::vector<std::string> findMp3(const std::string& directory);
    std::vector<musicLib> getMusic(const std::vector<std::string> &mp3Entry);
};

inline Files files;

#endif //FILES_H
