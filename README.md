# Project Title
CP386 Assignment #4
# MakeFile
In Linux, use the following command to compile and link the project:

gcc -std=gnu99 -o allocation allocation.c
# Or
use the renamed makefile (extension removed) provided in the assignment:

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
# ScreenShots
Refer to File screenshots.pdf 
# Individual contributions
All by Raymond Guo 
# Features 
1. Allocate memory to process using first/best/worst fit
2. Release process memory
3. Compact memory space 
4. Report memory space detail 

# Test Case
Refer to File testcase.pdf 
# Use Examples 
Refer to File screenshots.pdf

# About Developers
Name: Raymond Guo 
ID: 160366110

# License 
Course instructor/professor and TA can use the code




