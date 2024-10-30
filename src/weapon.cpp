#include "weapon.hpp"
#include "macros.hpp"
#include <cmath>


weapon::weapon( const std::string &path, int numSprites, int size, int numWeaps) : mNumSprites(numSprites), mFrame(1), mSize(size),
                                                                      mManaCapacity(125), mCurMana(150), mManaRegenRate(15),
                                                                      mxPos(0), myPos(0), mMeshNum(0), mProjectileQueue(), mManaCost(25),
                                                                      mProjectileSpeed(2), mWeaponResetTime(900), mWeaponResetting(false),
                                                                      mShotStartTime(0), mNumWeapons(numWeaps), mCurWeaponIndex(1) {
    loadFromFile( path );
    curMesh.mesh0 = {0,100 * mCurWeaponIndex ,100,100};

}


int weapon::getMana(){
    return static_cast<int>(mCurMana);
}

void weapon::updateMana( bool shoot ){
    // three conditions
    // we only take mana away if
    /*
    1. We are shooting 
    2. We have enough Mana
    3. we are not resetting - when we shoot the fire bolt becomes unavailaible
    
    */
    if( shoot && enoughMana() && !mWeaponResetting ){
        mCurMana -= mManaCost;
        return;
    }

    else if( mCurMana + mManaRegenRate/144.f > mManaCapacity ){
        mCurMana = mManaCapacity;
        return;
    }

    mCurMana += mManaRegenRate/144.f;
}


void weapon::setMesh( int weapIndex, meshes &pMesh ){
    // if we are at frame 144 if will be 144/numSprites for ex 3 it will be 48 then 144/48 is 3, we have 0,1,2 indexes for meshes
    if (mFrame > 143 )
        mFrame = 0;

    mMeshNum = mFrame / (143/mNumSprites);
    int offset = mHeight/mNumWeapons * weapIndex;


    switch(mMeshNum){
        case 0:
            pMesh.mesh0 = {0, offset ,100,100};
            break;
        case 1:
            pMesh.mesh1 = {100, offset ,100,100};
            break;
        case 2:
            pMesh.mesh2 = {200, offset ,100,100};
            break;
        default:
            break;
    }

    ++mFrame;
}



// x and y is the player x and y pos, the middle of the player model, rad will be width/2 sor height/2
void weapon::setPos(int x, int y, int radius, const double &rotation ){
    mxPos = x + radius * cos(rotation*M_PI/180);
    myPos = y + radius * sin(rotation*M_PI/180);
}


void weapon::changeWeapon()
{
    mCurWeaponIndex = (mCurWeaponIndex+1) % mNumWeapons;
}

// if its yourself shooting , just pass in rotation, else pass in x , y , rot , false, playerID
// @param weaponX the absolute x position not relative to camera
// @param waaponY also the absolute position for y
bool weapon::pushProjectile( int weaponX, int weaponY, const double &rot, bool isOwn, int weapIdx ){

    if( isOwn ){
        // if we are resetting  weapon after shooting we dont want to  be able to shoot 
        if( mWeaponResetting )
            return false;

        int x, y;  
        SDL_GetMouseState(&x,&y);

        pPos cam = gBoard.getCamPos();

        normal normVec = normalize( x-mxPos, y-myPos );

        mProjectileQueue.push_back({curMesh, normVec, rot, mxPos + cam.x * 1.f , myPos + cam.y * 1.f  , mFrame, mMeshNum, mCurWeaponIndex, isOwn } );
        // when push a projectile (shoot) we need to also reset the fire ball, so we dont render the fireball and also rendering the projectile

        mWeaponResetting = true;
        mShotStartTime = SDL_GetTicks();
    }

    else {

        pPos cam = gBoard.getCamPos();

        normal normVec = {cos( rot* (M_PI/180) ) , sin( rot* (M_PI/180) )};

        mProjectileQueue.push_back({ curMesh, normVec, rot, (float)weaponX + cam.x , (float)weaponY + cam.y , mFrame, mMeshNum, weapIdx, isOwn });

        // but we will be able to properly render that, simply, we we shoot we will past that we have shot and every packet sent
        // will send a boolean saying if we are resetting so the weapon stops rendering for all players
    }

    return true;
}


bool weapon::enoughMana(){

    if( mCurMana - mManaCost >= 0 )
        return true;
            
    return false;
}

void weapon::updateProjectiles(){

    if( mProjectileQueue.empty() )
        return;

    checkRemoveBullet();
    
    for( auto it = mProjectileQueue.begin() ; it != mProjectileQueue.end() ; it++ ){
        
        it->x += static_cast<float>(mProjectileSpeed * it->normVec.x);
        it->y += static_cast<float>(mProjectileSpeed * it->normVec.y);
        updateProjectileMesh( *(it) );
    }


}

