#include <stdio.h>
#include "blockmatr.h"
//#include <X86intrin.h>
#include <string>
#include <stdint.h>
#include <iostream>

using namespace std;

static uint64_t rdtsc(void) 
{
  unsigned hi,lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ((unsigned long long)lo)|( ((unsigned long long)hi) << 32);
}

int main(int argc, char **argv) {

   const int ay = argc > 1 ? std::stoi(argv[1]) : 1024, 
             ax = argc > 2 ? std::stoi(argv[2]) : 1024, 
             by = ax, 
             bx = argc > 3 ? std::stoi(argv[3]) : 1024, 
             cy = ay, cx = bx;

     blockmatrix<double> a(ay,ax), b(by, bx), c(cy, cx);

     for (int i = 0; i < ay; i++) {
        for (int j = 0; j < ax; j++) {
           *a(i,j) = i*j;
        }
     }
    
     for (int i = 0; i < by; i++) {
        for (int j = 0; j < bx; j++) {
           *b(i,j) = i*j;
        }
     }

     long long start = rdtsc();
     c.block_mul(a,b);
     long long end   = rdtsc();

     double sum = 0;
    
     for (int i = 0; i < cy; i++) {
        for (int j = 0; j < cx; j++) {
           sum += *c(i,j);
        }
     }
   
//     printf("blocked matrix sum=%.0lf time=%.6f\n", sum, (end - start)/1000000000.); 
     fprintf(stdout, "%lf\n", (end - start)/1000000000.);
  
     start = rdtsc();
     c.transp_mul(a, b);
     end   = rdtsc();

     sum = 0;   

     for (int i = 0; i < cy; i++) {
        for (int j = 0; j < cx; j++) {
           sum += *c(i,j);
        }
     }

//     printf("transp matrix sum=%.0lf time=%.6f\n", sum, (end - start)/1000000000.);
     fprintf(stdout, "%lf\n", (end - start)/1000000000.);

     start = rdtsc();
     c.mul(a, b);
     end   = rdtsc();

     sum = 0;   

     for (int i = 0; i < cy; i++) {
        for (int j = 0; j < cx; j++) {
           sum += *c(i,j);
        }
     }     

     fprintf(stdout, "%lf\n", (end - start)/1000000000.);

   return 0;
}

