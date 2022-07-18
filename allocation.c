/*
 -------------------------------------
 File: 160366110_allocation.c
 Project: A04
 file description:contiguous memory allocation simulation
 -------------------------------------
 Author:  Raymond Guo
 ID:      160366110
 Email:   guox6611@mylaurier.ca
 GitHub Login ID: Raymond-Guo567
 URL of GitHub Repository: https://github.com/Raymond-Guo567/CP386_Assignment_4
 Version  2022-07-17
 -------------------------------------
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#define MAX_LIMIT 300	//length of the command is assumed shorter than 300

typedef struct hole //Define a structure data type for memory hole
{
	int start_address;
	//int finish_address;
	int size;
	struct hole *next;
} hole_t;

typedef struct process //Define a structure data type for process
{
	int start_address;
	//int finish_address;
	int size;
	char pid[10];	//assume the process ID is less than 10 digits in length
	struct process *next;
} process_t;

void func(char *str);	//function converting string to lowercase string

void touppercase(char *str);  //function converting string to UPPERCASE STRING

int first_fit(char *pid1, int size1, hole_t **hole_head,
		process_t **process_head);		//first fit
int best_fit(char *pid1, int size1, hole_t **hole_head,
		process_t **process_head, int initial_delta); 	//best fit
int worst_fit(char *pid1, int size1, hole_t **hole_head,
		process_t **process_head);	//worst fit

int clean_empty_hole(hole_t **hole_head); //if a hole has zero length, clean it from the hole list

void display(hole_t **hole_head, process_t **process_head);	//function for "status"

void process_insert(process_t **process_head, int start_address, char *pid1,
		int size1);	//insert a new process to the process list, maintain ascending order in memory address

int length(hole_t **hole_head);	//check hole count in the hole list

int length1(process_t **process_head);//check the process count in the process list

/*release the memory of a process, and add the memory space to hole list */
int release(hole_t **hole_head, process_t **process_head, char *pid1);

/*insert the released process memory to hole list, and maintain the ascending order in memory address */
void hole_insert(hole_t **hole_head, int start_address, int size1);

/* if holes have contiguous address, combine the holes to one big hole */
void hole_combination(hole_t **hole_head);

/*compact memory area, processes to lower address, hole to high */
int compaction(hole_t **hole_head, process_t **process_head);

