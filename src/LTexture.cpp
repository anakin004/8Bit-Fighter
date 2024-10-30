#include "LTexture.hpp"
#include <SDL_FontCache.h>
#include "macros.hpp"

LTexture::LTexture() {
    mTexture = nullptr;
    mFontSize = 20;
    mFont = FC_CreateFont();                    //pt size
    FC_LoadFont( mFont, gRenderer, "fonts/lazy.ttf",  mFontSize  , FC_MakeColor(0,0,255,255), TTF_STYLE_NORMAL);
    if(mFont == nullptr)
        SDL_Log("FC_LoadFont Failed");
    mWidth = 0;
    mHeight = 0;

}

LTexture::~LTexture() {
    free();
}

void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != nullptr )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = nullptr;

    }
    if( mFont != nullptr ){
        FC_FreeFont( mFont );
        mFont = nullptr;
    }
}

int LTexture::getWidth() {
    return mWidth;
}

int LTexture::getHeight() {
    return mHeight;
}



bool LTexture::loadFromFile( std::string path )
{

    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );

    SDL_Surface* src = SDL_GetWindowSurface( gWindow );

    SDL_Surface* optimized = NULL;

    if( loadedSurface == NULL )
    {
        DEBUG_PRINT( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        optimized = SDL_ConvertSurface ( loadedSurface, src->format, 0 );
        
        if( optimized == NULL ){
            DEBUG_PRINT("Cannot optimize");
        }
        else{
            SDL_SetColorKey(optimized, SDL_TRUE, SDL_MapRGB( optimized->format, 0,0,0));
            
            //Create texture from surface pixels
            newTexture = SDL_CreateTextureFromSurface( gRenderer, optimized );
            if( newTexture == NULL )
            {
                DEBUG_PRINT( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
            }
            else
            {

                //Get image dimensions
                mWidth = loadedSurface->w;
                mHeight = loadedSurface->h;
            }

            //Get rid of old loaded surface
            SDL_FreeSurface( loadedSurface );
            SDL_FreeSurface( optimized );

        }
    }

    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}


// primarily meant for rendering playerStatText
// this will be more obvious why its speicfically for that
// we will render text to the title screen and dont want that text cached
// the only text we care about it player stat stuff
void LTexture::renderText( std::string textureText, int x, int y)
{
    if(mFont == nullptr)
        printf("NULL");
    
    FC_Draw( mFont, gRenderer, x, y, textureText.c_str() );

}

void LTexture::renderTitleText( std::string textureText, int x, int y, SDL_Color *color)
{
    if(mFont == nullptr)
        printf("NULL");
    
    if(color == nullptr)
        FC_Draw( mFont, gRenderer, x, y, textureText.c_str() );

    else{
        FC_DrawColor( mFont, gRenderer, x, y, *color, textureText.c_str() );
    }
}


