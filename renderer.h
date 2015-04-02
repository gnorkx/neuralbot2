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


        ~renderer();

        static renderer* Instance();
        void Init(iTuple, char*, int scr_bpp = 32);

        void ClearScreen();
        void Flip();
        void Rect(Coord, Coord, color);
        void Square(Coord, float, color);
        void Circle(Coord, float, color);
        void Line(Coord, Coord, color);

    private:

        void ApplySurface(iTuple, SDL_Surface*);
        void PutPixel(SDL_Surface*, iTuple, Uint32);
        Uint32 GetPixel(SDL_Surface*, iTuple);


        void DrawRect(iTuple, iTuple, color);
        void DrawCircle(iTuple, float, color);
        void DrawLine(iTuple, iTuple, color);

        iTuple Pos2Pix(Coord);
        int Size2Pix(float);
        iTuple Size2Pix(Coord);


        renderer();

};

#endif // RENDERER_H
