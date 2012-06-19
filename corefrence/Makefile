
all:
	g++ mcmc.cpp -o mcmc -std=c++0x -O3 -g -Wall

debug:
	g++ mcmc.cpp -o mcmc -Wall -g -std=c++0x

fast:
	g++ mcmc.cpp -o mcmc -std=c++0x -funroll-loops -floop-parallelize-all -ftree-parallelize-loops=4 -s -Wall

clang:
	clang++ mcmc.cpp -o mcmc -std=c++0x -Wall -g

clean:
	rm mcmc
	rm a.out
