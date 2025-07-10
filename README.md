# Wall paper manager

a small c++ script that I use to grab my wall papers

### How to use 

#### Requirements
`curl 8.14.1` *I used these version but it should work for most curl versions*
`clang 20.1.7` *again gcc or mingW **should** work for this but I used clang with compiler set to c++20*

```Bash
touch .config/wallpaper.conf ## make the file probably should add urls to it 
cd wallpaperManager          ## move in to the directory 
make                         ## this makes the binary
./wall_mngr                  ## this will download the wall papers to a .wallpapers directory in your $HOME directory
```

__NOTE__:
For those unfamiliar if you dont have clang make will fail you will need to update the `Makefile` 
to use the compiler of your choice
