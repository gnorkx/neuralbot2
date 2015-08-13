#include<cstdlib>
#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<algorithm>
#include"SDL/SDL.h"
#include"SDL/SDL_keyboard.h"
#include"tools/renderer.h"
#include"objects/object.h"
#include"objects/food.h"
#include"objects/bot.h"
#include"observer/observerManager.h"
#include"observer/stats.h"


#define _ReadSavedBot 0
//#include"toolbox.h"



using namespace std;



int fl_ClearScreen = 1;
int fl_CenterCM = 1;


int fl_lines = false;

int iFrame = 0;


vector<object*> gWorld;
vector<bot*> gBots;
vector<bot*> gNewBots;
bot* gBestBot;

void WorldInit()
{
    observerManager::Instance()->deleteAll();
    observerManager::Instance()->Add(new stats);

    for(int i = 0; i< 200;i++)
    {
        gWorld.push_back(new food({rnd0(300),rnd0(300)},1.));
        ((food*)gWorld[i])->subject_.notify(gWorld[i],stats::event::active);
    }
//    gWorld.push_back(new food({5,5},1.));
//    gWorld.push_back(new food({20,-10},1.));

    for(int i = 0; i< 20;i++)
    {
        gBots.push_back(new bot({rnd0(70),rnd0(70)}));
        gBots[i]->subject_.notify(gBots[i],stats::event::born);
    }
    //gBots.push_back(new bot({10,-80}));
   //gBots[0]->Direction_ = {1,1};
 //   gBots[0]->Vel_ = {0,0.1};

    gBestBot = new bot({0,0});

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


    if(iFrame%100 == 0)
    {
        iFrame = 0;
        //cleanup
        //cout<<"cleanup"<<endl;
        gBots.erase(std::remove_if(gBots.begin(), gBots.end(), [](const bot *b){return !b->active_;}), gBots.end());

        //get fittest bots
        //partial_sort(gBots.begin(), gBots.begin()+2, gBots.end(), [](const bot *a, const bot *b){return a->GetFitness()>b->GetFitness();});
        bot* tmpBot = gBots[0];
        for(auto it : gBots)
            if(it->GetFitness() > tmpBot->GetFitness())
                tmpBot = &(*it);
        if(tmpBot->GetFitness() > gBestBot->GetFitness())
        {
            delete gBestBot;
            gBestBot = new bot(*tmpBot);
            cout<<"new Best Bot: "<<gBestBot->GetFitness()<<endl;
        }

        if(gBots.size()<20)
        for(int i = gBots.size(); i< 20; i++)
        {
            bot* nBot = new bot(*gBestBot);
            nBot->Lifetime_ = 0;
            nBot->mutate();
            nBot->Pos_ = {rnd0(70),rnd0(70)};

            gNewBots.push_back(nBot);
            //cout<<"new Bot "<<gNewBots.size()<<endl;
        }

    }
    if(gNewBots.size())
    {
        for(int i = 0; i< gNewBots.size();i++)
            gBots.push_back(gNewBots[i]);
        gNewBots.clear();

    }
    iFrame++;

}


int main()
{
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Event event;

    renderer::Instance()->Init({1000,1000},"neuralbot 2");
    renderer::Instance()->scale_ = 3;

    WorldInit();


    //checks
    cout<<"sizes: \n";
    cout<<"bot: "<<sizeof(bot)<<endl;
    cout<<"nnet: "<<sizeof(FANN::neural_net)<<endl;
    cout<<"Coord: "<<sizeof(Coord)<<endl;
    cout<<"double: "<<sizeof(double)<<endl;



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
	observerManager::Instance()->deleteAll();
	renderer::Instance()->Quit();
    SDL_Quit();
    return 1;
}
