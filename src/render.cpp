#include "render.hpp"
#include "title.hpp"


void renderStuff( player *a ){

    SDL_RenderClear( gRenderer );

    if( !a->gameStarted() ){
        gTitleScreen.renderTitleScreen();

    }
    else{
        gBoard.renderClip();
        a->renderOtherPlayers();
        a->renderPlayer();
        a->checkRenderPause();
    }
    SDL_SetRenderDrawColor(gRenderer, 200,200,200,20);
    SDL_RenderPresent( gRenderer );
}
