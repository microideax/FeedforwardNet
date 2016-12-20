#!/bin/bash

rm -rf fftest out_conv.txt out_pool.txt out_fc.txt

max=60
for (( i=2; i <= max; ++i))
do
	echo -e "\n"
done


g++ -Wall -Wextra -pedantic -I ../ -O1 ff_test.cpp -o fftest
