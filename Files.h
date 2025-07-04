//
// Created by rei on 7/1/25.
//

#ifndef FILES_H
#define FILES_H
#include <string>
#include <vector>


class Files {
    std::string directory;
public:
    std::vector<std::string> findMp3(std::string directory);
};



#endif //FILES_H
