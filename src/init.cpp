#include "init.hpp"
#include "macros.hpp"
#include "title.hpp"

// so board.cpp can have vision of the screen width and height during linking
#include "board.hpp"

// Global variables
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
TTF_Font* gFont = nullptr;

const int WIDTH = 1000;
const int HEIGHT = 800;



// initalizing window, renderer, sdl_img, mixer, etc
bool init() {
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO ) < 0 || SDLNet_Init() < 0) {
        SDL_Log("Failed to initialize SDL_net: %s", SDLNet_GetError());
        success = false;
        DEBUG_PRINT("Couldn't initialize video");
    } else {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0")) {
            DEBUG_PRINT("Warning: Linear texture filtering not enabled!");
        }
        gWindow = SDL_CreateWindow("FighterV2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
                                   SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            success = false;
            DEBUG_PRINT("Could not create window %s", SDL_GetError());
        } else {
            if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG  ) {
                success = false;
                DEBUG_PRINT(" img_init coudlnt init ");
            } else {
                gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
                SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
                if (gRenderer == NULL) {
                    DEBUG_PRINT("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                    success = false;
                } else{
                    
                    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                    {
                        DEBUG_PRINT( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                        success = false;
                    }
    

                }
            }
        }
    }
    return success;
}
