all: clean_environment create_object create_image open_image

create_object: raytracer.cpp
								c++ raytracer.cpp -o raytracer.o -std=c++17
create_image: raytracer.o
								./raytracer.o input.txt  >> scene.ppm
open_image: scene.ppm
								open scene.ppm

clean_environment:
								rm -rf *.o
								rm -rf *.ppm
