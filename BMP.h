#ifndef __BMP_H__
#define __BMP_H__

#include <iostream> 
#include <fstream> 
#include <cstring> // memcpy()  
#include <algorithm> 
#include <vector> 
#include <stdio.h>   
#include <cmath> 

int LeerBit(unsigned char valor, int pos){ 
  return ((valor & (int)pow(2, pos))>0 ? 1 : 0); 
} 

int Leer4Bit(unsigned char valor, int pos) { 
  return ((valor >> 4 * pos) & 15);  //Recuerda que 15 es igual  a 1111
} 

int Redondeo(float v) { 
  return (int)( (v - floor(v)) < (ceil(v) - v) ? floor(v) : ceil(v));  //FLoor() retorna el minimo entero, y ceil() el maximo entero de "X"
} 



template<class U> 
class Image
{
	
	  public:
      
              int fila; // number of rows
              int col; // number of columns 
              U *ptr;
 
      
             Image(){};
             Image(int n){ inicializar(n, n); }
             Image(int m, int n){ inicializar(m, n); }
             
             ~Image(){};
             Image(const Image& oldImage);
             
             void inicializar(int m, int n){ //It lets us to initialize our image 
   
				 fila = m;
				 col = n;
				 ptr = new U[fila * col];
			 }  
             U Get(int i, int j) { return *(ptr + i*(col)+j); }  //Returns the pixel value of this position i,j
             void Set(int i, int j, U val)	{ *(ptr + i*(col)+j) = val; }  //Assign a pixel value in the position i,j
             
                   
};



template <class T> 
class BMP_Image : public Image<T>{
  
	public:
	 
		BMP_Image():Image<T>(){} 
		//BMP_Image(int n) { inicializar(n, n); } 
		BMP_Image(int n):Image<T>(n,n){ } 
		
		//BMP_Image(int m, int n)	{ inicializar(m, n); }
		BMP_Image(int m, int n):Image<T>(m,n){ }
  
		 
		void Set_BMP_Image(BMP_Image<T> mat) 
		{ 
			delete Image<T>::ptr; 
			inicializar(mat.fila, mat.col); 
			memcpy(&(Image<T>::ptr[0]), &(mat.Image<T>::ptr[0]), Image<T>::fila*Image<T>::col * sizeof(T)); 
		} 
  
		void Get_BMP_Image(BMP_Image<T> *mat) 
		{ 
			if (mat->Image<T>::fila == Image<T>::fila && mat->Image<T>::col == Image<T>::col) 
			memcpy(&mat->Image<T>::ptr[0], &(Image<T>::ptr[0]), Image<T>::fila*Image<T>::col * sizeof(T)); 
		} 
  
		void Imprimir() 
		{ 
			for (int i = 0; i < Image<T>::fila; i++) { 
				for (int j = 0; j < Image<T>::col; j++) 
					std::cout << Image<T>::ptr[i*Image<T>::col + j]<<" "; 
				std::cout << std::endl; 
			} 
		} 
  
		void InvertirPorFilas() { 
			T *temp; 
			temp = new T[Image<T>::col]; 
			for (int i = 0; i < Image<T>::fila/2; i++) 
			{ 
				memcpy(&(temp[0]), &(Image<T>::ptr[i*Image<T>::col]), Image<T>::col * sizeof(T)); 
				memcpy(&(Image<T>::ptr[i*Image<T>::col]), &(Image<T>::ptr[(Image<T>::fila-i)*Image<T>::col]), Image<T>::col * sizeof(T)); 
				memcpy(&(Image<T>::ptr[(Image<T>::fila-i)*Image<T>::col]), &(temp[0]), Image<T>::col * sizeof(T)); 
			} 
		}
		
		~BMP_Image(){Image<T>::~Image(); } 
}; 


class BMPHeaderFile 
{ 
	private: 
	
		unsigned char tipoArchivo[2];     			// 'BM' 
		unsigned char tamanioArchivo[4];  		    // Tamaño del archivo en bytes 
		unsigned char reservado[4];       			// libres   
		unsigned char offsetDatos[4];     			// File offset to rastes Data 
	
  
	public: 
	 
