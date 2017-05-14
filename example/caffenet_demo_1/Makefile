IDIR =./inference_net
#IDIR +=./inference_net/stb_image 

CC=g++
CFLAGS=-I$(IDIR)

LDIR = /opt/Xilinx/Vivado_HLS/2016.2/include

ff_test:ff_test.cpp FORCE
	$(CC) $(CFLAGS) -I$(LDIR) -O3 $< -o $@
FORCE:
	

.PHONY: clean

clean:
	rm -f *.o *.txt ff_test
