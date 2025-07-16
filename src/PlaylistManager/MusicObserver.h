//
// Created by rei on 7/16/25.
//

#ifndef MUSICOBSERVER_H
#define MUSICOBSERVER_H
#pragma once

class MusicObserver {
public:
    virtual ~MusicObserver() = default;
    virtual void onMusicChanged() = 0;
};
#endif //MUSICOBSERVER_H
