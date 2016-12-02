#include "ImageProcessing.h"
#include<string>
class Facade{
	
	public:
		Image_Processing test;
	
		Facade(char name_file[]){
			  test.Load(name_file);
			  test.Histogram();	 
			  test.Ecualization();  
			  test.Save(generate_name(name_file));
	   	
	  }
	  
	  char* generate_name(char name[]){
			  static char new_name[50]= "out";
			  int j = 3;
			  int i =0;
			  
			  while(*(name+i)){
				  new_name[j]=name[i];
				  i++;j++;
			  }
			  new_name[j]='\0';
			  
			  return new_name;
	  }
	 
};