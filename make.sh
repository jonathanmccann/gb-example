#!/bin/bash
# $1 is the location of 'lcc'
# $2 is the location of the emulator

cd src

rm main.gb

$1 -o main.gb main.c enemy.c player.c sprite_and_background.c

wine $2 main.gb > /dev/null 2>&1