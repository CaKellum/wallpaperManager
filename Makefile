default: build

build: clean
	clang++ -std=c++20 -o wall_mngr main.cpp -l curl

clean:
	rm -rf wall_mngr
