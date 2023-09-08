#include "animation.h"
#include "displaymanager.h"
#include <malloc.h>
#include <Wire.h>

Animation::Frame *Animation::frames = nullptr;
int Animation::frameCount = 0;
int Animation::period = 0;
int Animation::tick = 0;
int Animation::currentFrame = 0;
char Animation::frameText[Animation::frameTextSize] = {0};

void Animation::init() {
    if(frameCount) {
        free(frames);
    }
    frameCount = 0;
    frames = nullptr;
    period = 0;
    tick = 0;
    currentFrame = 0;
}

int Animation::addFrame(int duration, void(*getText)(char *text, int size, int tick)) {
    frames = (Frame*)realloc(frames, ++frameCount * sizeof(Frame));
    frames[frameCount - 1].duration = duration;
    frames[frameCount - 1].create = getText;
    frames[frameCount - 1].remainingDuration = duration;
    period += duration;
    return frameCount - 1;
}

void Animation::repeat(int frame, int times) {
    for(int i = 0; i < times; i++) {
        addFrame(frames[frame].duration, frames[frame].create);
    }
}

void Animation::performTick() {
    Frame *f = frames + currentFrame;
    if(f->remainingDuration == 0) {
        f->remainingDuration = f->duration;
        currentFrame = (currentFrame + 1) % frameCount;
        f = frames + currentFrame;
    }
    f->create(frameText, frameTextSize, tick);
    DisplayManager::clear();
    DisplayManager::print(frameText);
    f->remainingDuration--;
    tick = (tick + 1) % period;
}