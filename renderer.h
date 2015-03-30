#ifndef RENDERER_H
#define RENDERER_H

#include"tuple.h"
#include"Coord.h"
#include"color.h"
#include<string>
#include"SDL/SDL.h"

using namespace std;

class renderer
{
    public:
        float scale_;
        iTuple shift_;
        iTuple resolution_;

        SDL_Surface *screen_;

        static renderer *instance_;

        ~renderer();

        renderer* Instance();
        void Init(iTuple&, string&, int scr_bpp = 32);

        void ClearScreen();
        void Rect(fTuple, fTuple, color);
        void Square(fTuple, float, color);
        void Circle(fTuple, float, color);
        void Line(fTuple, fTuple, color);

    private:

        void ApplySurface(iTuple, SDL_Surface*);
        void PutPixel(SDL_Surface*, iTuple, Uint32);
        Uint32 GetPixel(SDL_Surface*, iTuple);


        void DrawRect(iTuple, iTuple, color);
        void DrawCircle(iTuple, float, color);
        void DrawLine(iTuple, iTuple, color);

        iTuple Pos2Pix(fTuple);
        int Size2Pix(float);
        iTuple Size2Pix(fTuple);


        renderer();

};

#endif // RENDERER_H