	    char *GetTipoArchivo() { 
			char *tipo; 
			tipo=new char[3]; 
			tipo[0] = tipoArchivo[0];
			tipo[1] = tipoArchivo[1];
			tipo[2] = '\0'; 
			return tipo;
		} 
		
		int GetTamanioArchivo(){ 
			int temp = 0;
			memcpy(&temp, tamanioArchivo, sizeof(tamanioArchivo));
			return temp;
		} 
		
		int GetReservado(){ 
			int temp = 0;
			memcpy(&temp, reservado, sizeof(reservado));
			return temp;
		} 
		
		int GetOffsetDatos(){
			int temp = 0;
			memcpy(&temp, offsetDatos, sizeof(offsetDatos));
			return temp;
		}	 
  
		void SetTipoArchivo(char *val) {
			tipoArchivo[0] = val[0];
			tipoArchivo[1] = val[1];
		}
		 
		void SetTamanioArchivo(int val){ 
			memcpy(tamanioArchivo, &val, sizeof(tamanioArchivo)); 
		} 
		void SetReservado(int val)     { 
			memcpy(reservado, &val, sizeof(reservado)); 
		} 
		void SetOffsetDatos(int val)   { 
			memcpy(offsetDatos, &val, sizeof(offsetDatos));
		}  
		 
	 
		void visualizar() 
		{		 
			std::cout << "Tipo de Archivo  : " << GetTipoArchivo()<< std::endl;		 
			std::cout << "Tamaño de Archivo: " << GetTamanioArchivo() <<std::endl;		 
			std::cout << "Reservado        : " << GetReservado() << std::endl;		 
		 } 
}; 


class BMPInfoHeader 
{ 
	private: 
		unsigned char tamanio[4];           // Tamaño del Info de la cabecera 40 
		unsigned char ancho[4];             // Ancho del bitmap 
		unsigned char alto[4];              // Alto del bitmap 
		unsigned char planes[2];            // numero de planes = 1 
		unsigned char nroBitsPixel[2];      // Bits por pixel 
		unsigned char compresion[4];        // Tipo de compresion 
		unsigned char tamanioImagen[4];     // Tamaño de la imagen; caso de comprimida 
		unsigned char XpixelPorM[4];        // Resolucion horizontal pixels por metro 
		unsigned char YpixelPorM[4];        // Resolucion vertical pixels por metro 
		unsigned char coloresUsados[4];     // Numero de colores usados 
		unsigned char coloresImportantes[4];// Numero de colores importantes 0 = todos 
  
  
	public: 
	
		int GetTamanio(){ 
			int temp = 0; 
			memcpy(&temp, tamanio, sizeof(tamanio)); 
			return temp; 
		}
    
		int GetAncho(){ 
			int temp = 0; 
			memcpy(&temp, ancho, sizeof(ancho)); 
			return temp; 
		} 
		
		int GetAlto(){ 
			int temp = 0;
			memcpy(&temp, alto, sizeof(alto));
			return temp; 
		} 
		
		int GetPlanes(){ 
			int temp = 0; 
			memcpy(&temp, planes, sizeof(planes)); 
			return temp; 
		} 
		
