#!/bin/bash
gcc -c -fPIC -o main.o main.c
gcc -shared -o main.so main.o
