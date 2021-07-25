#!/bin/sh

superfamiconv palette -i sprites/playerpallete.png -M gba -R -C 16 -0 ff00ff -d data/playerpallete.bin -v
superfamiconv palette -i sprites/enemypallete.png -M gba -R -C 16 -0 ff00ff -d data/enemypallete.bin -v


superfamiconv tiles -i sprites/ship.png  -B 4 -D -p data/playerpallete.bin -M gba -H 8 -W 8 -F  -d data/sprite.bin -v
superfamiconv tiles -i sprites/bullet.png -B 4 -D -p data/playerpallete.bin -M gba -H 8 -W 8 -F  -d data/bullet.bin -v
superfamiconv tiles -i sprites/font.png -B 4 -D -p data/enemypallete.bin -M gba -H 8 -W 8 -F -d data/font.bin -v

