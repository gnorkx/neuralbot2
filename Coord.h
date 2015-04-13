
#ifndef COORD_H
#define COORD_H

#include<iostream>
#include<cmath>
#include<cstdio>


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
    float abs() const {return sqrt(x*x + y*y);} ;
    float absqr() const {return x*x + y*y;} ;
    float CosTheta( const Coord &v ) const {return (x*v.x + y*v.y)/sqrt((absqr()*v.absqr());};
    float SinTheta( const Coord &v ) const {return cross(v)/sqrt(absqr()*v.absqr());};
    float Theta( const Coord &v ) const {return acos(CosTheta(v))/M_PI*180.;};
    Coord unit() const {return Coord(x/abs(),y/abs());} ;
    Coord cross(const Coord &v) const {return Coord(y*v.x-x*v.y, x*v.y-y*v.x);};
    void rotate(float);
    Coord perp() const;

};


//non class member functions


Coord operator* (const float& , const Coord&);


#endif
