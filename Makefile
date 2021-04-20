all: create_ppm create_image open_image

create_ppm: raytracer.cpp
								clang++ raytracer.cpp -o raytracer.o -std=c++17 -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk
create_image: raytracer.o
								./raytracer.o input.txt  >> image.ppm
open_image: image.ppm
								open image.ppm

clean:
								rm *.o
								rm *.ppm