/**
 * Main progam
 * @param argc
 * @param argv
 * @return -1: no argument input
 */
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

	hole_t *hole_head = NULL;
	hole_head = (hole_t*) malloc(sizeof(hole_t));
	if (hole_head == NULL) {
		return 1;
	}
	hole_head->start_address = 0;
	hole_head->size = MAX;
	hole_head->next = NULL;

	process_t *process_head = NULL;

	while (1) {		// program can only be terminated with "exit" input
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
			count++;
			token = strtok(NULL, " ");
		}
		char *str1 = NULL;
		str1 = malloc(sizeof(cmd[0]) * sizeof(char));
		strcpy(str1, cmd[0]);
		//func(str1);	//convert to lowercase string
		touppercase(str1);
		char *pid1 = NULL;
		int size1;

		if (strcmp(str1, "RQ") == 0 || strcmp(str1, "STATUS") == 0
				|| strcmp(str1, "RL") == 0 || strcmp(str1, "C") == 0) {
			if (strcmp(str1, "RQ") == 0) {		//request memory
				//printf("command is %s \n", str1);
				if (count != 4) {
					printf("Incorrect input for request memory\n");
				} else {
					pid1 = malloc(sizeof(cmd[1]) * sizeof(char));
					strcpy(pid1, cmd[1]);
					size1 = atoi(cmd[2]);
					char *str4 = NULL;
					str4 = malloc(sizeof(cmd[3]) * sizeof(char));
					strcpy(str4, cmd[3]);
					touppercase(str4);
					touppercase(pid1);
					if (sizeof(cmd[1]) > 10 || sizeof(cmd[1]) == 0 || size1 <= 0
							|| size1 >= MAX) {
						printf(
								"\nProgram Terminated due to incorrect process ID or process size inputs\n\n");
						exit(-1);
					}
					if (strcmp(str4, "B") == 0 || strcmp(str4, "F") == 0
							|| strcmp(str4, "W") == 0) {
						if (strcmp(str4, "B") == 0) {	//best fit
							//printf("command is %s  %s %d %s \n", str1, pid1, size1,	str4);
							int a = best_fit(pid1, size1, &hole_head,
									&process_head, MAX);
							if (a == -1) {
								printf("No hoe of sufficient size\n");
							} else {
								printf(
										"Sucessfully allocated %d to process %s\n",
										size1, pid1);
							}
						} else if (strcmp(str4, "F") == 0) {	//first fit
							int a = first_fit(pid1, size1, &hole_head,
									&process_head);
							if (a == -1) {
								printf("No hole of sufficient size\n");
							} else {
								printf(
										"Sucessfully allocated %d to process %s\n",
										size1, pid1);
							}
						} else {	//worst_fit
							//printf("command is %s  %s %d %s \n", str1, pid1, size1, str4);
							int a = worst_fit(pid1, size1, &hole_head,
									&process_head);
							if (a == -1) {
								printf("No hole of sufficient size\n");
							} else {
								printf(
										"Sucessfully allocated %d to process %s\n",
										size1, pid1);
							}
						}
					} else {
						printf(
								"\nAlgorithm selection is incorrect, check and input again!\n\n");
						//exit(-1);
					}
				}
			} else if (strcmp(str1, "RL") == 0) {
				if (count != 2) {
					printf("Incorrect input for release memory\n");
				} else {
					pid1 = malloc(sizeof(cmd[1]) * sizeof(char));
					strcpy(pid1, cmd[1]);
					touppercase(pid1);
					if (sizeof(cmd[1]) > 10) {
						printf(
								"\nIncorrect process ID or process size inputs, check and input again!\n\n");
						//exit(-1);
					}
					int a = release(&hole_head, &process_head, pid1);
					if (a == -1) {
						printf("could find process %s from the process list\n",
								pid1);
					} else {
						printf("Successfully released memory for process %s\n",
								pid1);
					}
				}
			} else if (strcmp(str1, "C") == 0) {
				//printf("command is %s \n", str1);
				if (count != 1) {
					printf("Incorrect input for compaction command\n");
				} else {
					int a = compaction(&hole_head, &process_head);
					if (a == -1) {
						printf("There is no process\n");
					} else {
						printf("Compaction process is successful\n");
					}
				}
			} else if (strcmp(str1, "STATUS") == 0) {
				display(&hole_head, &process_head);
			} else {
				printf("Check your command input, it is questionable\n");
			}

		} else if (strcmp(str1, "EXIT") == 0) {
			printf("\nProgram Terminated per User Request\n");
			exit(0);
		} else {
			printf("\n Input is incorrect, check and input again!\n");
			//printf("\nProgram Terminated due to incorrect input\n\n");
			//exit(-1);
		}
	}
	return 0;
}

/**
 * convert a string to lowercase string
 * @param str
 */
void func(char *str) {
	for (int i = 0; i <= strlen(str); i++) {
		if (str[i] >= 65 && str[i] <= 90)
			str[i] = str[i] + 32;
	}
}

/**
 * convert string to uppercase string
 * @param str
 */
void touppercase(char *str) {

	for (int i = 0; i <= strlen(str); i++) {
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] = str[i] - 32;
	}
}

/**
 * First fit function
 * @param pid1, process ID
 * @param size1, process memory size
 * @param hole_head, hole head
 * @param process_head, process head
 * @return -1, no hole of sufficient size, otherwise, successfuly allocated memory
 */
