#!/bin/bash
gcc -E main.c -o main.i 
gcc -S main.i -o main.s

