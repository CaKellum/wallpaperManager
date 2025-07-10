# Wall paper manager

a small c++ script that I use to grab my wall papers

I did not test on windows nor do I care to

#### Requirements
`make` <br> 
`curl`  <br>
`clang` *gcc or mingW **should** work for this just edit the Makefile*  <br>

## How to use

```Bash
touch .config/wallpaper.conf ## make the file probably should add urls to it 
cd wallpaperManager          ## move in to the directory 
make                         ## this makes the binary
./wall_mngr                  ## download the wall papers to $HOME/.wallpapers
```
