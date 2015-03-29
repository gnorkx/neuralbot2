#ifndef RENDERER_H
#define RENDERER_H

#include"tuple.h"
#include"Coord.h"
#include"color.h"
#include"SDL/SDL.h"


class renderer
{
    public:
        float scale_;
        struct {int x; int y;} shift_,resolution_;

        SDL_Surface *screen_;

        static renderer *instance_;

        ~renderer();

        renderer* Instance();
        void Init(iTuple&, string&, int scr_bpp = 32);

        void ClearScreen();
        void Rect(fTuple&, fTuple&, Color&);
        void Square(fTuple&, float, Color&);
        void Circle(fTuple&, float, Color&);
        void Line(fTuple&, fTuple&, Color&);

    private:

        void ApplySurface(iTuple&, SDL_Surface*);
        void PutPixel(SDL_Surface*, iTuple&, Uint32);
        Uint32 GetPixel(SDL_Surface*, iTuple&);


        void DrawRect(iTuple&, iTuple&, Color&);
        void DrawCircle(iTuple&, float, Color&);
        void DrawLine(iTuple&, iTuple&, Color&);


        renderer();

};

#endif // RENDERER_H
