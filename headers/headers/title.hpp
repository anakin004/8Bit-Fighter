#pragma once

#include "LTexture.hpp"
#include <SDL.h>
#include <string>

class titleScreen : public LTexture {

    public: 
        // the title screen will dsplay playerModels you can pick from
        titleScreen();

        // a udp package will be send when the game starts
        // this disable title screen rendering and free its resources
        void renderTitleScreen();
        void setReady();    
        // we can have a part that lets you select a certain character 

    private:
        bool mReady;

        // for text blinking
        Uint32 mTime;
        int mBlink;
        bool mBlinking;

};


extern SDL_Renderer *gRenderer;
extern SDL_Window *gWindow;

extern const int WIDTH;
extern const int HEIGHT;

extern titleScreen gTitleScreen;