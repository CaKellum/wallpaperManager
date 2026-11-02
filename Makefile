.SILENT:

APP = wall_mngr
INSTALL_LOCATION = ~/.scripts/

default: build

build: clean
	clang++ -std=c++20 -o $(APP) main.cpp -l curl

clean:
	rm -rf $(APP)

install: build
	mv $(APP) $(INSTALL_LOCATION)


help:
	printf "options are as follows:\n"
	printf "\t\e[32mbuild\e[0m: makes the executable $(APP)\n"
	printf "\t\e[31mclean\e[0m: removes the local build\n"
	printf "\t\e[34minstall\e[0m: moves the executable to $(INSTALL_LOCATION)\n"
	printf "It will default to the \e[32mbuild\e[0m\n"

.PHONEY: build clean install