int first_fit(char *pid1, int size1, hole_t **hole_head,
		process_t **process_head) {
	process_t *ptr1;
	ptr1 = *process_head;
	hole_t *ptr;
	ptr = *hole_head;
	int flag = 0;
	while (ptr != NULL) {
		if (ptr->size >= size1) {
			flag = 1;
			break;
		}
		ptr = ptr->next;
	}
	if (flag == 0) {		// no hole with sufficient size for the process
		return -1;
	} else {
		process_insert(process_head, ptr->start_address, pid1, size1);
		ptr->start_address = ptr->start_address + size1;
		ptr->size = ptr->size - size1;

		int a = clean_empty_hole(hole_head);
		return 1;
	}
}

/**
 *	best fit function
 * @param pid1
 * @param size1
 * @param hole_head
 * @param process_head
 * @return -1, no hole of sufficient size, otherwise, successfuly allocated memory
 *
 */
int best_fit(char *pid1, int size1, hole_t **hole_head,
		process_t **process_head, int initial_delta) {

	hole_t *ptr;
	ptr = *hole_head;
	int len1 = length(hole_head);
	process_t *ptr1;
	ptr1 = *process_head;

	if (*hole_head == NULL) {
		return -1;
	}
	int best_delta = initial_delta;	//set delta as the maximum possible value
	int flag = 0;
	int i = 0;

	while (ptr != NULL) {
		if ((ptr->size - size1) >= 0 && (ptr->size - size1) <= best_delta) {
			if (len1 > 1) {	//only when hole count is bigger than 1, show the delta
				printf("index = %d delta = %d best delta = %d\n", i,
						ptr->size - size1, best_delta);
			}
			best_delta = ptr->size - size1;
			flag = 1;	//when one hole has sufficient size, change the flag
			i++;
		}
		ptr = ptr->next;
	}
	if (flag == 0) {
		return -1;
	}
	ptr = *hole_head;
	while (ptr != NULL && ptr->size - size1 != best_delta) {
		ptr = ptr->next;
	}
	process_insert(process_head, ptr->start_address, pid1, size1);

	ptr->start_address = ptr->start_address + size1;
	ptr->size = ptr->size - size1;

	int a = clean_empty_hole(hole_head);	//in case hole is 0 size
	return 1;
}
/**
 * Function for worst fit
 * @param pid1
 * @param size1
 * @param hole_head
 * @param process_head
 * @return -1, no hole of sufficient size, otherwise, successfuly allocated memory
 *
 */
int worst_fit(char *pid1, int size1, hole_t **hole_head,
		process_t **process_head) {

	hole_t *ptr;
	ptr = *hole_head;
	int len1 = length(hole_head);
	process_t *ptr1;
	ptr1 = *process_head;

	if (*hole_head == NULL) {
		return -1;
	}

	int best_delta = -1;//set delta as a negative, want find the max positive delta

	int flag = 0;
	int i = 0;

	while (ptr != NULL) {
		if ((ptr->size - size1) >= 0 && (ptr->size - size1) >= best_delta) {
			if (len1 > 1) {	//only diplay when there are more than 1 hole
				printf("index = %d delta = %d best delta = %d\n", i,
						ptr->size - size1, best_delta);
			}
			best_delta = ptr->size - size1;
			flag = 1;
			i++;
		}
		ptr = ptr->next;
	}

	if (flag == 0) {
		return -1;
	}

	ptr = *hole_head;
	while (ptr != NULL && ptr->size - size1 != best_delta) {//reach to the hole with max delta
		ptr = ptr->next;
	}
	process_insert(process_head, ptr->start_address, pid1, size1);
	ptr->start_address = ptr->start_address + size1;
	ptr->size = ptr->size - size1;
	int a = clean_empty_hole(hole_head);	//in case hole is 0 size
	return 1;
}

/**
 * Fucntion to clean a hole from hole list if its size is zero
 * @param hole_head
 * @return	no use
 */
int clean_empty_hole(hole_t **hole_head) {

	hole_t *ptr;
	hole_t *previous;
	ptr = *hole_head;
	previous = NULL;

	if (*hole_head == NULL) {
		return 0;
	}
	while (ptr != NULL) {
		if (ptr->size == 0) {
			if (previous == NULL) {
				*hole_head = (*hole_head)->next;
			} else {
				previous->next = ptr->next;
				ptr = ptr->next;
			}
		} else {
			previous = ptr;
			ptr = ptr->next;
		}
	}
	return 0;
}

