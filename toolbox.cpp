#include"toolbox.h"

std::mt19937 r(0);
std::normal_distribution<float> norm_dist(0.,1.);
float rnd( void)
{//rando numbers in 0..1
  return (float)r()/r.max();
}
float rnd0( float scale)
{//random numbers beween -scale..scale
  return 2.*scale*(rnd()-0.5);
}

float norm(float stdev)
{
    return stdev*norm_dist(r);
}

int signum( float x )
{return x>=0?1:-1;}
int signum( int x )
{return x>=0?1:-1;}
