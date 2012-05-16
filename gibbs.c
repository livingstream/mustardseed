/*
gibbs.c
c code for some simple gibbs sampling

compile with something like

gcc -lgsl -lgslblasnative gibbs.c

and run with something like

a.out 10000 0.98 > bvn.dat

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

/* main loop */

int main(int argc, char *argv[])
{
  long n,i;
  double x,y,rho,sd;
  gsl_rng *r = gsl_rng_alloc(gsl_rng_mt19937);
  n=(long) atoi(argv[1]);
  rho=(double) atof(argv[2]);
  sd=sqrt(1-rho*rho);
  x=0;y=0;
  printf(" %3.3f %3.3f \n",x,y);
  for (i=1;i<n;i++)
    {
      x=rho*y+gsl_ran_gaussian(r,sd);
      y=rho*x+gsl_ran_gaussian(r,sd);
      printf(" %3.3f %3.3f \n",x,y);      
    } 
  return(0);
}




/* end of file */











