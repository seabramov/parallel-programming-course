#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;


template<class T> class blockmatrix 
{
    public:
        blockmatrix(size_t lines, size_t columns): lines(lines), columns(columns) 
        { 
            bigbody = new T[lines * columns];
            memset(bigbody, 0, (lines * columns) * sizeof(T));
            body = new T*[lines];
          
            for (size_t i = 0; i < lines; i++) 
            {
                body[i] = bigbody + i*columns;
            }
        }

       ~blockmatrix() 
        { 
            delete [] body;
            delete [] bigbody;
        }
       
        T * operator()(size_t line, size_t col) 
        { 
            return body[line]+col;
        }
       
        T * operator()(size_t line, size_t col) const 
        { 
            return body[line]+col;
        }

        bool ordinary_mult(T* res, T* left_matr, T* right_matr, size_t n, size_t rowsize)
        {
                T* tr = new T [n * n];

                for (size_t i = 0; i < n; i++)
                {
                    for (size_t j = 0; j < n; j++)
                        tr[i * n + j] = right_matr[i + j * rowsize]; 
                }

                for (size_t i = 0; i < n; i++) 
                {
                    for (size_t j = 0; j < n; j++) 
                    {
                        T acc = (T)0;
                   
                        for (size_t k = 0; k < n; k++) 
                        {
                            acc += left_matr[i * rowsize + k] * tr[j * n + k];
                        }
                   
                        res[i * rowsize + j] += acc;
                    }
                }             

                delete [] tr; 
        }


        bool mul(blockmatrix const &left, blockmatrix const &right) 
        {
            if (left.columns != right.lines) 
                return false;
            if (lines != left.lines) 
                return false;
            if (columns != right.columns) 
                return false;
            
            for (size_t i = 0; i < lines; i++) 
            {
                 for (size_t j = 0; j < columns; j++) 
                 {
                    T acc = (T)0;
                    
                    for (size_t k = 0; k < left.columns; k++) 
                    {
                        acc += left.body[i][k] * right.body[k][j];
                    }
                    
                    body[i][j] = acc;
                }
            }
            
            return true;
        }
            
        bool block_mul(blockmatrix const &left, blockmatrix const &right)  
        {
            if (left.columns != right.lines) 
                return false;
            if (lines != left.lines) 
                return false;
            if (columns != right.columns) 
                return false;

            blocked(bigbody, left.bigbody, right.bigbody, left.lines, left.lines);
            return true;
        }        

        /* NOTE: Basic block size for block mult is 128x128 */
        void blocked(T* res, T* left_matr, T* right_matr, size_t n, size_t size)
        {
            if (n == 128)
            {   
                ordinary_mult(res, left_matr, right_matr, n, size);
            }
            else
            {
                size_t displ_11 = 0;
                size_t displ_12 = n/2;
                size_t displ_21 = (n/2) * size;
                size_t displ_22 = (n/2) * (size + 1);

                blocked(res + displ_11, left_matr + displ_11, right_matr + displ_11, n/2, size);
                blocked(res + displ_11, left_matr + displ_12, right_matr + displ_21, n/2, size);
                blocked(res + displ_12, left_matr + displ_11, right_matr + displ_12, n/2, size);               
                blocked(res + displ_12, left_matr + displ_12, right_matr + displ_22, n/2, size);
                blocked(res + displ_21, left_matr + displ_21, right_matr + displ_11, n/2, size);
                blocked(res + displ_21, left_matr + displ_22, right_matr + displ_21, n/2, size);
                blocked(res + displ_22, left_matr + displ_21, right_matr + displ_12, n/2, size);
                blocked(res + displ_22, left_matr + displ_22, right_matr + displ_22, n/2, size);
            }
        }

        bool transp_mul(blockmatrix const &left, blockmatrix const &right)
        {
            if (left.columns != right.lines) 
                return false;
            if (lines != left.lines) 
                return false;
            if (columns != right.columns) 
                return false;

            size_t tr_size = right.lines + right.columns;   
            T** transp = new T* [right.columns];

            for (size_t i = 0; i < right.columns; i ++)
            {
                transp[i] = new T [right.lines];
            }

            for (size_t i = 0; i < right.lines; i++)
            {  
                for (size_t j = 0; j < right.columns; j++)
                {
                    transp[i][j] = right.body[j][i];            
                }
            }

            for (size_t i = 0; i < lines; i++)
            {  
                for (size_t j = 0; j < columns; j++)
                {
                    T acc = (T)0;

                    for (size_t k = 0; k < left.columns; k++)
                    {
                        acc += left.body[i][k] * transp[j][k];
                    }
            
                    body[i][j] = acc;
                }
            }      

            delete [] transp;
        }                


        bool add(blockmatrix const &left, blockmatrix const &right); 
        
        static void tune(size_t arg)    // NOTE: Possible functionality of this
        {                               // function was implemented in transp_mul()
          ;
        }
    
    private:
        T ** body;
        T *bigbody;
        size_t lines, columns;
};

