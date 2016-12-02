#include<iostream>
#include "ImageProcessing.h"

int main(){
	
	  Image_Processing test;
	
	  test.Load("lena.bmp");
	  test.Histogram();	 
	  test.Ecualization();  
	  test.Save("output.bmp");
  	


	return 0;
}