		int GetNroBitsPixel()      { int temp = 0; memcpy(&temp, nroBitsPixel, sizeof(nroBitsPixel)); return temp; } 
		int GetCompresion()        { int temp = 0; memcpy(&temp, compresion, sizeof(compresion)); return temp; } 
		int GetTamanioImagen()     { int temp = 0; memcpy(&temp, tamanioImagen, sizeof(tamanioImagen)); return temp; } 
		int GetXPixelPorM()        { int temp = 0; memcpy(&temp, XpixelPorM, sizeof(XpixelPorM)); return temp; } 
		int GetYPixelPorM()        { int temp = 0; memcpy(&temp, YpixelPorM, sizeof(YpixelPorM)); return temp; } 
		int GetColoresUsados()     { int temp = 0; memcpy(&temp, coloresUsados, sizeof(coloresUsados)); return temp; } 
		int GetColoresImportantes(){ int temp = 0; memcpy(&temp, coloresImportantes, sizeof(coloresImportantes)); return temp; } 
  
  
	   void SetTamanio(int val)           { memcpy(tamanio, &val, sizeof(tamanio));} 
	   void SetAncho(int val)             { memcpy(ancho, &val, sizeof(ancho)); } 
	   void SetAlto(int val)              { memcpy(alto, &val, sizeof(alto)); } 
	   void SetPlanes(int val)            { memcpy(planes, &val, sizeof(planes));} 
	   void SetNroBitsPixel(int val)      { memcpy(nroBitsPixel, &val, sizeof(nroBitsPixel));} 
	   void SetCompresion(int val)        { memcpy(compresion, &val, sizeof(compresion)); } 
	   void SetTamanioImagen(int val)     { memcpy(tamanioImagen, &val, sizeof(tamanioImagen));} 
	   void SetXPixelPorM(int val)        { memcpy(XpixelPorM, &val, sizeof(XpixelPorM)); } 
	   void SetYPixelPorM(int val)        { memcpy(YpixelPorM, &val, sizeof(YpixelPorM)); } 
	   void SetColoresUsados(int val)     { memcpy(coloresUsados, &val, sizeof(coloresUsados));} 
	   void SetColoresImportantes(int val){ memcpy(coloresImportantes, &val, sizeof(coloresImportantes));} 
	  
	  void visualizar() 
	  { 
	    std::cout << "Tamaño             : " << GetTamanio() << std::endl; 
	    std::cout << "Ancho              : " << GetAncho() << std::endl; 
	    std::cout << "Alto               : " << GetAlto() << std::endl; 
	    std::cout << "Planes             : " << GetPlanes() << std::endl; 
	    std::cout << "Nro Bits por Pixel : " << GetNroBitsPixel() << std::endl; 
	    std::cout << "Compresion         : " << GetCompresion() << std::endl; 
	    std::cout << "Tamaño Imagen      : " << GetTamanioImagen() << std::endl; 
	    std::cout << "X pixel por M      : " << GetXPixelPorM() << std::endl; 
	    std::cout << "Y pixel por M      : " << GetYPixelPorM() << std::endl; 
	  } 
}; 

class BMPColor {
	
	private: 
	
		unsigned char Red;      //channel red 
		unsigned char Green;     // intensidad del verde 
		unsigned char Blue;      // intensidad del azul 
		unsigned char reservado; // sin uso 
	
	
	public: 
	
		int GetRed()       { return (int)Red; } 
		int GetGreen()     { return (int)Green; } 
		int GetBlue()      { return (int)Blue; } 
		int GetReservado() { return (int)reservado; } 
	  
	  
		void SetRed(int val)       { Red = (char)val; } 
		void SetGreen(int val)     { Green = (char)val; } 
		void SetBlue(int val)      { Blue = (char)val; } 
		void SetReservado(int val) { reservado = (char)val; } 
	  
		void visualizar() 
		{ 
			std::cout << "Red             : " << GetRed() << std::endl; 
			std::cout << "Green           : " << GetGreen() << std::endl; 
			std::cout << "Blue            : " << GetBlue() << std::endl; 
			std::cout << "Reservado       : " << GetReservado() << std::endl; 
		} 
	  
		BMPColor& operator = (const BMPColor &bmpc); 
		int operator == (const BMPColor &bmpc); 
}; 

BMPColor& BMPColor::operator = (const BMPColor &bmpc) 
{ 
  if (this != &bmpc) 
    { 
      this->Red = bmpc.Red; 
      this->Green = bmpc.Green; 
      this->Blue = bmpc.Blue; 
      this->reservado = bmpc.reservado; 
    } 
  return *this; 
} 

int BMPColor::operator == (const BMPColor &bmpc) 
{ 
  if (this != &bmpc) 
    { 
      if (this->Red == bmpc.Red && this->Green == bmpc.Green&&this->Blue == bmpc.Blue&&this->reservado == bmpc.reservado) 
	return 1; 
    } 
  return 0; 
} 

#endif