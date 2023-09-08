#pragma once

struct Animation {
    struct Frame
    {
        int duration;
        int remainingDuration;
        void(*create)(char *text, int textSize, int tick);
    };
    
    static Frame *frames;
    static int frameCount;
    static int tick;
    static int period;
    static int currentFrame;
    static const int frameTextSize = 50;
    static char frameText[frameTextSize];

    static void build();
    static int addFrame(int duration, 
        void(*)(char *text, int textSize, int tick));
    static void repeat(int frameID, int times = 1);
    static void performTick();
};