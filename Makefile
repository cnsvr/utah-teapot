all: clean_environment create_object create_image open_image

create_object: raytracer.cpp
								c++ raytracer.cpp -o raytracer.out -std=c++17
create_image: raytracer.out
								./raytracer.out input.txt  >> scene.ppm
open_image: scene.ppm
								open scene.ppm

clean_environment:
								rm -rf *.out
								rm -rf *.ppm
