#!/bin/sh
day=${PWD##*/}
g++ main.cpp -o $day -O3 -g -march=native -pedantic -Wall -Wextra
