#pragma once

#include "LTexture.hpp"
#include "math.hpp"
#include "board.hpp"
#include <list>
#include <vector>

union meshes{
    SDL_Rect mesh0;
    SDL_Rect mesh1;
    SDL_Rect mesh2;
};

struct pos{
    int x, y;
};


struct projectile{

    meshes meshSprites;

    normal normVec;
    double rotation;
    float x, y;

    Uint16 curFrame;
    int curMesh;
    int numIndex;

    // all projectils can be described as is own, or not is own
    bool isOwn;

};


class weapon : public LTexture{
    public:
        // path will be loaded into loadFromFile, numSprites will detemine the frame cycles
        // size will be the width and height of the whole atlas, if  i will try to keep each individvual
        // sprite at 100x100 so 3 sprites will be 300x100
        weapon(const std::string &path, int numSprites, int size, int numWeaps);

        void updateProjectiles();

        // takes rotation of player for animation of projectile
        //@returns true if succcessful false if not
        bool pushProjectile(  int weaponX = 0, int weaponY = 0, const double &rot = 0, bool isOwn = true, int weapIdx = 0 );
        void checkRemoveBullet();
        bool enoughMana();
        void updateMana( bool );
        void updateWeapon();
        void changeWeapon();

        int getMana();
        int getCapacity();
        Uint8 getWeapIndex();
        bool isResetting();
        bool checkBonked( int, int );

        void renderProjectiles();
        void renderThisWeapon();
        void renderOtherWeapons( int x, int y, int weapIdx, bool resetting);


        void setPos( int x, int y, int radius, const double &rotation );        

    private:

        // @note x and y are absolute positions
        std::list<projectile> mProjectileQueue;

        void setMesh( int weapIndex, meshes &pMesh );
        // updateMesh is for projectiles shot whereas set mesh is for the standing animation when holding it
        void updateProjectileMesh( projectile &p );
        
        meshes curMesh;

        int mxPos;
        int myPos;

        Uint16 mFrame;
        Uint16 mSize;
        // the ammount of sprites for each weapon animation
        Uint16 mNumSprites;
        Uint16 mNumWeapons;
        // we offset the y for dest rect by mHeight/mNumWeapons * curWeapon
        Uint16 mCurWeaponIndex;
        Uint16 mMeshNum;



        // in ms
        Uint16 mWeaponResetTime;
        Uint32 mShotStartTime;
        bool mWeaponResetting;

        // in seconds
        int mProjectileSpeed;
        int mManaCost;
        int mManaRegenRate;
        float mCurMana;
        int mManaCapacity;

};

extern const int WIDTH;
extern const int HEIGHT;
extern board gBoard;

