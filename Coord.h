
#ifndef COORD_H
#define COORD_H

#include<iostream>
#include<cmath>



class Coord{
  public:
    float x;
    float y;

    Coord(){x=0;y=0;};
    Coord(float a, float b): x(a), y(b) {};
    ~Coord(){};

    Coord operator + (const Coord&);
    Coord operator - (const Coord&);
    Coord& operator += (const Coord&);
    Coord& operator -= (const Coord&);
    float operator* (const Coord&);
    friend std::ostream& operator << (std::ostream&, const Coord&);

    void scale(float a){x*=a; y*=a;};
    void swap(Coord&);
    float abs(){return sqrt(x*x + y*y);};
    float absqr(){return x*x + y*y;};
    float CosTheta( Coord v ){return (x*v.x + y*v.y)/(x*x + y*y)/(v.x*v.x+v.y*v.y);};
    float Theta( Coord v ){return acos(CosTheta(v))/M_PI*180.;};
    Coord unit(){return Coord(x/abs(),y/abs());};

};


//non class member functions


Coord operator* (const float& , const Coord&);


#endif
