
#ifndef COORD_CPP
#define COORD_CPP

#include<cstdio>
#include"Coord.h"
Coord Coord::operator +(const Coord& rhs)
{
  return Coord(x+rhs.x, y+rhs.y);
}

Coord Coord::operator -(const Coord& rhs)
{
  return Coord(x-rhs.x, y-rhs.y);
}

Coord& Coord::operator +=(const Coord& rhs)
{
  x += rhs.x;
  y += rhs.y;
  return *this;
}
Coord& Coord::operator -=(const Coord& rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  return *this;
}
float Coord::operator* (const Coord& rhs)
{
   return Coord(x+rhs.x, y+rhs.y).abs();
}

void Coord::swap(Coord& c2)
{
    float tmp;
    tmp = c2.x;
    c2.x = x;
    x = tmp;
    tmp = c2.y;
    c2.y = x;
    y = tmp;

}

std::ostream& operator << (std::ostream& os, const Coord& co)
{
  os<<co.x<<", "<<co.y;
  return os;
}

//non class member functions


Coord operator* (const float& a , const Coord& rhs)
{
  return Coord(a*rhs.x, a*rhs.y);
};


void rotate(float alpha)
{
    alpha = alpha/180.*M_PI;
    float x1 = cos(alpha)*x + sin(alpha)*y;
    float y1 = cos(alpha)*y - sin(alpha)*x;
    x= x1; y= y1;

}


#endif

