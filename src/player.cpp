#include "player.hpp"
#include "board.hpp"
#include "macros.hpp"
#include "math.hpp"
#include "network.hpp"
#include "title.hpp"
#include <cmath>
#include <string>
#include <cstring>
#include <random>
#include <thread>



player::player() : mRot(0), mxVel(0), myVel(0), mpVel(1), 
                   mCurWeapon(nullptr),
                   mShoot(false), mPaused(false), mJoined(false), dst_ip{0,0}, mGameStarted(false), mSpriteIndex(0),mxPos(), myPos(),
                   mOtherPlayers {{
                        {0,0,0,-1,false},
                        {0,0,0,-1,false},
                        {0,0,0,-1,false},
                        {0,0,0,-1,false},
                    }},
                    mConnection {-1,nullptr,nullptr}
                    {
			    
                        if (SDLNet_ResolveHost( &dst_ip, HOST , DST_TCP_PORT ) < 0) {
                            SDL_Log("Failed to resolve host: %s", SDLNet_GetError());
                            // Handle the error, e.g., throw an exception or set an error state
                        }

                    }


                    

void player::move(){
    mxPos += mxVel;
    myPos += myVel;

    int xOff = getWidth()/6;
    int yOff = getHeight()/2;

    if( mxPos - xOff < 0 )
        mxPos = xOff;

    else if( mxPos + xOff > gBoard.getWidth() )
        mxPos = gBoard.getWidth()- xOff;

    if(myPos - yOff < 0 )
        myPos = yOff;
    
    else if( myPos + yOff > gBoard.getHeight() )
        myPos = gBoard.getHeight() - yOff;

}

void player::respawn(){
                            // sudo random respawn pos in the bounds
    mxPos = ( gBoard.getWidth()  * SDL_GetTicks() )% (gBoard.getWidth()-100) + 50;
    myPos = ( gBoard.getWidth()  * SDL_GetTicks() )% (gBoard.getHeight()-100) + 50;
}



weapon* player::getWeapon(){
    return mCurWeapon;
}



// send signal to server that you have joined the game, after that it will display stuff like num readied up etc

void player::readyUp(){

    // if we have already connected we dont want to send another TCP request when joining
    if( mJoined )
        return;

     // we arent truly connecting to server, UDP socket is returned to see if we can successfully establish a socket
    // but connectToServer is meant to sent the initial connection and then we recieve a response on first contact 
    //                                 change to ipv4 of main computa
    connectToServer( HOST, mConnection );


    if (mConnection.pID > -1 && mConnection.udpSocket && mConnection.tcpSocket ) 
        mJoined = true;
    
    else
        SDL_Log("Failed to Connect");
    
    

}


bool player::initPlayer( std::string path, weapon* thisWeap ){


    if(!loadFromFile(path)){
        printf("loading from path didnt work");
        return false;
    }
    if(thisWeap==nullptr){
        printf("weap is null");
        return false;
    }

    mxPos = ( gBoard.getWidth()  * SDL_GetTicks() )% (gBoard.getWidth()-100) + 50;
    myPos = ( gBoard.getWidth()  * SDL_GetTicks() + 7*( mConnection.pID * dst_ip.port * mpVel ))% (gBoard.getHeight()-100) + 50;

    mCurWeapon = thisWeap;

    return true;
}


bool player::getInput( SDL_Event e){

        while(SDL_PollEvent(&e) == 1)
        {
            if(e.type == SDL_QUIT){
                return false;
            }
            
            if(!handleEvent(e))
                return false;
        }

        return true;


    }

void player::recieveGameStartTCP(  SDL_mutex *mutex ){
    
    bool recv;
        // Attempt to receive a tcp packet on mConnection.udpSocket 
    if (SDLNet_TCP_Recv( mConnection.tcpSocket , &recv , sizeof(bool)) > 0) {
        // Successfully received a packet, now interpret the data as a boolean
            SDL_Log("Game is starting!\n");
              
            SDL_LockMutex(mutex);
            mGameStarted = true;  // Mark that the game has started
            SDL_UnlockMutex(mutex);
            
            // when we are clicking on main menu this turns to true just set it to false, easy short hand
            mShoot = false;

            //freeing some stuff like title screen
            freeUnnecesary();
    } 
}