//checks bounds of projectile
bool checkBounds( const projectile& proj){
    pPos cam = gBoard.getCamPos();
    bool remove = false;

    if(proj.isOwn){
        if( proj.x > cam.x + WIDTH || proj.x < cam.x  )
            remove = true;

        else if( proj.y > cam.y + HEIGHT  || proj.y < cam.y ) 
            remove = true;
    }
    else{
        if( proj.y > gBoard.getHeight() || proj.x > gBoard.getWidth() || proj.y < 0 || proj.x < 0 )
            remove = true;
    }
    
    return remove;
}

// @note checks if player has been hit
bool weapon::checkBonked( int playerX, int playerY ){
    for( auto it = mProjectileQueue.begin() ; it != mProjectileQueue.end() ; it++ ){

        if( !it->isOwn ){
            if( squaredDistCollision( (int)it->x, (int)it->y, 50 , playerX - 50 , playerY - 50 , 50 ))
                return true;
        }

    }
    return false;
}


void weapon::checkRemoveBullet(){

    mProjectileQueue.remove_if(checkBounds);

}

void weapon::updateWeapon(){
    if( SDL_GetTicks() - mShotStartTime > mWeaponResetTime )
        mWeaponResetting = false;
}


void weapon::renderThisWeapon(){


    if( mWeaponResetting )
        return;

    SDL_Rect dst { mxPos, myPos, mSize, mSize};

    setMesh( mCurWeaponIndex, curMesh );
    switch(mMeshNum){
        case 0:
            SDL_RenderCopy( gRenderer, mTexture, &curMesh.mesh0, &dst );
            break;
        case 1:
            SDL_RenderCopy( gRenderer, mTexture, &curMesh.mesh1, &dst );
            break;
        case 2:
            SDL_RenderCopy( gRenderer, mTexture, &curMesh.mesh2, &dst );
            break;
        default:
            break;
    }


}


void weapon::renderOtherWeapons( int x, int y, int weapIdx, bool resetting ){
    
    if(!resetting){

        SDL_Rect dst { x, y, mSize, mSize};
        meshes otherMesh;
        setMesh(weapIdx, otherMesh);

        switch(mMeshNum){
            case 0:
                SDL_RenderCopy( gRenderer, mTexture, &otherMesh.mesh0, &dst );
                break;
            case 1:
                SDL_RenderCopy( gRenderer, mTexture, &otherMesh.mesh1, &dst );
                break;
            case 2:
                SDL_RenderCopy( gRenderer, mTexture, &otherMesh.mesh2, &dst );
                break;
            default:
                break;
        }

    }
}

Uint8 weapon::getWeapIndex(){
    return static_cast<Uint8>(mCurWeaponIndex);
}



int weapon::getCapacity(){
    return mManaCapacity;
}

bool weapon::isResetting(){
    return mWeaponResetting;
}

void weapon::updateProjectileMesh( projectile &p ){
    if ( p.curFrame > 143 )
        p.curFrame = 0;
    p.curMesh = p.curFrame / (143/mNumSprites);
    int offset = mHeight/mNumWeapons * p.numIndex;

    switch( p.curMesh ){
        case 0:
            p.meshSprites.mesh0 = {0, offset ,100,100};
            break;
        case 1:
            p.meshSprites.mesh1 = {100, offset ,100,100};
            break;
        case 2:
            p.meshSprites.mesh2 = {200, offset ,100,100};
            break;
        default:
            break;
    }

    ++p.curFrame;
}

// @note it renders by the absolute position, so we need to make sure the projectiels store the absolute position
void weapon::renderProjectiles(){

    pPos cam = gBoard.getCamPos();

    for( auto it = mProjectileQueue.begin(); it != mProjectileQueue.end() ; it++ ){
        SDL_FRect dst = { it->x  - cam.x , it->y - cam.y, mWidth/mNumSprites * 1.f, mHeight/mNumWeapons * 1.f};
        switch(it->curMesh){

        case 0:
            SDL_RenderCopyF( gRenderer, mTexture, &((it->meshSprites).mesh0), &dst );
            break;
        case 1:
            SDL_RenderCopyF( gRenderer, mTexture, &((it->meshSprites).mesh1), &dst );
            break;
        case 2:
            SDL_RenderCopyF( gRenderer, mTexture, &((it->meshSprites).mesh2), &dst );
            break;
        default:
            break;

        }
    }
}