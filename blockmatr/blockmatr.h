#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;


template<class T> class blockmatrix {

public:
   blockmatrix(size_t lines, size_t columns): lines(lines), columns(columns) { // subject to change
      bigbody = new T[lines * columns];
      memset(bigbody, 0, (lines * columns) * sizeof(T));
      body = new T*[lines];
      for (size_t i = 0; i < lines; i++) {
         body[i] = bigbody + i*columns;
      }
   }

   ~blockmatrix() { 
      delete [] body;
      delete [] bigbody;
   }
   
   T * operator()(size_t line, size_t col) { // *acc(0,0) = 2.34; *acc(i,j) = *acc(i,j+1);
      return body[line]+col;
   }
   
   T * operator()(size_t line, size_t col) const { // *acc(0,0) = 2.34; *acc(i,j) = *acc(i,j+1);
      return body[line]+col;
   }

   void print_matrix()
   {
      for (size_t i = 0; i < lines; i++)
      {
         for (size_t j = 0; j < columns; j++)
         {
            cout << body[i][j] << "\t";            
         }
         cout << "\n\n";
      }  
   }

   bool mul(blockmatrix const &left, blockmatrix const &right)  
   {
      rec_mul(bigbody, left.bigbody, right.bigbody, left.lines, left.lines);
      
      return true;
   }
      

      void rec_mul(T* res, T* left_matr, T* right_matr, size_t n, size_t rowsize)
      {
         if (n == 2)
         {
            for (size_t i = 0; i < 2; i++) 
            {
               for (size_t j = 0; j < 2; j++) 
               {
                  T acc = (T)0;
               
                  for (size_t k = 0; k < 2; k++) 
                  {
                     acc += left_matr[i * rowsize + k] * right_matr[j + k * rowsize];
                  }
               
                  res[i * rowsize + j] += acc;
               }
            }          
         }
         else
         {
            size_t displ_11 = 0;
            size_t displ_12 = n/2;
            size_t displ_21 = (n/2) * rowsize;
            size_t displ_22 = (n/2) * (rowsize + 1);

            rec_mul(res + displ_11, left_matr + displ_11, right_matr + displ_11, n/2, rowsize);
            rec_mul(res + displ_11, left_matr + displ_12, right_matr + displ_21, n/2, rowsize);
            rec_mul(res + displ_12, left_matr + displ_11, right_matr + displ_12, n/2, rowsize);               
            rec_mul(res + displ_12, left_matr + displ_12, right_matr + displ_22, n/2, rowsize);
            rec_mul(res + displ_21, left_matr + displ_21, right_matr + displ_11, n/2, rowsize);
            rec_mul(res + displ_21, left_matr + displ_22, right_matr + displ_21, n/2, rowsize);
            rec_mul(res + displ_22, left_matr + displ_21, right_matr + displ_12, n/2, rowsize);
            rec_mul(res + displ_22, left_matr + displ_22, right_matr + displ_22, n/2, rowsize);
         }
      }

   bool add(blockmatrix const &left, blockmatrix const &right); 
   static void tune(size_t arg) { // subject for change
      ;
   }
private:
   T ** body;
   T *bigbody;
   size_t lines, columns;
};

