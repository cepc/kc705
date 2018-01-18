##### Makefile for decode/analysis of CMOS pixel sensor data

CC = g++

ROOT_INCLUDE = `root-config --cflags`
ROOT_LIBS    = `root-config --libs`

FLAGS = -I. $(ROOT_INCLUDE)
LIBS  = $(ROOT_LIBS)

.PHONY: all
all: decode2root pixelana

pixelana: pixelana.o  CMOS.o
	$(CC) -o $@ $(FLAGS) pixelana.o CMOS.o $(LIBS)

decode2root: decode2root.o Decode.o
	$(CC) -o $@ $(FLAGS) decode2root.o Decode.o $(LIBS)

.cc.o:
	$(CC) $(FLAGS) -c $<


clean:
	rm pixelana.o
	rm CMOS.o
	rm decode2root.o
	rm Decode.o
	rm pixelana
	rm decode2root
