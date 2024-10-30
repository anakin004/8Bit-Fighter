#pragma once

#include "board.hpp"
#include "LTexture.hpp"
#include "weapon.hpp"
#include "network.hpp"
#include <SDL.h>
#include <array>

// first 3 params regard the state, other two have info about player and is player shot
struct playerState{
    int x;
    int y;
    double rotation;
    int pID;
    bool shot;
    short idx;
    bool resetting;
};

class player : public LTexture{

    public:
        //sets default values for player
        player();
        // for closing connections
        ~player();

        bool initPlayer( std::string path , weapon* thisWeapon );
        void closeConnection();
        void checkRenderPause();
        bool checkQuit();

        //@returns false if we quit true if we are not 
        bool handleEvent( SDL_Event e );
        bool getInput( SDL_Event e);

        bool gameStarted();
        pPos getPlayerPos();
        weapon* getWeapon();
        void selectSprite();

        bool regProjectile();
        void regOtherPlayerProjectile( int );
        void move();

        void updatePlayer();
        void updateOtherPlayers();
        void renderPlayer();
        void renderOtherPlayers();
        void renderStats( int, int);

        void scanGameStart();
        void readyUp();
        void recieveGameStartTCP(  SDL_mutex *mutex );
        void renderWait( SDL_mutex *mutex );

    private:


        std::array<playerState,4> mOtherPlayers;

        responseData mConnection;
        IPaddress dst_ip;

        void sendPlayerPack( bool shot );
        void freeUnnecesary();
        void respawn();


        weapon *mCurWeapon;


        // player velocity rate 
        int mpVel;
        int myVel;
        int mxVel;

        bool mShoot;
        bool mGameStarted;
        bool mJoined;
        bool mPaused;
        
        double mRot;

        // pos on the board
        int mxPos;
        int myPos;

        short mSpriteIndex;

};