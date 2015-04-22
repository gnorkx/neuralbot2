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
vector<bot*> gNewBots;

void WorldInit()
{

    for(int i = 0; i< 200;i++)
        gWorld.push_back(new food({rnd0(150),rnd0(150)},1.));
//    gWorld.push_back(new food({5,5},1.));
//    gWorld.push_back(new food({20,-10},1.));

    for(int i = 0; i< 20;i++)
        gBots.push_back(new bot({rnd0(70),rnd0(70)}));
    //gBots.push_back(new bot({10,-80}));
   //gBots[0]->Direction_ = {1,1};
 //   gBots[0]->Vel_ = {0,0.1};


}


void Frame( void )
{

    renderer::Instance()->ClearScreen();
    for(int i = 0; i<gBots.size(); ++i)
    {
        gBots[i]->update();
    }
    //    cout<<"test1"<<endl;

    for(auto it = gWorld.begin(); it!=gWorld.end(); ++it)
    {
        (*it)->update();
    }

    if(gNewBots.size())
    {
        for(int i = 0; i< gNewBots.size();i++)
            gBots.push_back(gNewBots[i]);
        gNewBots.clear();

    }

}


int main()
{
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Event event;

    renderer::Instance()->Init({1000,1000},"neuralbot 2");
    renderer::Instance()->scale_ = 3;

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
