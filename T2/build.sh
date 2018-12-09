#!/bin/bash

g++ ./lib/btree.h -c btree.cpp
g++ -c main.cpp
g++ -o executar btree.o main.o 