#ifndef TOOLBOX_H_INCLUDED
#define TOOLBOX_H_INCLUDED

#include<cstdlib>
#include<random>



float rnd( void);
//rando numbers in 0..1

float rnd0( float scale = 1.);
//random numbers beween -scale..scale}

float norm(float );


int signum( int );
int signum( float );



#endif // TOOLBOX_H_INCLUDED
