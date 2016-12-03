Mi Proyecto "EQUALIZANDO UNA IMAGEN .BMP"

EL objetivo de esta aplicación es resaltar el contraste al tratar de uniformizar la distribucion de colores de la imagen,
cuyo formato es BMP, este archivo puede ser de 1,8,16 y de 24 bits.

Para compilar este programa deberá bajar los archivos BMP.h, ImageProcessing.h, Facada.h y main.cpp. y una imagen
a su gusto, el cual podria lena.bmp

Para compilarlos desde consola debera escribir la siguiente instruccion:

g++ -std=c++11 BMP.h ImageProcessing.h Facade.h Main.cpp -o a.out

finalmente:
./a.out

Y podra ver los cambios en una nueva imagen con el nombre outlena.bmp