/**
 * Function to display the process allocation and hole details
 * @param hole_head
 * @param process_head
 */
void display(hole_t **hole_head, process_t **process_head) {
	hole_t *ptr1;
	process_t *ptr2;
	ptr1 = *hole_head;
	ptr2 = *process_head;

	int allocated_memory = 0;
	int free_memory = 0;

	while (ptr2 != NULL) {	//calculate the total allocated memory of processes
		allocated_memory = allocated_memory + ptr2->size;
		ptr2 = ptr2->next;
	}

	while (ptr1 != NULL) {	//calculate the total of free memory of holes
		free_memory = free_memory + ptr1->size;
		ptr1 = ptr1->next;
	}

	ptr1 = *hole_head;
	ptr2 = *process_head;
	printf("Partitions [Allocated memory = %d]:\n", allocated_memory);
	while (ptr2 != NULL) {
		printf("Address [%d:%d] Process %s\n", ptr2->start_address,
				ptr2->start_address + ptr2->size - 1, ptr2->pid);
		ptr2 = ptr2->next;
	}

	printf("\nHoles [Free memory = %d]:\n", free_memory);

	while (ptr1 != NULL) {
		printf("Address [%d:%d] len = %d\n", ptr1->start_address,
				ptr1->start_address + ptr1->size - 1, ptr1->size);
		ptr1 = ptr1->next;
	}
}

/**
 * Function to insert a new process to the process list
 * @param process_head
 * @param start_address, the start address of the new process to be inserted
 * @param pid1, the pid of the new process to be inserted
 * @param size1, the size of the new process to be inserted
 */
void process_insert(process_t **process_head, int start_address, char *pid1,
		int size1) {

	process_t *new_process;
	new_process = (process_t*) malloc(sizeof(process_t));

	new_process->start_address = start_address;
	strcpy(new_process->pid, pid1);
	new_process->size = size1;
	new_process->next = NULL;

	process_t *ptr = *process_head;
	process_t *previous = NULL;

	if ((*process_head) == NULL) {
		(*process_head) = new_process;
	} else if ((*process_head)->start_address > start_address) {
		new_process->next = (*process_head);
		(*process_head) = new_process;
	} else {
		while (ptr != NULL) {
			if (ptr->start_address > start_address) {
				previous->next = new_process;
				new_process->next = ptr;
				break;
			} else {
				previous = ptr;
				ptr = ptr->next;
			}
		}
		if (ptr == NULL) {
			previous->next = new_process;
		}
	}
}
/**
 * Function to calculate the number of holes in the hole list
 * @param hole_head
 * @return hole count in the hole list
 */
int length(hole_t **hole_head) {
	int length = 0;
	hole_t *current;

	for (current = *hole_head; current != NULL; current = current->next) {
		length++;
	}
	return length;
}
/**
 * Function to calculate the numberof processes in the process list
 * @param process_head
 * @return process count in the process list
 */
int length1(process_t **process_head) {
	int length = 0;
	process_t *current;

	for (current = *process_head; current != NULL; current = current->next) {
		length++;
	}
	return length;
}

/**
 * Function to release the process with ID of pid1 from the process list
 * @param hole_head
 * @param process_head
 * @param pid1, ID of the process to be released
 * @return -1: no process with ID of pid1 was found
 */
