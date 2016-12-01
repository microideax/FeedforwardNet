g++ -pthread -std=c++11 -Wall -Wextra -pedantic -I ../ -O3 LeNet-5_demo.cpp -o LeNet_demo `pkg-config --cflags --libs opencv`

#no opencv
g++ -std=c++11 -Wall -Werror -Wextra -pedantic -I ../ -O3 lenet.cpp -o hlstest
