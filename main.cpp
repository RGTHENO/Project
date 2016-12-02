#include "BMP.h" 

using namespace std;

BMPHeaderFile bmpCA; 
BMPInfoHeader bmpCI; 


BMPColor *bmpPaleta; 

BMP_Image <BMPColor> mat; 
BMP_Image <float> histogramaR; 
BMP_Image <float> histogramaG; 
BMP_Image <float> histogramaB; 

void getPalette(char *FileOrigen,int nc) 
{ 
	std::ifstream origen(FileOrigen, std::ios::binary); 
	if (origen.fail()) 
		std::cerr << "Error al abrir el archivo: " << FileOrigen << std::endl; 
	else 
    {	 
		origen.seekg(54); //donde esta la informacion de la paleta;		 
		//Leer la tabla de color			 
		bmpPaleta = new BMPColor[nc]; 		 
		for (int i = 0; i < nc; i++) 
		{ 
			origen.read((char *)&bmpPaleta[i], sizeof(BMPColor)); 
			bmpPaleta[i].visualizar(); 
		} 
    } 
} 

// Retorna el indice en base al color de la paleta 
int getPaletteID(BMPColor color, int tp) 
{ 
  for (int i = 0; i < tp; i++) 
    if (bmpPaleta[i] == color) 
      return i; 
  return -1; 
} 

void Load(char* FileOrigen) {
  
  std::ifstream origen(FileOrigen, std::ios::binary);	 
  
  if (origen.fail()) 
		std::cerr << "Error al abrir el archivo: " << FileOrigen << std::endl; 
  
  else 
  {			 
      //Leer cabecera de Archivo 
      origen.read((char *)&bmpCA,sizeof(BMPHeaderFile));		 
      if (strcmp(bmpCA.GetTipoArchivo(), "BM") != 0) 
	  { 
	  std::cout << "No es un Archivo BMP.... : " << FileOrigen << std::endl; 
	  getchar(); 
	  exit; 
	} 
      bmpCA.visualizar(); 
      
      
      //Leer Info de la cabecera 
      origen.read((char *)&bmpCI, sizeof(BMPInfoHeader));
      bmpCI.visualizar();	 
      
      mat.inicializar(bmpCI.GetAlto(), bmpCI.GetAncho()); 
      
      std::cout << "Total tamaño de la cabecera: " << sizeof(BMPHeaderFile) + sizeof(BMPInfoHeader)<<" bytes"<<std::endl; 
      
      unsigned char dato; 
      
      switch (bmpCI.GetNroBitsPixel()) 
	{ 
	case 1: 
	  { 
	    int nc = 2; 
	    getPalette(FileOrigen, nc);					 
	    origen.seekg(bmpCA.GetOffsetDatos()); // hasta donde esta el contenido del archivo; 
	    
	    for (int i = 0; i < mat.col*mat.fila;) 
	      { 
		if (i % 8 == 0)	origen.read((char *)&dato, sizeof(unsigned char)); 
		for (int j = 7; j >= 0; j--, i++)					 
		  mat.ptr[i] = bmpPaleta[LeerBit(dato, j)]; 
	      } 
	    
	    break; 
	  }  
	case 4: 
	  {	 
	    // Leer la tabla de color			 
	    int nc = 16; 
	    getPalette(FileOrigen, nc); 
	    origen.seekg(bmpCA.GetOffsetDatos()); // hasta donde esta el contenido del archivo; 
	    for (int i = 0; i < (mat.col*mat.fila) ; i=i+2) 
	      {			 
		origen.read((char *)&dato, sizeof(unsigned char)); 
		mat.ptr[i] = bmpPaleta[Leer4Bit(dato, 1)]; 
		mat.ptr[i+1] = bmpPaleta[Leer4Bit(dato, 0)]; 
	      }						 
	    break; 
	  } 
	case 8: 
	  { 
	    // Leer la tabla de color			 
	    int nc = 256; 
	    getPalette(FileOrigen, nc);		 
	    origen.seekg(bmpCA.GetOffsetDatos()); // hasta donde esta el contenido del archivo; 
	    for (int i = 0; i < mat.col*mat.fila;i++) 
	      { 
		origen.read((char *)&dato, sizeof(unsigned char));				 
		mat.ptr[i] = bmpPaleta[(int)(dato)]; 
	      }			 
	    break;		 
	  } 
	case 24: 
	  { 
	    origen.seekg(bmpCA.GetOffsetDatos()); // hasta donde esta el contenido del archivo; 
	    unsigned char dato3[3];			 
	    for (int i = 0; i < mat.col*mat.fila; i++) 
	      { 
		origen.read((char *)&dato3, sizeof(unsigned char)*3);				 
		mat.ptr[i].SetRed(dato3[0]) ; 
		mat.ptr[i].SetGreen(dato3[1]); 
		mat.ptr[i].SetBlue(dato3[2]); 
		mat.ptr[i].SetReservado(0) ; 
	      } 
	    break; 
	  } 
	default: 
	  break; 
	} 
      
      mat.InvertirPorFilas(); 
      origen.close(); 
    } 
} 

