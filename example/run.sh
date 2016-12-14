#!/bin/bash

rm -rf fftest

max=60
for (( i=2; i <= max; ++i))
do
	echo -e "\n"
done


g++ -std=c++0x -Wall -Wextra -pedantic -I ../ -O1 ff_test.cpp -o fftest
