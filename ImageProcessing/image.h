#ifndef IMAGE_H
#define IMAGE_H

 
template<class T> 
class Image
{
	
	  private:
      
              int fila; // number of rows
              int col; // number of columns 
              T *ptr;

      public:
      
             Image();
             Image(int n){ inicializar(n, n); }
             Image(int m, int n){ inicializar(m, n); }
             
             ~Image();
             Image(const Image& oldImage);
             
             void inicializar(int m, int n){ //It lets us to initialize our image 
   
				 fila = m;
				 col = n;
				 ptr = new T[fila * col];
			 } 
             //void operator=(const Image&);
             T Get(int i, int j) { return *(ptr + i*(col)+j); }  //Returns the pixel value of this position i,j
             void Set(int i, int j, T val)	{ *(ptr + i*(col)+j) = val; }  //Assign a pixel value in the position i,j
             
                  
             //Image operator+(const Image &oldImage);
             //Image operator-(const Image& oldImage);
             
};

#endif
