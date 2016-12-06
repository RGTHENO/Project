#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdio.h> 
#include <cmath> 
#include <fstream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
 

typedef double T_DOUBLE;
typedef char T_CHAR;
typedef long T_LONG;
typedef float T_FLOAT;
typedef int T_INT;
typedef unsigned char T_BYTE;

const T_LONG BLOQUELINEA = 1024;

using namespace cv;
using namespace std;

#define norm(x, y) (fabs(x) + fabs(y)) 
  
   
template<class T>
class h_Matriz
{

	public:
		T *ptr;
		size_t row, col;
		h_Matriz(){}
		h_Matriz(size_t n)				{ inicializar(n, n); }
		h_Matriz(size_t m, size_t n)	{ inicializar(m, n); }
		h_Matriz(cv::Mat img)
		{
			inicializar(img.rows, img.cols);
			for (int i = 0; i < row; i++)
				memcpy(&(ptr[i*col]), img.ptr<T>(i, 0), col * sizeof(T));
		}
	
		void inicializar(size_t m, size_t n)
		{
			row = m;
			col = n;
			ptr = new T[row * col];
		}

		inline T Get(size_t r, size_t c)			{ return *(ptr + r*(col)+c); }
		inline void Set(size_t r, size_t c, T val)	{ *(ptr + r*(col)+c) = val; }
	
		void Set_Matriz(h_Matriz<T> mat)
		{
			delete ptr;
			inicializar(mat.row, mat.col);
			memcpy(&(ptr[0]), &(mat.ptr[0]), row*col * sizeof(T));
		}
	
		void Get_Matriz(h_Matriz<T> *mat)
		{
			if (mat->row == row && mat->col == col)
				memcpy(&mat->ptr[0], &(ptr[0]), row*col * sizeof(T));
		}
	
		void h_Matriz2Mat(cv::Mat *img)
		{
			if (img->rows == row && img->cols == col)
				for (size_t i = 0; i < row; i++)
					memcpy(img->ptr<T>(i, 0), &(ptr[i*col]), col * sizeof(T));
		}
	
		void Imprimir()
		{
			for (size_t i = 0; i < row; i++)
			{
				for (size_t j = 0; j < col; j++)
					cout << ptr[i*col + j] << "\t";
				cout << endl;
			}
		}
	
		~h_Matriz()
		{
		}
};

// modificar el kernel para convolucion
template<class T>
void convolucion(h_Matriz<T> *kernel)
{
	int r = kernel->row;
	int c = kernel->col;
	h_Matriz<T> temp(r, c);
	for (int k = 0; k < r; k++)
		for (int l = 0; l < c; l++)
			temp.Set(k, l, kernel->Get(r - k - 1, c - l - 1));
	kernel->Set_Matriz(temp);
}

// correlacion, es convolucion si el kernel es modificado para convolucion
template<class T>
void correlacion(h_Matriz<T> *img, h_Matriz<T> *imgout, h_Matriz<T> kernel)
{
	T suma1;
	int mitad = kernel.row / 2;
	for (int i = mitad; i < img->row - mitad; i++)
		for (int j = mitad; j < img->col - mitad; j++)
		{
			suma1 = 0;
			for (int k = 0; k < kernel.row; k++)
				for (int l = 0; l < kernel.col; l++)
					suma1 += img->Get(i - mitad + k, j - mitad + l)*kernel.Get(k, l);
			imgout->Set(i, j, (T)(suma1));
		}
}

// correlacion2 con dos kernel, es convolucion si los kernel es modificado para convolucion
template<class T, class T1>
void correlacion2(h_Matriz<T> *img, h_Matriz<T> *imgout, h_Matriz<T1> kernel, h_Matriz<T1> kernel2)
{
	T suma1;
	T suma2;
	T tmp;
	int mitad = kernel.row / 2;

	for (int i = mitad; i < img->row - mitad; i++)
		for (int j = mitad; j < img->col - mitad; j++){
			suma1 = 0;
			suma2 = 0;
			for (int k = 0; k < kernel.row; k++)
				for (int l = 0; l < kernel.col; l++){
					tmp = img->Get(i - mitad + k, j - mitad + l);
					suma1 += tmp*kernel.Get(k, l);
					suma2 += tmp*kernel2.Get(k, l);
				}
			T val = norm((T)suma1, (T)suma2);
			imgout->Set(i, j, (T)val);
		}
}


int main()
{
		
		Mat cvimagen;
		cvimagen = imread("meme.bmp", 0);
		cvimagen.convertTo(cvimagen, CV_32FC1);
		
		// en HOST
		
		h_Matriz<float> imagen(cvimagen);
		h_Matriz<float> imagenout(imagen.row, imagen.col);
		
		// ahora en la matriz puedo hacer cositas
		//h_Matriz<unsigned char> imagen2(cvimagen);
		
		
		//mediana
		/**
		h_Matriz<float> kernelx(3);
		kernelx.Set(0, 0, 1.0/9);	kernelx.Set(0, 1, 1.0/9);	kernelx.Set(0, 2, 1.0/9);
		kernelx.Set(1, 0, 1.0/9);	kernelx.Set(1, 1, 1.0/9);	kernelx.Set(1, 2, 1.0/9);
		kernelx.Set(2, 0, 1.0/9);	kernelx.Set(2, 1, 1.0/9);	kernelx.Set(2, 2, 1.0/9);
		
		h_Matriz<float> kernely(3);
		kernely.Set(0, 0, -1);	kernely.Set(0, 1, 0);	kernely.Set(0, 2, 1);
		kernely.Set(1, 0, -1);	kernely.Set(1, 1, 0);	kernely.Set(1, 2, 1);
		kernely.Set(2, 0, -1);	kernely.Set(2, 1, 0);	kernely.Set(2, 2, 1);
		
		**/
		//Sobel
		
		h_Matriz<float> kernelx(3);
		kernelx.Set(0, 0, -1);	kernelx.Set(0, 1, 0);	kernelx.Set(0, 2, 1);
		kernelx.Set(1, 0, -2);	kernelx.Set(1, 1, 0);	kernelx.Set(1, 2, 2);
		kernelx.Set(2, 0, -1);	kernelx.Set(2, 1, 0);	kernelx.Set(2, 2, 1);
		
		h_Matriz<float> kernely(3);
		kernely.Set(0, 0, -1);	kernely.Set(0, 1, -2);	kernely.Set(0, 2, -1);
		kernely.Set(1, 0, 0);	kernely.Set(1, 1, 0);	kernely.Set(1, 2, 0);
		kernely.Set(2, 0, 1);	kernely.Set(2, 1, 2);	kernely.Set(2, 2, 1);
		
		
		
		// para cambiar el kernel para obtener kernel para convolution
		
		convolucion(&kernelx);
		convolucion(&kernely);
		
		 
		correlacion2(&imagen, &imagenout, kernelx, kernely);
		//correlacion(&imagen, &imagenout, kernelx);
	 
		imagenout.h_Matriz2Mat(&cvimagen);

		cvimagen.convertTo(cvimagen, CV_8UC1);	
		imshow("mi imagen", cvimagen);

		// Guardar y visualizar la imagen guardada

		imwrite("salida.bmp", cvimagen);
 
		return 0;
}