int release(hole_t **hole_head, process_t **process_head, char *pid1) {

	process_t *ptr_p;
	hole_t *ptr_h;
	process_t *first = NULL;
	process_t *t_value = NULL;
	int start_address;
	int size1;

	ptr_p = *process_head;
	ptr_h = *hole_head;

	if (*process_head == NULL) {
		return -1;
	}

	int flag = 0;

	while (ptr_p != NULL) {
		if (first == NULL && strcmp(ptr_p->pid, pid1) == 0) { //if first process is the one to be released
			start_address = ptr_p->start_address;
			size1 = ptr_p->size;
			printf("releasing memory for process %s\n", ptr_p->pid);
			flag = 1;
			break;
		} else {
			if (strcmp(ptr_p->pid, pid1) == 0) {
				start_address = ptr_p->start_address;
				size1 = ptr_p->size;
				printf("releasing memory for process %s\n", ptr_p->pid);
				flag = 1;
				break;
			} else {
				first = ptr_p;
				ptr_p = ptr_p->next;
			}
		}
	}
	if (flag == 0) {
		return -1;
	}

	if (first == NULL) {
		t_value = (*process_head)->next;
		free(*process_head);
		*process_head = t_value;
	} else {
		if (ptr_p->next == NULL) {
			first->next = NULL;
		} else {
			first->next = ptr_p->next;
		}
		free(ptr_p);
	}
	hole_insert(hole_head, start_address, size1); // insert the rleased memory to hole list
	return 0;
}
/**
 * Functo to insert block of released memory to hole list
 * @param hole_head
 * @param start_address, start address of the block of released memory
 * @param size1, size of the block of released memory
 */
void hole_insert(hole_t **hole_head, int start_address, int size1) {
	hole_t *ptr;
	ptr = *hole_head;

	hole_t *new_hole;
	new_hole = (hole_t*) malloc(sizeof(hole_t));
	new_hole->start_address = start_address;
	new_hole->size = size1;
	new_hole->next = NULL;
	hole_t *previous = NULL;

	if (*hole_head == NULL) {
		*hole_head = new_hole;
	} else if ((*hole_head)->start_address > start_address) {
		/*
		 printf("zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz\n");
		 printf("head start address is %d   argument start address is %d\n",
		 (*process_head)->start_address, start_address);
		 */
		new_hole->next = (*hole_head);
		(*hole_head) = new_hole;
	} else {

		while (ptr != NULL) {
			if (ptr->start_address > start_address) {
				previous->next = new_hole;
				new_hole->next = ptr;
				break;
			} else {
				previous = ptr;
				ptr = ptr->next;
			}
		}
		if (ptr == NULL) {
			previous->next = new_hole;
		}
	}
	hole_combination(hole_head);
}
/**
 * Function to combine the holes with contiguous address
 * @param hole_head
 */
void hole_combination(hole_t **hole_head) {
	hole_t *ptr;
	hole_t *next = NULL;
	ptr = *hole_head;

	while (ptr->next != NULL) {
		next = ptr->next;

		if ((ptr->start_address + ptr->size) == next->start_address) {
			ptr->size = ptr->size + next->size;
			if (next->next == NULL) {
				ptr->next = NULL;
			} else {
				ptr->next = next->next;
			}
			free(next);
		} else {
			ptr = ptr->next;
		}
	}
}
/**
 * Compact the memory, move process to lower address with contiguous address, and hole to high address
 * @param hole_head
 * @param process_head
 * @return	=-1: no process in the memory
 */
int compaction(hole_t **hole_head, process_t **process_head) {
	process_t *ptr_p;
	hole_t *ptr_h;
	hole_t *temp;

	if (*process_head == NULL) {
		return -1;
	}
	int p_mem_num;
	(*process_head)->start_address = 0; //force the first process memory address to be zero
	p_mem_num = (*process_head)->size;

	ptr_p = *process_head;
	while (ptr_p->next != NULL) {
		ptr_p->next->start_address = ptr_p->start_address + ptr_p->size;
		p_mem_num = p_mem_num + ptr_p->next->size;
		ptr_p = ptr_p->next;
	}
	if (*hole_head == NULL) {
		return 0;
	}
	int h_mem_num = 0;
	ptr_h = (*hole_head);
	while (ptr_h != NULL) {
		h_mem_num = h_mem_num + ptr_h->size;
		temp = ptr_h->next;
		free(ptr_h);
		ptr_h = temp;
	}
	hole_t *new_hole;
	new_hole = (hole_t*) malloc(sizeof(hole_t));
	new_hole->start_address = p_mem_num;
	new_hole->size = h_mem_num;
	new_hole->next = NULL;

	*hole_head = new_hole;
}
