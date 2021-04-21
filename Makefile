all: create_ppm create_image open_image

create_ppm: raytracer.cpp
								c++ raytracer.cpp -o raytracer.o -std=c++17
create_image: raytracer.o
								./raytracer.o input.txt  >> scene.ppm
open_image: image.ppm
								open image.ppm

clean:
								rm *.o
								rm *.ppm
