#include<iostream>
#include "ImageProcessing.h"

int main(){
	
	  Image_Processing test;
	
	  test.Load("mandrill.bmp");
	  test.Histogram();	 
	  test.Ecualization();  
	  test.Save("equa_lena.bmp");
  	


	return 0;
}