// title screen wiating after join
void player::renderWait( SDL_mutex *mutex ){

    const char* wait = "joining...";

    Uint32 prevTime = SDL_GetTicks();
    int blink = 1000;
    bool blinking = false;

    SDL_Color black = {255,255,255,255};
    
    int w = 0, h = 0;
    w = FC_GetWidth( mFont, wait );
    h = FC_GetWidth( mFont, wait );

    while(true){
        SDL_LockMutex(mutex);
        if(mGameStarted)
            break;
        SDL_UnlockMutex(mutex);

        Uint32 curTime = SDL_GetTicks();
        SDL_RenderClear( gRenderer );
        SDL_SetRenderDrawColor( gRenderer, 50, 50, 50 , 255 );


        if( curTime - prevTime > blink ){
            blinking = !blinking;
            prevTime = curTime;
        }

        if(blinking){
            FC_DrawColor( mFont, gRenderer, WIDTH/2 - w/2, HEIGHT/2, black, wait );
        }
        else{
            const char* wait2 = "joining..";
            FC_DrawColor( mFont, gRenderer, WIDTH/2 - w/2, HEIGHT/2, black, wait2 );            
        }

        SDL_RenderPresent( gRenderer );
    }
}

// preCondition -  we have joined the game and have readied up
void player::scanGameStart() {
    if ( mGameStarted || !mJoined )
        return;

    SDL_mutex *mutex = SDL_CreateMutex();

    //std::thread waitScreen( &player::renderWait, this, mutex );
    std::thread TCPrecv( &player::recieveGameStartTCP, this, mutex );

    //if (waitScreen.joinable()) {
    //    waitScreen.join();  // Wait for the `renderWait` thread to finish
    //}

    if (TCPrecv.joinable()) {
        TCPrecv.join();  // Wait for the `recieveGameStartTCP` thread to finish
    }

}

bool player::gameStarted(){
    return mGameStarted;
}


// only prints if we are paused
void player::checkRenderPause(){

    if(mPaused){
        
        SDL_SetRenderDrawColor(gRenderer , 0, 0, 0, 128);  // 128 is for 50% opacity (half-transparent)

        // Create a rectangle covering the entire screen
        SDL_Rect overlayRect = { 0, 0, WIDTH, HEIGHT };

        // Render the semi-transparent rectangle
        SDL_RenderFillRect( gRenderer , &overlayRect);

        
        const char* text = "quit";
        int w = FC_GetWidth( mFont, text );
        int h = FC_GetHeight( mFont, text);

        int pad = 10;
        SDL_Rect quitRect = {WIDTH/2 - w/2 - pad/2 , HEIGHT/2 -h/2 -pad/2 , w + pad, h + pad };
        SDL_SetRenderDrawColor(gRenderer, 0,0,0,200);
        SDL_RenderDrawRect( gRenderer, &quitRect );

        int mouseX, mouseY;
        SDL_GetMouseState( &mouseX, &mouseY);


        SDL_SetRenderDrawColor( gRenderer, 50, 50, 50, 200 );
        if(mouseX >= WIDTH/2 - w/2 && mouseX <= WIDTH/2 + w/2 ){
            if( mouseY >= HEIGHT/2 - h/2 && mouseY <= HEIGHT/2 + h/2 )
                SDL_RenderFillRect( gRenderer, &quitRect );
        }

        //drawing text ontop of everything
        SDL_Color offwhite {200,200,200,255};
        FC_DrawColor( mFont, gRenderer, WIDTH/2 - w/2, HEIGHT/2 - h/2, offwhite , text );
    }

}

bool player::checkQuit(){

    bool quit = false;

    if( mPaused ){
        int mouseX, mouseY;
        SDL_GetMouseState( &mouseX, &mouseY);


        // this is imported from the render quit screen
        // we need width and height to check mouse positions
        const char* text = "quit";
        int w = FC_GetWidth( mFont, text );
        int h = FC_GetHeight( mFont, text);

        SDL_SetRenderDrawColor( gRenderer, 50, 50, 50, 200 );
        if(mouseX >= WIDTH/2 - w/2 && mouseX <= WIDTH/2 + w/2 ){
            if( mouseY >= HEIGHT/2 - h/2 && mouseY <= HEIGHT/2 + h/2 ){
                quit = true;
            }
        }

    }

    return quit;
}


void player::selectSprite(){

    int y = HEIGHT/2 - (mHeight/3)/2;
    // we will start at the middle then go left and right for other rects
    int baseX = WIDTH/2 - (mWidth/3)/2;

    int padX = 50;

    int mouseX,mouseY;
    SDL_Rect select {0,0,100,100};
    SDL_GetMouseState( &mouseX, &mouseY);
    
    if( mouseX >= baseX - mWidth/3 - padX && mouseX<= baseX - mWidth/3 - padX + 100 && mouseY >= y && mouseY <= y+100 ){
        select.x = baseX - mWidth/3 - padX;
        select.y = y;
        mSpriteIndex = 1;
    }

    else if( mouseX >= baseX && mouseX<= baseX + 100 && mouseY >= y && mouseY <= y+100 ){
        select.x = baseX;
        select.y = y;
        mSpriteIndex = 0;
    }

    else if( mouseX >= baseX + (mWidth/3) + padX && mouseX<= baseX + (mWidth/3) + padX + 100 && mouseY >= y && mouseY <= y+100 ){
        select.x = baseX + (mWidth/3) + padX;
        select.y = y;
        mSpriteIndex  = 2;
    }
    else{
        mSpriteIndex = 0;
    }
}

