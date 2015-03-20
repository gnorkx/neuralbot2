struct Colour
{
  int R;
  int G;
  int B;
};



Uint32 GetPixel(SDL_Surface *s, int x, int y)
{
    Uint32 *pix = (Uint32*)s->pixels;
    return pix[x + y*s->w];
}

void PutPixel(SDL_Surface *s, int x, int y, Uint32 val)
{
    Uint32 *pix = (Uint32*)s->pixels;
    pix[x + y*s->w] = val;
}
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
  //Temporary rectangle to hold the offsets
  SDL_Rect offset;

  //Get the offsets
  offset.x = x;
  offset.y = y;

  //Blit the surface
  SDL_BlitSurface( source, NULL, destination, &offset );
}


void DrawRect(int x, int y, int h, int w, Colour c)
{
  SDL_Surface *surf = SDL_CreateRGBSurface(0, w, h, 32, 0,0,0,0);

  SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, c.R, c.G, c.B));

  apply_surface(x,y,surf,screen);
  SDL_FreeSurface(surf);
}
void DrawSqr(int x, int y, int l, Colour c)
{
  DrawRect(x, y, l, l, c);
}
void ClearScreen()
{
  DrawRect(0, 0, SCREEN_HEIGHT, SCREEN_WIDTH, {0,0,0});
}

void DrawCircle(int x, int y, float r, Colour c)
{

    int iRad = ceil(r);
  SDL_Surface *surf = SDL_CreateRGBSurface(0, 2*iRad, 2*iRad, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

  for(int phi = 0 ; phi<360*0.1*r+1; phi++)
  {
     // phi=1;
     PutPixel(surf,(int)r*cos(phi)+r,(int)r*sin(phi)+r,SDL_MapRGBA(surf->format, c.R, c.G, c.B,255));//SDL_MapRGB(surf->format, c.R, c.G, c.B));
  }

  apply_surface(x-iRad,y-iRad,surf,screen);
  SDL_FreeSurface(surf);
}
void DrawLine(Coord lstart, Coord lend, Colour c)
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



int Pos2Pix(float pos, float shift)
{
    return (int)(SCALE*(pos - shift ));
}
