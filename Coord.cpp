

#include"Coord.h"
Coord Coord::operator +(const Coord& rhs) const
{
  return Coord(x+rhs.x, y+rhs.y);
}

Coord Coord::operator -(const Coord& rhs) const
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
float Coord::operator* (const Coord& rhs) const
{
   return x*rhs.x + y*rhs.y;
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


void Coord::rotate(float alpha)
{
    float x1 = cos(alpha)*x + sin(alpha)*y;
    float y1 = cos(alpha)*y - sin(alpha)*x;
    x= x1; y= y1;

}

Coord Coord::perp() const
{
    return Coord(y,-x);
}

bool Coord::sameDirection(const Coord &v) const
{
    Coord Diff = v - *this;
    Coord vLarge = v.absqr()>this->absqr()?v:*this;

    if( fabs(Diff*vLarge)/vLarge.abs() < vLarge.abs() )
        return true;
    else
        return false;
}


float Coord::ThetaMath(const Coord &v) const
{
    float phi =asin(cross(v)/sqrt(absqr()*v.absqr()));

    if(sameDirection(v))
    {
        if(phi>0)
            return phi;
        else
            return 2*M_PI + phi;
    }
    else
        return M_PI - phi;

}
