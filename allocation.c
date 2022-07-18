/*
 -------------------------------------
 File: 160366110_allocation.c
 Project: A04
 file description:contiguous memory allocation simulation
 -------------------------------------
 Author:  Raymond Guo
 ID:      160366110
 Email:   guox6611@mylaurier.ca
 Version  2022-07-
 -------------------------------------
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#define MAX_LIMIT 300	//length of the command is assumed shorter than 300

void func(char *str);

typedef struct hole //Define a structure data type for memory hole
{
	int start_address;
	int finish_address;
	int size;
	struct hole *next;
} hole_t;

typedef struct process //Define a structure data type for process
{
	int start_address;
	int finish_address;
	int size;
	char pid[10];	//assume the process ID is less than 10 digits in length
	struct process *next;
} process_t;

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Input file name missing...exiting with error code -1\n");
		return -1;
	}
	int MAX;
	MAX = atoi(argv[1]);

	char str[MAX_LIMIT];
	char *cmd[1000];
	printf("Allocated %d bytes of memory\n", MAX);
	while (1) {
		printf("Allocator>");
		fgets(str, MAX_LIMIT, stdin);

		char *str2;
		char *str3;
		str3 = NULL;

		str3 = strtok(str, "\r\n");
		while (str3 != NULL) {
			str2 = malloc(sizeof(str3) * sizeof(char));
			strcpy(str2, str3);
			str3 = strtok(NULL, "\r\n");
		}

		int count = 0;

		char *token = NULL;
		token = strtok(str2, " ");

		while (token != NULL) {
			cmd[count] = malloc(sizeof(token) * sizeof(char));

			strcpy(cmd[count], token);
			//printf("cmd= %s ", cmd[count]);
			count++;
			token = strtok(NULL, " ");
		}

		char *str1 = NULL;
		str1 = malloc(sizeof(cmd[0]) * sizeof(char));
		strcpy(str1, cmd[0]);
		func(str1);	//convert to lowercase string
		char *pid1 = NULL;
		int size1;

		if (strcmp(str1, "rq") == 0 || strcmp(str1, "status") == 0
				|| strcmp(str1, "rl") == 0 || strcmp(str1, "c") == 0) {

			if (strcmp(str1, "rq") == 0) {		//request memory
				printf("command is %s \n", str1);
				pid1 = malloc(sizeof(cmd[1]) * sizeof(char));
				strcpy(pid1, cmd[1]);
				size1 = atoi(cmd[2]);
				char *str4 = NULL;
				str4 = malloc(sizeof(cmd[3]) * sizeof(char));
				strcpy(str4, cmd[3]);
				func(str4);
				if (sizeof(cmd[1]) > 10 || sizeof(cmd[1]) == 0 || size1 <= 0
						|| size1 >= MAX) {
					printf(
							"\nProgram Terminated due to incorrect process ID or process size inputs\n\n");
					exit(-1);
				}

				if (strcmp(str4, "b") == 0 || strcmp(str4, "f") == 0
						|| strcmp(str4, "w") == 0) {

					if (strcmp(str4, "b") == 0) {	//best fit
						printf("command is %s  %s %d %s \n", str1, pid1, size1,
								str4);
						//	best_fit(size1);
					} else if (strcmp(str4, "f") == 0) {	//first fit
						printf("command is %s  %s %d %s \n", str1, pid1, size1,
								str4);
						//first_fit(size1);
					} else {
						printf("command is %s  %s %d %s \n", str1, pid1, size1,
								str4);
						//worst_fit(size1);
					}
				} else {
					printf(
							"\nProgram Terminated due to incorrect input for fit algorithm\n\n");
					exit(-1);
				}

			} else if (strcmp(str1, "rl") == 0) {
				printf("command is %s \n", str1);

			} else if (strcmp(str1, "c") == 0) {
				printf("command is %s \n", str1);

			} else if (strcmp(str1, "status") == 0) {
				printf("command is %s \n", str1);

			}

			//printf("Keep on going");
		} else if (strcmp(str1, "exit") == 0) {
			printf("\nProgram Terminated per User Request\n");
			exit(0);
		} else {
			printf("\nProgram Terminated due to incorrect input\n\n");
			exit(-1);
		}

	}

	return 0;
}

void func(char *str) {

	for (int i = 0; i <= strlen(str); i++) {
		if (str[i] >= 65 && str[i] <= 90)
			str[i] = str[i] + 32;
	}

}
