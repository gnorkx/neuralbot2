#include "renderer.h"


renderer::renderer(): scale_(1.), shift_({0,0}), resolution_({0,0})
{
    //ctor


}

void renderer::Init(iTuple resolution, char *name, int scr_bpp)
{
    resolution_ = resolution;
    screen_ = SDL_SetVideoMode( resolution_.x, resolution_.y, scr_bpp, SDL_SWSURFACE );
    SDL_WM_SetCaption( name , NULL );
    scale_ = 7;
    shift_.x = -resolution_.x/2;
    shift_.y = -resolution_.y/2;
}



renderer::~renderer()
{


    SDL_FreeSurface(screen_);
}



renderer* renderer::Instance()
{
    static renderer instance_;
    return &instance_;
}

void renderer::ClearScreen()
{
  iTuple tmp(0,0);
  DrawRect(tmp, resolution_, {0,0,0});
}

void renderer::Flip()
{
    SDL_Flip( screen_ );
}

void renderer::Rect(Coord Pos, Coord Size, color c)
{
    DrawRect(Pos2Pix(Pos),Size2Pix(Size), c);
}
void renderer::Square(Coord Pos, float l, color c)
{
    Rect(Pos,{l,l},c);
}
void renderer::Circle(Coord Pos, float Radius, color c)
{
    DrawCircle(Pos2Pix(Pos),Size2Pix(Radius), c);
}
void renderer::Line(Coord Start, Coord End, color c)
{
    DrawLine(Pos2Pix(Start),Pos2Pix(End), c);
}


//private methods

void renderer::ApplySurface(iTuple Pos, SDL_Surface* source)
{
  //Temporary rectangle to hold the offsets
  SDL_Rect offset;

  //Get the offsets
  offset.x = Pos.x;
  offset.y = Pos.y;

  //Blit the surface
  SDL_BlitSurface( source, NULL, screen_, &offset );
}

void renderer::PutPixel(SDL_Surface* s, iTuple Pos, Uint32 val)
{
    Uint32 *pix = (Uint32*)s->pixels;
    pix[Pos.x + Pos.y*s->w] = val;
}

Uint32 renderer::GetPixel(SDL_Surface* s, iTuple Pos)
{
    Uint32 *pix = (Uint32*)s->pixels;
    return pix[Pos.x + Pos.y*s->w];
}

iTuple renderer::Pos2Pix(Coord Pos)
{
    return iTuple(Size2Pix(Pos.x)-shift_.x,Size2Pix(Pos.y)-shift_.y);
}

int renderer::Size2Pix(float l)
{
    return scale_*l + 0.5;
}

iTuple renderer::Size2Pix(Coord l)
{
    return iTuple(Size2Pix(l.x),Size2Pix(l.y));
}

void renderer::DrawRect(iTuple Pos, iTuple Size, color c)
{
  SDL_Surface *surf = SDL_CreateRGBSurface(0, Size.x, Size.y, 32, 0,0,0,0);

  SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, c.r, c.g, c.b));

  ApplySurface(Pos,surf);
  SDL_FreeSurface(surf);
}

void renderer::DrawCircle(iTuple Pos, float r, color c)
{

    int iRad = ceil(r);
  SDL_Surface *surf = SDL_CreateRGBSurface(0, 2*iRad, 2*iRad, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

  for(int phi = 0 ; phi<360*0.1*r+1; phi++)
  {
     // phi=1;
     iTuple PixelPos = {r*cos(phi)+r, r*sin(phi)+r};
     PutPixel(surf,PixelPos,SDL_MapRGBA(surf->format, c.r, c.g, c.b,255));//SDL_MapRGB(surf->format, c.R, c.G, c.B));
  }

    Pos.x-=iRad; Pos.y -= iRad;
  ApplySurface(Pos,surf);
  SDL_FreeSurface(surf);
}
void renderer::DrawLine(iTuple lstart, iTuple lend, color c)
{
  if(lstart.x > lend.x)
  {
    DrawLine(lend,lstart,c);
    return;
  }

  Coord Delta( lend.x-lstart.x, lend.y - lstart.y);

  SDL_Surface *surf = SDL_CreateRGBSurface(0, (Delta.x)+1., fabs(Delta.y)+1., 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);


  float n;
  Delta.y<0?n=-Delta.y:n=0;

  if(Delta.x>fabs(Delta.y))
  {
      float m= Delta.y/Delta.x;
      for(int x = 0; x<Delta.x; x++)
      {
     // cout<<lend.x-lstart.x<<" "<<n<<" "<<m<<" "<<x<<" "<<(int)(m*x+n)<<endl;
        iTuple PixelPos(x,m*x+n);
         PutPixel(surf,PixelPos,SDL_MapRGBA(surf->format, c.r, c.g, c.b,255));//SDL_MapRGB(surf->format, c.R, c.G, c.B));
      }
  } else {
    float m = Delta.x/Delta.y;
     for(int y = 0; y<fabs(Delta.y); y++)
      {
         iTuple PixelPos((y-n)*m,y);
         PutPixel(surf,PixelPos,SDL_MapRGBA(surf->format, c.r, c.g, c.b,255));//SDL_MapRGB(surf->format, c.R, c.G, c.B));
      }
    }

  lstart.y-=n;
  ApplySurface(lstart,surf);
  SDL_FreeSurface(surf);
}