void Save(char *FileDestino) 
{ 
	std::ofstream destino(FileDestino, std::ios::binary); 
  
	if (destino.fail()) 
		std::cerr << "Error al abrir el archivo: " << FileDestino << std::endl; 
	else 
	{ 
		mat.InvertirPorFilas(); 
		if (bmpCI.GetNroBitsPixel() == 24) // convertir imagenes a 24 bists de versiones recientes a una antigua.... 
		{ 
		  bmpCA.SetOffsetDatos(54); 
		  bmpCA.SetTamanioArchivo(54 + bmpCI.GetTamanioImagen()); 
		  bmpCI.SetTamanio(40); 
		} 
		//Escribir cabecera de Archivo 
		destino.write((char *)&bmpCA, sizeof(BMPHeaderFile)); 
		bmpCA.visualizar(); 
      
		//Guardar Info de la cabecera 
		destino.write((char *)&bmpCI, sizeof(BMPInfoHeader)); 
		bmpCI.visualizar(); 
      
      
		unsigned char dato; 
      
		switch (bmpCI.GetNroBitsPixel()) 
		{ 
			case 1: 
			{ 
				int nc = 2; 
			   // guardarPaleta 
				destino.write((char *)&bmpPaleta[0], nc*sizeof(BMPColor));			 
 
				for(int i = 0; i < mat.col*mat.fila;) 
				{ 
					dato = 0; 
					for (int j = 7; j >= 0; j--, i++) 
					dato = mat.ptr[i].GetRed()==255? dato + (unsigned char)pow(2,j):dato;												 
					destino.write((char *)&dato, sizeof(unsigned char));				 
				} 
				break; 
			} 
	
			case 4: 
			{						 
				int nc = 16; 
				// guardarPaleta 
				destino.write((char *)&bmpPaleta[0], nc*sizeof(BMPColor)); 
	    
				for(int i = 0; i < (mat.col*mat.fila); i = i + 2) 
				{				 
					dato = (unsigned char)getPaletteID(mat.ptr[i], nc); 
					dato = dato << 4; 
					dato += (unsigned char)getPaletteID(mat.ptr[i+1], nc);				 
					destino.write((char *)&dato, sizeof(unsigned char)); 
				} 
				break; 
			} 
			case 8: 
			{ 
				// Leer la tabla de color			 
				int nc = 256; 
				// guardarPaleta 
	    
				destino.write((char *)&bmpPaleta[0], nc*sizeof(BMPColor)); 
	    
				for(int i = 0; i < mat.col*mat.fila; i++) 
				{ 
					dato = (unsigned char)getPaletteID(mat.ptr[i], nc); 
					destino.write((char *)&dato, sizeof(unsigned char));				 
				} 
				break; 
			} 
			case 24: 
			{			 
				unsigned char dato3[3]; 
				for(int i = 0; i < mat.col*mat.fila; i++) 
				{ 
					dato3[0]=mat.ptr[i].GetRed(); 
					dato3[1]=mat.ptr[i].GetGreen(); 
					dato3[2]=mat.ptr[i].GetBlue();				 
					destino.write((char *)&dato3, sizeof(unsigned char) * 3);				 
				} 
				break; 
			} 
			default: 
				break; 
		}//fin switch-case	 
      
      destino.close(); 
	} // fin else
} 


