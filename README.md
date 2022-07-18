# Project Title
CP386 Assignment #4
# MakeFile
#Do not edit the contents of this file

CC = gcc

CFLAGS = -std=gnu99

TARGET = allocation  
OBJFILES = allocation.o 
 
all: $(TARGET)

allocation: allocation.c

	$(CC) $(CFLAGS) -o allocation allocation.c
	
run: allocation
	./allocation 1048576
  
clean:

	rm -f $(OBJFILES) $(TARGET) *~ 