bool player::handleEvent( SDL_Event e ){
    

    if( !mGameStarted ){

        if( e.type == SDL_KEYDOWN ){
            if( e.key.keysym.sym == SDLK_SPACE )
                gTitleScreen.setReady();
        }

        if( e.type == SDL_MOUSEBUTTONDOWN ){

            if(e.button.button == SDL_BUTTON_LEFT && e.key.repeat == 0 ){
                selectSprite();
            }
    }

    }


	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_w: myVel -= mpVel; break;
            case SDLK_s: myVel += mpVel; break;
            case SDLK_a: mxVel -= mpVel; break;
            case SDLK_d: mxVel += mpVel; break;
            case SDLK_p: gBoard.changeBoard("sound_effects/sprites/fireBoard.png"); break;
            case SDLK_o: gBoard.changeBoard("sound_effects/sprites/board.png"); break;
            case SDLK_UP: mCurWeapon->changeWeapon(); break;
            case SDLK_SPACE: readyUp(); break;
            case SDLK_ESCAPE: mPaused = !mPaused; break;
            default: break;
        }
    
    }
    //If a key was released
    else if( e.type == SDL_KEYUP )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_w: myVel += mpVel; break;
            case SDLK_s: myVel -= mpVel; break;
            case SDLK_a: mxVel += mpVel; break;
            case SDLK_d: mxVel -= mpVel; break;
            default: break;
        }
    }

    else if( e.type == SDL_MOUSEBUTTONDOWN ){

        if(e.button.button == SDL_BUTTON_LEFT && e.key.repeat == 0 ){
            // this is only return false point
            if( checkQuit() )
                return false;

            if(!mPaused)
                mShoot = true;
        }
    }


    return true;
}


void player::closeConnection(){
    SDLNet_UDP_Close( mConnection.udpSocket );
}


pPos player::getPlayerPos(){
    return {mxPos, myPos};
}


// @return true or false if we successfuly can shoot
// @note is useful for sending packets as to if we successfuly fired
bool player::regProjectile(){

    // if we dont shoot we gain mana if we shoot we lose mana
    mCurWeapon->updateMana( mShoot );

    // if we are already shooting or if we dont have enough mana
    if( !mShoot || !mCurWeapon->enoughMana() )
        return false;

    // once we shoot we make it false so we dont shoot again
    mShoot = false;
                            // zeros are only there to statisfy default params, not important
                            // when its yourself shooting
    // if pushprojectle is successful then we return true else fasle
    return mCurWeapon->pushProjectile(0,0, mRot );


}

// precondition is that the other player has shot, we get in a udp package that they are able to shoot and they have shot
void player::regOtherPlayerProjectile( int playerID ){


    playerState p = mOtherPlayers[playerID];;
    pPos camPos = gBoard.getCamPos();

    int playerX, playerY;
    playerX = p.x - camPos.x - mHeight/2;
    playerY = p.y - camPos.y - mHeight/2;

    int weaponX = playerX + mHeight/2 * cos(p.rotation *M_PI/180);
    int weaponY = playerY + mHeight/2 * sin(p.rotation *M_PI/180);


    mCurWeapon->pushProjectile( weaponX, weaponY, p.rotation, false, p.idx );


}




void player::renderPlayer(){


    pPos playerPos = getPlayerPos();
    pPos camPos = gBoard.getCamPos();

    int playerX, playerY;
    playerX =  playerPos.x - camPos.x - mHeight/2;
    playerY =  playerPos.y - camPos.y - mHeight/2;

    renderStats( playerX, playerY );

    // offset the player pos by the cam pos then the player width and height to center the player in the middle
    // height is constant of sprites
    SDL_Rect dst = {playerX , playerY, mHeight, mHeight};

    SDL_Rect src = {100 * mSpriteIndex, 0, 100, 100};

    SDL_RenderCopyEx(gRenderer, mTexture , &src , &dst , mRot , NULL , SDL_FLIP_NONE );

    (mCurWeapon)->renderThisWeapon();
    (mCurWeapon)->renderProjectiles();


}

