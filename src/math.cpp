#include "math.hpp"
#include "player.hpp"
#include <SDL.h>
#include <cmath>


int getQuad(int x, int y){
    if(x >= 0 && y >= 0)
        return I;
    
    if(x < 0 && y >= 0)
        return II;
    
    if( x < 0 && y < 0 )
        return III;

    return IV;

}


double getRotation( int playerX, int playerY){
    
    int x, y;
    pPos camPos = gBoard.getCamPos();
    SDL_GetMouseState(&x,&y);


    double termX = static_cast<double>(x-(playerX-camPos.x));
    double termY = static_cast<double>(y-(playerY-camPos.y));

    if(termX==0)termX=1;

    int quadrant = getQuad(termX, termY);

    double angle;

    switch(quadrant){

        case I:
            angle = atan(termY/termX)*180/M_PI;
            break;

        case II:
            angle = ( M_PI+ atan(termY/termX))*180/M_PI;
            break;
        
        case III:
            angle = ( M_PI+ atan(termY/termX) )*180/M_PI;
            break;
        case IV:
            angle = (2*M_PI + atan(termY/termX) ) * 180/M_PI;
            break;
    }

    return angle;

}


normal normalize(int x, int y){

    double h = sqrt(x*x+y*y);
    return{x/h,y/h};
    
}
