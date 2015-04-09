#include<cstdlib>
#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<algorithm>
#include"SDL/SDL.h"
#include"SDL/SDL_keyboard.h"
#include"renderer.h"
#include"object.h"
#include"food.h"
#include"bot.h"


#define _ReadSavedBot 0
//#include"toolbox.h"



using namespace std;



int fl_ClearScreen = 1;
int fl_CenterCM = 1;


int fl_lines = false;


vector<object*> gWorld;
vector<bot*> gBots;

void WorldInit()
{

    gWorld.push_back(new food({1,1},1.));

    gBots.push_back(new bot({1,0}));


}


void Frame( void )
{

    renderer::Instance()->ClearScreen();
    for(auto it = gBots.begin(); it!=gBots.end(); ++it)
    {
        (*it)->update();
    }
        cout<<"test1"<<endl;

    for(auto it = gWorld.begin(); it!=gWorld.end(); ++it)
    {
        (*it)->update();
    }


}


int main()
{
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Event event;

    renderer::Instance()->Init({1000,1000},"neuralbot 2");

    WorldInit();


    int quit = false;
    int fl_speed = true;
    int fl_pause = false;
      while( quit == false )
      {
        while( SDL_PollEvent( &event ) )
        {
          if( event.type == SDL_QUIT )
          {
            quit = true;
          }
          else if(event.type == SDL_KEYDOWN)
          {
              if(event.key.keysym.sym == SDLK_p)
              {renderer::Instance()->scale_*=1.1;}
              else if(event.key.keysym.sym == SDLK_m)
              {renderer::Instance()->scale_*=0.9;}
              else if(event.key.keysym.sym == SDLK_LEFT)
                  {renderer::Instance()->shift_.x+=1.;fl_CenterCM = 0;}
              else if(event.key.keysym.sym == SDLK_RIGHT)
                  {renderer::Instance()->shift_.x-=1.;fl_CenterCM = 0;}
              else if(event.key.keysym.sym == SDLK_UP)
                   {renderer::Instance()->shift_.y+=1.;fl_CenterCM = 0;}
              else if(event.key.keysym.sym == SDLK_DOWN)
                   {renderer::Instance()->shift_.y-=1.;fl_CenterCM = 0;}
              else if(event.key.keysym.sym == SDLK_b)
                  fl_ClearScreen = fl_ClearScreen==1?0:1;
              else if(event.key.keysym.sym == SDLK_c)
                  fl_lines = !fl_lines;
              else if(event.key.keysym.sym == SDLK_s)
                  fl_speed = !fl_speed;
              else if(event.key.keysym.sym == SDLK_t)
                  fl_pause = !fl_pause;
              else if(event.key.keysym.sym == SDLK_q)
                  {quit = true; }
          }
        }


        if(!fl_pause)
        {
            Frame();

            renderer::Instance()->Flip();
            if(fl_speed) SDL_Delay( 5);
        }


	}
	renderer::Instance()->Quit();
    SDL_Quit();
    return 1;
}
