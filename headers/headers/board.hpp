#pragma once

#include "LTexture.hpp"
#include <string>
#include <SDL_image.h>
#include <SDL.h>


struct pPos
{
    int x, y;
};

class player;


class board : public LTexture {

    public:
        board();

        // render using camx and camy
        void renderClip();
        void updateBoardPos( player* a );
        bool changeBoard( std::string path);
        pPos getCamPos();
        

    private:

        int mCamX;
        int mCamY;


};

extern SDL_Renderer *gRenderer;
extern SDL_Window *gWindow;
extern board gBoard;

extern const int WIDTH;
extern const int HEIGHT;