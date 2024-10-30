#pragma once

struct normal{
    double x,y;
};

enum quad{
    I,
    II,
    III,
    IV
};

int getQuad(int x, int y);
double getRotation( int playerX, int playerY);
normal normalize(int x, int y);

//useful for checking bonking and collisions
template<typename T> bool squaredDistCollision( const T &x1 , const T &y1, const T &rad1, const T &x2, const T &y2, const T &rad2 ){
    T x = x1-x2;
    T y = y1-y2;
    if( x*x + y*y < (rad1+rad2)*(rad1+rad2) )
        return true;
    
    return false;

}

