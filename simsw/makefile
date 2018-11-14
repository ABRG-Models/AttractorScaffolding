all:
	g++ -o model_nd -g -O3 -Wall model.cpp modelfn.cpp
	g++ -o model -DDRIFT -g -O3 -Wall model.cpp modelfn.cpp
	g++ -o bitflips -g -O3 bitflips.cpp modelfn.cpp
	g++ -o showfitness -g showfitness.cpp modelfn.cpp
	g++ -o makefitness -g makefitness.cpp modelfn.cpp
