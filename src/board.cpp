#include "board.hpp"
#include "player.hpp"
#include "macros.hpp"

board::board() {
    mCamX = 0;
    mCamY = 0;
}

bool board::changeBoard( std::string path){
    if( !gBoard.loadFromFile(path)){
        printf("couldnt lod from file path");
        return false;
    }
    return true;
}

pPos board::getCamPos(){
    return {mCamX, mCamY};
}




void board::updateBoardPos( player *a ){

    pPos playerPos = a->getPlayerPos();

    mCamX = playerPos.x-WIDTH/2;
    mCamY = playerPos.y-HEIGHT/2;

    if( mCamX < 0 )
        mCamX = 0;

    else if( mCamX+WIDTH > gBoard.getWidth() )
        mCamX = gBoard.getWidth()-WIDTH;

    if(mCamY < 0 )
        mCamY = 0;
    
    else if( mCamY+HEIGHT > gBoard.getHeight() )
        mCamY = gBoard.getHeight()-HEIGHT;


}

void board::renderClip() {

    SDL_Rect clip = {mCamX, mCamY, WIDTH, HEIGHT};

    SDL_RenderCopy( gRenderer, mTexture , &clip , NULL );

}