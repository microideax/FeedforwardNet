IDIR =./inference_net
#IDIR +=./inference_net/stb_image 

CC=g++
CFLAGS=-I$(IDIR)

LDIR = /nfs/app/Xilinx/Vivado_HLS/2016.4/include

ff_test:ff_test.cpp FORCE
	$(CC) $(CFLAGS) -I$(LDIR) -g3 -O0 $< -o $@
FORCE:
	

.PHONY: clean

clean:
	rm -f *.o *.txt ff_test
