#!/bin/sh

#palletes
superfamiconv palette -i sprites/playerpallete.png -M gba  -C 16 -0 ff00ff -d data/playerpallete.bin -v
superfamiconv palette -i sprites/enemypallete.png -M gba  -C 16 -0 ff00ff -d data/enemypallete.bin -v

#tiles
superfamiconv tiles -i sprites/ship.png  -B 4 -D -p data/playerpallete.bin -M gba -H 8 -W 8 -F  -d data/ship.bin -v
superfamiconv tiles -i sprites/shipprop.png  -B 4 -D -p data/playerpallete.bin -M gba -H 8 -W 8 -F  -d data/shipprop.bin -v
superfamiconv tiles -i sprites/bullet.png -B 4 -D -p data/enemypallete.bin -M gba -H 8 -W 8 -F  -d data/bullet.bin -v

superfamiconv tiles -i sprites/underline.png -B 4 -D -p data/playerpallete.bin -M gba -H 8 -W 8 -F  -d data/underline.bin -v
superfamiconv tiles -i sprites/exsplosion.png  -B 4 -D -p data/playerpallete.bin -M gba -H 8 -W 8 -F  -d data/exsplosion.bin -v
superfamiconv tiles -i sprites/font.png -B 4 -D -p data/playerpallete.bin -M gba -H 8 -W 8 -F -d data/font.bin -v
superfamiconv tiles -i sprites/numbers.png -B 4 -D -p data/playerpallete.bin -M gba -H 8 -W 8 -F -d data/numbers.bin -v

#music

sox music/skytheme.wav build/skytheme-build.wav rate 18157
sox build/skytheme-build.wav -b 8 -e signed build/skytheme.raw Channels 1
cd build
raw2gba skytheme.raw
cd ../

#this works^ this might not
#sox music/skytheme.wav rate 18157 -b 8 -e signed buildskytheme.raw Channels 1




#for some reason, raw2gba requires you be inside the directory for it to work otherwise it calls the array build/skytheme#for some reason, raw2gba requires you be inside the directory for it to work otherwise it calls the array build/skytheme
