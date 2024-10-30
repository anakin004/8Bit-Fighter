#include "title.hpp"

#include <SDL.h>

titleScreen::titleScreen() {

    // we need to initalize texture before game loop because we need to init first
    mReady = false;
    mTime = 0;
    mBlink = 1000;
    mBlinking = false;
}


void titleScreen::renderTitleScreen(){

    // not really expensive since we are just doing title screen
    FC_LoadFont( mFont, gRenderer, "fonts/lazy.ttf", 50, FC_MakeColor( 0, 0, 0,150), TTF_STYLE_NORMAL);

    // each player is 100x100 pixels
    SDL_Rect player1 {0,  0,100,100};
    SDL_Rect player2 {100,0,100,100};
    SDL_Rect player3 {200,0,100,100};

    // y will be constant           
    int y = HEIGHT/2 - (mHeight/3)/2;
    // we will start at the middle then go left and right for other rects
    int baseX = WIDTH/2 - (mWidth/3)/2;

    int padX = 50;

    // we offset by mWidth, because we need to space out by a whole sprite  /3 as we have 3 sprites total 300/3 = 300
    SDL_Rect dst1 {baseX,                       y,100,100};
    SDL_Rect dst2 {baseX - (mWidth/3) - padX,   y,100,100};
    SDL_Rect dst3 {baseX + (mWidth/3) + padX,   y,100,100};


    SDL_RenderCopy( gRenderer, mTexture, &player1, &dst1 );
    SDL_RenderCopy( gRenderer, mTexture, &player2, &dst2 );
    SDL_RenderCopy( gRenderer, mTexture, &player3, &dst3 );


    // drawing box around current player select
    int mouseX,mouseY;
    SDL_Rect select {0,0,100,100};
    SDL_GetMouseState( &mouseX, &mouseY);
    bool valid = false;

    if( mouseX >= baseX - mWidth/3 - padX && mouseX<= baseX - mWidth/3 - padX + 100 && mouseY >= y && mouseY <= y+100 ){
        select.x = baseX - mWidth/3 - padX;
        select.y = y;
        valid = true;
    }

    else if( mouseX >= baseX && mouseX<= baseX + 100 && mouseY >= y && mouseY <= y+100 ){
        select.x = baseX;
        select.y = y;
        valid = true;
    }

    else if( mouseX >= baseX + (mWidth/3) + padX && mouseX<= baseX + (mWidth/3) + padX + 100 && mouseY >= y && mouseY <= y+100 ){
        select.x = baseX + (mWidth/3) + padX;
        select.y = y;
        valid = true;
    }

    if(valid){
    SDL_SetRenderDrawColor(gRenderer, 0,0,0,255);
    SDL_RenderDrawRect(gRenderer, &select );
    }

    const char* text = ":: Choose Your Character ::";
    int w = FC_GetWidth( mFont,text );
    int h = FC_GetHeight( mFont, text);
    renderTitleText(text, WIDTH/2 - w/2, HEIGHT/2 - h/2 - 100);

    const char* text2 = "- Click Space to Join -";
    w = FC_GetWidth( mFont,text2 );
    h = FC_GetHeight( mFont, text2);
    renderTitleText(text2, WIDTH/2 - w/2, HEIGHT/2 - h/2 + 150);

    Uint32 curTime = SDL_GetTicks();
    if(curTime - mTime > mBlink && curTime > 15000 ){
        mBlinking = !mBlinking;
        mTime = curTime;
    }
    if(mBlinking && !mReady ){
        const char* text3 = "HI :)";
        w = FC_GetWidth(mFont, text3);
        h = FC_GetHeight(mFont, text3);
        SDL_Color color = {230,22,23,30};
        renderTitleText( text3, WIDTH/2 - w/2, HEIGHT/2 - h/2 + 210, &color );
    }


    FC_LoadFont( mFont, gRenderer, "fonts/lazy.ttf", 100, FC_MakeColor( 0, 0, 0, 0), TTF_STYLE_BOLD);
    const char* title = "FighterV2";
    w = FC_GetWidth( mFont,title );
    h = FC_GetHeight( mFont, title);
    SDL_Color red = {200,0,0,199};
    renderTitleText( title, WIDTH/2 - w/2, HEIGHT/2 - h/2 - 200, &red );


    FC_LoadFont( mFont, gRenderer, "fonts/lazy.ttf", 20, FC_MakeColor( 0, 0, 0, 0), TTF_STYLE_ITALIC);
    SDL_Color crimson = {150,34,34,255};
    const char* subtext = "Ryan Mangeno (TM)  ~~ anakin";
    w = FC_GetWidth( mFont,subtext );
    h = FC_GetHeight( mFont, subtext);
    renderTitleText( subtext, WIDTH/2 - w/2 + 115, HEIGHT/2 - h/2 - 160, &crimson );

}


void titleScreen::setReady(){
    mReady = true;
}