#include "gamestate.hpp"
#include "player.hpp"
#include "init.hpp"
#include "board.hpp"
#include "render.hpp"
#include "logic.hpp"
#include "macros.hpp"
#include "weapon.hpp"
#include "weapon.hpp"
#include "network.hpp"
#include "title.hpp"

//there will be one instance of the board and title screen
board gBoard;
titleScreen gTitleScreen;

Game::Game(){
    mRunning = true;
}

void Game::start(){

    if(!init()) 
        return;

    player a;
                        // path         , numsprites,      diameter    #weaps
    weapon fire( "sound_effects/sprites/fireAtlas.png", 3,   100,         3 );
    
    if( !gBoard.changeBoard("sound_effects/sprites/board.png") ||
        !a.initPlayer("sound_effects/sprites/playerAtlas.png", &fire) || 
        !gTitleScreen.loadFromFile( "sound_effects/sprites/playerAtlas.png" )
    ){
        printf("\ncouldnt init player and board");
        return;
    }

    SDL_Event e;
    bool quit = false;

    while(quit == false){

        Uint32 FrameStart = SDL_GetTicks();
        if((a.getInput(e))==false){
            quit = true;
            mRunning = false;
        }

        updateLogic( &a );
        renderStuff( &a );

        //144 is the frame cap
        if(SDL_GetTicks()-FrameStart < 1000/144.f)
            SDL_Delay((1000/144.f-(SDL_GetTicks()-FrameStart)));
        
    } 
}

Game::~Game(){
    free();
}

// players and textures have default  destructors
void Game::free(){

    SDL_DestroyRenderer( gRenderer );
    gRenderer = nullptr;

    SDL_DestroyWindow( gWindow );
    gWindow = nullptr;

    SDL_Quit();
    SDLNet_Quit();
}