void Histogram() {
  
  int opc = bmpCI.GetNroBitsPixel();
  
  if(opc <= 8){
    
    opc = (int)(1<<opc);
    histogramaR.inicializar(opc, 4);
    for (int i = 0; i < opc; i++) { 
		histogramaR.Set(i, 0, 0);	histogramaR.Set(i, 1, 0); 
    }

    int idx;
    float size = mat.col*mat.fila;
  
    for (int i = 0; i < size; i++) { 
      idx = getPaletteID(mat.ptr[i], opc); 
      histogramaR.Set(idx, 0, histogramaR.Get(idx, 0) + 1); 
    } 
    
    for (int i = 0; i < opc; i++) 
      histogramaR.Set(i, 1, histogramaR.Get(i, 0) / size); 
    
    /*transformation of equalization */
    histogramaR.Set(0, 2, (opc - 1)*histogramaR.Get(0, 1)); 
    
    float v; 
    v = histogramaR.Get(0, 2); 
    histogramaR.Set(0, 3, Redondeo(v)); 
    
    for (int i = 1; i < opc; i++) { 
      histogramaR.Set(i, 2, histogramaR.Get(i - 1, 2) + (opc - 1)*histogramaR.Get(i, 1)); 
      v = histogramaR.Get(i, 2); 
      histogramaR.Set(i, 3, Redondeo(v)); 
    } 
    
    histogramaR.Imprimir();
    
  } else {

	  for (int c = 0; c < 3; c++) {  // channels
	      opc = 256; 
	      histogramaR.inicializar(opc, 4); // R 
	      histogramaG.inicializar(opc, 4); // G
	      histogramaB.inicializar(opc, 4); // B
	      
		  for (int i = 0; i < opc; i++) {
				histogramaR.Set(i, 0, 0);  histogramaR.Set(i, 1, 0); 
				histogramaG.Set(i, 0, 0);  histogramaG.Set(i, 1, 0); 
				histogramaB.Set(i, 0, 0);  histogramaB.Set(i, 1, 0); 
		  }
      
		 int idx;
		 float size = mat.col*mat.fila;
      
		 for(int i = 0; i < size; i++) { 
			idx = mat.ptr[i].GetRed(); 
			histogramaR.Set(idx, 0, histogramaR.Get(idx, 0) + 1); 
			idx = mat.ptr[i].GetGreen(); 
			histogramaG.Set(idx, 0, histogramaG.Get(idx, 0) + 1); 
			idx = mat.ptr[i].GetBlue(); 
			histogramaB.Set(idx, 0, histogramaB.Get(idx, 0) + 1); 
		 } 
	    
		for (int i = 0; i < opc; i++) { 
			histogramaR.Set(i, 1, histogramaR.Get(i, 0) / size); 
			histogramaG.Set(i, 1, histogramaG.Get(i, 0) / size); 
			histogramaB.Set(i, 1, histogramaB.Get(i, 0) / size); 
		} 

		histogramaR.Set(0, 2, (opc - 1)*histogramaR.Get(0, 1)); 
		histogramaG.Set(0, 2, (opc - 1)*histogramaG.Get(0, 1)); 
		histogramaB.Set(0, 2, (opc - 1)*histogramaB.Get(0, 1)); 
      
      
		float v; 
		v = histogramaR.Get(0, 2); 
		histogramaR.Set(0, 3, Redondeo(v)); 
		v = histogramaG.Get(0, 2); 
		histogramaG.Set(0, 3, Redondeo(v)); 
		v = histogramaB.Get(0, 2); 
		histogramaB.Set(0, 3, Redondeo(v)); 
      
		for (int i = 1; i < opc; i++) { 
			histogramaR.Set(i, 2, histogramaR.Get(i - 1, 2) + (opc - 1)*histogramaR.Get(i, 1)); 
			v = histogramaR.Get(i, 2); histogramaR.Set(i, 3, Redondeo(v)); 
	
			histogramaG.Set(i, 2, histogramaG.Get(i - 1, 2) + (opc - 1)*histogramaG.Get(i, 1)); 
			v = histogramaG.Get(i, 2);	histogramaG.Set(i, 3, Redondeo(v)); 
	
			histogramaB.Set(i, 2, histogramaB.Get(i - 1, 2) + (opc - 1)*histogramaB.Get(i, 1)); 
			v = histogramaB.Get(i, 2);	histogramaB.Set(i, 3, Redondeo(v)); 
      } 
      
    }
  }
} 

/* Histogram Equalization*/
void Ecualization()  {
  
  int opc = bmpCI.GetNroBitsPixel(); 
  opc = (int)(1<<opc);
  int idx; 
  float size = mat.col*mat.fila;
  
  if(opc <= 8){
    for (int i = 0; i < size; i++) { 
      idx = getPaletteID(mat.ptr[i], opc); 
      mat.ptr[i] = bmpPaleta[(int)histogramaR.Get(idx, 3)]; 
    }
  } else {
    for (int i = 0; i < size; i++) { 
	idx = mat.ptr[i].GetRed(); 
	mat.ptr[i].SetRed((int)histogramaR.Get(idx, 3)); 
	idx = mat.ptr[i].GetGreen(); 
	mat.ptr[i].SetGreen((int)histogramaG.Get(idx, 3)); 
	idx = mat.ptr[i].GetBlue(); 
	mat.ptr[i].SetBlue((int)histogramaB.Get(idx, 3));
      }
  }
  
}

int main(){

 
  Load("lena.bmp");
  Histogram();	 
  Ecualization();  
  Save("equa_lena.bmp");
  
  
  return 0; 
} 

