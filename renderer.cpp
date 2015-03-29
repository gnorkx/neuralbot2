#include "renderer.h"

renderer::renderer(): scale_(1.), shift_({0,0}), resolution_({0,0})
{
    //ctor


}

void renderer::Init(iTuple &resolution, string &name, int src_bpp)
{
    resolution_ = res;
    screen_ = SDL_SetVideoMode( resolution_.x, resolution_.y, scr_bpp, SDL_SWSURFACE );
    SDL_WM_SetCaption( name.c_str();, NULL );
}



renderer::~renderer()
{


    SDL_FreeSurface(screen_);
    delete instance_;
}



renderer* renderer::Instance()
{
    if(!instance_)
        instance_ = new renderer();
    return instance_;
}

void renderer::ClearScreen()
{
  DrawRect(0, 0, resolution_.x, resolution_.y, {0,0,0});
}

void renderer::Rect(fTuple &Pos, fTuple &Size, Color &c)
{
    DrawRect(Pos2Pix(Pos),Size2Pix(Size), c);
}
void renderer::Square(fTuple &Pos, float l, Color &c)
{
    Rect(Pos,{l,l},c);
}
void renderer::Circle(fTuple &Pos, float Radius, Color &c)
{
    DrawCircle(Pos2Pix(Pos),Size2Pix(Radius), c);
}
void renderer::Line(fTuple &Start, fTuple &End, Color &c)
{
    DrawLine(Pos2Pix(Start),Pos2Pix(End), c);
}


//private methods

void renderer::ApplySurface(iTuple& Pos, SDL_Surface*)
{
  //Temporary rectangle to hold the offsets
  SDL_Rect offset;

  //Get the offsets
  offset.x = Pos.x;
  offset.y = Pos.y;

  //Blit the surface
  SDL_BlitSurface( source_, NULL, destination, &offset );
}

void renderer::PutPixel(SDL_Surface* s, iTuple& Pos, Uint32 val)
{
    Uint32 *pix = (Uint32*)s->pixels;
    pix[Pos.x + Pos.y*s->w] = val;
}

Uint32 renderer::GetPixel(SDL_Surface* s, iTuple& Pos)
{
    Uint32 *pix = (Uint32*)s->pixels;
    return pix[Pos.x + Pos.y*s->w];
}

iTuple Pos2Pix(fTuple &Pos)
{
    Pos.x -= shift_.x;
    Pos.y -= shift_.y;
    return iTuple(Size2Pix(Pos.x),Size2Pix(Pos.y));
}

int Size2Pix(float l)
{
    return scale_*l + 0.5;
}

iTuple Size2Pix(fTuple &l)
{
    return iTuple(Size2Pix(l.x),Size2Pix(l.y))
}

void renderer::DrawRect(iTuple& Pos, iTuple& Size, Color& c)
{
  SDL_Surface *surf = SDL_CreateRGBSurface(0, Size.x, Size.y, 32, 0,0,0,0);

  SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, c.R, c.G, c.B));

  apply_surface(Pos.x,Pos.y,surf,screen_);
  SDL_FreeSurface(surf);
}

void renderer::DrawCircle(iTuple &Pos, float r, Color &c)
{

    int iRad = ceil(r);
  SDL_Surface *surf = SDL_CreateRGBSurface(0, 2*iRad, 2*iRad, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

  for(int phi = 0 ; phi<360*0.1*r+1; phi++)
  {
     // phi=1;
     PutPixel(surf,(int)r*cos(phi)+r,(int)r*sin(phi)+r,SDL_MapRGBA(surf->format, c.R, c.G, c.B,255));//SDL_MapRGB(surf->format, c.R, c.G, c.B));
  }

  apply_surface(Pos.x-iRad,Pos.y-iRad,surf,screen_);
  SDL_FreeSurface(surf);
}
void renderer::DrawLine(iTuple&, iTuple&, Color&)
{
  if(lstart.x > lend.x)
  {
    DrawLine(lend,lstart,c);
    return;
  }

  Coord Delta = lend-lstart;

  SDL_Surface *surf = SDL_CreateRGBSurface(0, (Delta.x)+1., fabs(Delta.y)+1., 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);


  float n;
  Delta.y<0?n=-Delta.y:n=0;

  if(Delta.x>fabs(Delta.y))
  {
      float m= Delta.y/Delta.x;
      for(int x = 0; x<Delta.x; x++)
      {
     // cout<<lend.x-lstart.x<<" "<<n<<" "<<m<<" "<<x<<" "<<(int)(m*x+n)<<endl;
         PutPixel(surf,x,(int)(m*x+n),SDL_MapRGBA(surf->format, c.R, c.G, c.B,255));//SDL_MapRGB(surf->format, c.R, c.G, c.B));
      }
  } else {
    float m = Delta.x/Delta.y;
     for(int y = 0; y<fabs(Delta.y); y++)
      {
         PutPixel(surf,(y-n)*m,y,SDL_MapRGBA(surf->format, c.R, c.G, c.B,255));//SDL_MapRGB(surf->format, c.R, c.G, c.B));
      }
    }

  apply_surface(lstart.x,lstart.y-n,surf,screen);
  SDL_FreeSurface(surf);
}
