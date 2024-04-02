name=tmp

all:
	g++-12 ./src/main.cpp -o ./build/run && ./build/run > ./output/$(name).ppm
open:
	$(MAKE) all && imv ./output/$(name).ppm
