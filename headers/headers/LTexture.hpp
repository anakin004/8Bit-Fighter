#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <SDL_ttf.h>
#include <SDL_FontCache.h>

class LTexture {
    public:

        LTexture();
        ~LTexture();
        bool loadFromFile(std::string path);
        void renderText( std::string, int, int );
        void renderTitleText( std::string, int, int, SDL_Color *color = nullptr );
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor);
        void free();
        int getWidth();
        int getHeight();
    
    protected:
        FC_Font *mFont;
        SDL_Texture* mTexture;
        int mFontSize;
        int mWidth;
        int mHeight;
};

extern SDL_Window *gWindow;
extern SDL_Renderer *gRenderer;



