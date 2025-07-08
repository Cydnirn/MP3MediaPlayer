//
// Created by rei on 7/1/25.
//

#include "Files.h"

#include <filesystem>
#include <taglib/fileref.h>
#include <taglib/tag.h>

Files::Files()
{
    // Get the user's home directory
    const char* homeDir = nullptr;
    printf("Setting up music directory...\n");
    #ifdef _WIN32
    // Windows
    homeDir = std::getenv("USERPROFILE");
    if (homeDir) {
        directory = std::string(homeDir) + "\\Music";
    } else {
        directory = "C:\\Music"; // Fallback
    }
    #else
    // Linux
    homeDir = std::getenv("HOME");
    if (homeDir) {
        directory = std::string(homeDir) + "/Music";
    } else {
        directory = "/home/Music"; // Fallback
    }
    #endif
    printf("Directory set to: %s\n", directory.c_str());
}


std::vector<std::string> Files::findMp3(const std::string& dir = "")
{   const std::string useDir = dir.empty() ? directory : dir;
    std::vector<std::string> mp3Entry;
    for (const auto & entry : std::filesystem::recursive_directory_iterator(useDir)) {
        dirEntry.emplace_back(entry.path().generic_u8string());
    }
    int idx = 0;
    for (auto & i : dirEntry) {
        std::filesystem::path tmp = dirEntry[idx];
        auto utf8 = tmp.u8string();
        auto str = std::string(reinterpret_cast<char const*>(utf8.data()), utf8.size());
        if (tmp.extension() == ".mp3") {
            mp3Entry.emplace_back(str);
        }
        idx++;
    }
    return mp3Entry;
}

std::vector<musicLib> Files::getMusic(const std::vector<std::string> &mp3Entry)
{
    std::vector<musicLib> musicList;
    for (const auto & mp3 : mp3Entry) {
        if (TagLib::FileRef file(mp3.c_str()); !file.isNull() && file.tag())
        {
            TagLib::Tag *tag = file.tag();
            musicLib music;
            music.title = tag->title().toCString(true);
            music.artist = tag->artist().toCString(true);
            music.year = tag->year();
            music.path = mp3;
            musicList.push_back(music);
        }
    }
    return musicList;
}