void player::renderOtherPlayers(){

    if( !mGameStarted )
        return;

    pPos camPos = gBoard.getCamPos();

    for( auto curPlayer = mOtherPlayers.begin(); curPlayer != mOtherPlayers.end(); curPlayer++ ){

        if(curPlayer->pID == -1 || curPlayer->pID == mConnection.pID ){
            continue;
        }
        
        int playerX, playerY;
        playerX = curPlayer->x - camPos.x - mHeight/2;
        playerY = curPlayer->y - camPos.y - mHeight/2;

        //rendering the playermodel
        SDL_Rect dst = {playerX , playerY, 100,100};

        SDL_Rect src = {0,0,100,100};
        
        SDL_RenderCopyEx(gRenderer, mTexture , &src , &dst , curPlayer->rotation , NULL , SDL_FLIP_NONE );


        //rendering the weapon
        int weapX = playerX + mHeight/2 * cos(curPlayer->rotation *M_PI/180);
        int weapY = playerY + mHeight/2 * sin(curPlayer->rotation *M_PI/180);
        mCurWeapon->renderOtherWeapons( weapX, weapY, curPlayer->idx, curPlayer->resetting );


    }
}




void player::updatePlayer(){
        // add the xvel and yvel to x and y pos so if the vel is 0 it will not move
    pPos playerPos = getPlayerPos();
    pPos camPos = gBoard.getCamPos();

    if( mCurWeapon->checkBonked( playerPos.x, playerPos.y ) )
        respawn();

    bool didShoot = false;

    // if we are paused we dont want to rotate and move, but we always want to update projectiles and weapon for coolness :)
    // ~~ obviously sending packs too
    if( !mPaused ){
        move();


        double rotation = getRotation( playerPos.x, playerPos.y );
        mRot = rotation;
        mCurWeapon->setPos( playerPos.x - camPos.x - mHeight/2 ,
                            playerPos.y - camPos.y - mHeight/2,  
                            mHeight/2 , 
                            rotation );

        didShoot = regProjectile();
    }
    mCurWeapon->updateProjectiles();
    mCurWeapon->updateWeapon();
    sendPlayerPack( didShoot );

        
}

//@param takes bool shot if the player shot or not
void player::sendPlayerPack( bool shot ){


    playerState p { mxPos, myPos, mRot, mConnection.pID , shot, mCurWeapon->getWeapIndex(), mCurWeapon->isResetting() };
    UDPpacket *sendPack = SDLNet_AllocPacket( sizeof(playerState) );
    sendPack->len = sizeof(playerState);
    sendPack->address = dst_ip;

    memcpy( sendPack->data, &p, sizeof(playerState) );

    if(SDLNet_UDP_Send( mConnection.udpSocket , -1,  sendPack ) == 0)
        SDL_Log("Packet not send");

    SDLNet_FreePacket( sendPack );

}


void player::updateOtherPlayers(){

    if( !mGameStarted )
        return;

    UDPpacket *recvPack = SDLNet_AllocPacket(sizeof(playerState));

    playerState *resPackData = nullptr;

    while( SDLNet_UDP_Recv( mConnection.udpSocket  , recvPack) == 1 ){

        resPackData = reinterpret_cast<playerState*>(recvPack->data);
        memcpy(&mOtherPlayers[resPackData->pID], resPackData, sizeof(playerState) );
        
        // if another player shot we want to add that to projectile queue, we might
        // want to define ownership of a projectile so we are eliminating ourselfs with out own bullet when we shoot it
        // because it is technically in the players hitbox when it shoots
        if( resPackData->shot ){
            regOtherPlayerProjectile( resPackData->pID );
        }
    }

    SDLNet_FreePacket( recvPack );
    

}


// @param x not absolute x but relaitve
// @param y relative y
void player::renderStats( int x, int y){
    
    std::string zeros = "";
    int mana = mCurWeapon->getMana();

    //edge case when we have exactly 0 mana
    if(mana == 0)
        zeros = "00";

    else if( floor( log10(mana) ) < 1 )
        zeros = "00";
    //
    else if( floor( log10(mana) ) < 2 ) )
        zeros = "0";

    // else zeros is "" which is default

    std::string manaStr = zeros+ std::to_string( mCurWeapon->getMana() ) + "/125";
    renderText( manaStr, x + 5, y - 40 );

}

// freeing resources like tcp since we only use it to connection, maybe 
// might change it since when a game ends we might want to restart but for now this is it
void player::freeUnnecesary(){

    gTitleScreen.free();
    SDLNet_TCP_Close( mConnection.tcpSocket );
    SDL_Log("Closing TCP Port");
    mConnection.tcpSocket = nullptr;

}


player::~player(){
    if( mConnection.tcpSocket != nullptr ){
        SDLNet_TCP_Close( mConnection.tcpSocket );
        mConnection.tcpSocket = nullptr;
        SDL_Log("Closing TCP Port");
    }
    if(mConnection.udpSocket != nullptr ){
        SDLNet_UDP_Close( mConnection.udpSocket );
        SDL_Log("Closing UDP Socket");
        mConnection.udpSocket = nullptr;
    }
}
