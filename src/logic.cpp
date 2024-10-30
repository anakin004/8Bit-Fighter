#include "player.hpp"
#include "board.hpp"
#include "macros.hpp"
#include "title.hpp"


void updateLogic( player *a )
{   
    if( a->gameStarted() ){
        a->updatePlayer();
        a->updateOtherPlayers();
        gBoard.updateBoardPos( a );
    }
    else{
        a->scanGameStart();
    }
}