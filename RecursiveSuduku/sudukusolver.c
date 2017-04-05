
/**
* @file sudukusolver.c
* @author Raphael Peretz <refi950>
* @date Dec 2014
*
* @brief Solve suduku table.

* @section DESCRIPTION
* Prints a solved suduku in case of solution otherwise printing
* error message.
* Input : file with unsolved suduku
* Process: Searching for a solution.
* Output : Suduku solution.
*/
// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <math.h>
#include <string.h>
#include "sudukutree.h"
#include "genericdfs.h"


// -------------------------- const definitions -------------------------
/**
* @def MAX_LINE_LENGTH 200
* @brief Maximum length of lines.
*/
#define MAX_LINE_LENGTH 200
/**
* @def MAX_TABLE_SIZE 100
* @brief Maximum suduku table size.
*/
#define MAX_TABLE_SIZE 100
/**
* @def define DIVIDE_BY " \n"
* @brief devide lines by.
*/
#define DIVIDE_BY " \n"
/**
* @def CORRENT_NUMBER_OF_PARAMETERS 2
* @brief Expected number of arguments.
*/
#define CORRENT_NUMBER_OF_PARAMETERS 2
/**
* @def ERROR 1
* @brief In case of an error.
*/

#define ERROR 1
/**
* @def SUCSSES 0
* @brief In case of successful running.
*/

#define SUCSSES 0
/**
* @def TRUE 1
* @brief In case of true case.
*/
#define TRUE 1
/**
* @def FALSE 0
* @brief In case of false case.
*/
#define FALSE 0
// -------------------------- global definitions -------------------------
/**
* @def gTableSize
* @brief Suduku table size.
*/
int gTableSize;

// ------------------------------ functions -----------------------------



/**
*@brief:Helper function to build head
*	actually builds the head line by line.
*
*@param fp pointer to the file stream
*@param: filePath
*@return Matrix of the head as read from the file.
*
*/
sudukuTable buildHeadHelper(FILE* fp, const char* filePath)
{
	char line[MAX_LINE_LENGTH];
	char *token;
	//Allocate memory for the first suduku table.
	sudukuTable newHead = malloc(gTableSize*sizeof(int*));
	//In case of failed allocation.
	if(newHead == NULL)
	{
		exit(ERROR);
	}
	int matrixValue;
	int i, j;
	//Assamble the first table.
	for (i = 0; i < gTableSize; i++)
	{
		newHead[i] = malloc(gTableSize * sizeof(int));
		//In case of failed allocation.
		if(newHead[i] == NULL)
		{
			exit(ERROR);
		}
		if (fgets(line, MAX_LINE_LENGTH, fp) != NULL)
		{

			token = strtok(line, DIVIDE_BY);
			//In case of error free the head that was build so far.
			for (j = 0; j < gTableSize; j++)
			{
				//Not enough values.
				if (token == NULL)
				{

					printf("%s:not a valid suduku file\n", basename(filePath));
					gTableSize = i + 1;
					freeNodeSoduku(newHead);
					fclose(fp);
					exit(ERROR);
				}
				//validate input.
				int result = sscanf(token, "%d", &matrixValue);
				if (result != 1 || matrixValue < 0 || matrixValue > MAX_TABLE_SIZE)
				{

					printf("%s:not a valid suduku file\n", basename(filePath));
					gTableSize = i + 1;
					freeNodeSoduku(newHead);
					fclose(fp);
					exit(ERROR);
				}
				newHead[i][j] = matrixValue;
				token = strtok(NULL, DIVIDE_BY);
			}
			if(token != NULL)
			{
				printf("%s:not a valid suduku file\n", basename(filePath));
				gTableSize = i + 1;
				freeNodeSoduku(newHead);
				fclose(fp);
				exit(ERROR);
			}
		}
		else
		{
			printf("%s:not a valid suduku file\n", basename(filePath));
			gTableSize = i + 1;
			freeNodeSoduku(newHead);
			fclose(fp);
			exit(ERROR);
		}
	}
	fclose(fp);
	return newHead;

}

/**
*@brief:Buuild the first suduku table as shown in 
*       the file. The dfs algorithem will explore from
*       this table and will solve the suduku
*
*@param: filePath
*@return Matrix of the head as read from the file.
*
*/
sudukuTable buildHead(const char* filePath)
{
	FILE* fp = fopen(filePath, "r");
	if(fp == NULL)
	{

		printf("%s: no such file\n", basename(filePath));
		fclose(fp);
		exit(ERROR);
	}

	char line[MAX_LINE_LENGTH];
	char *token;
	//Securing table size value.
	if (fgets(line, MAX_LINE_LENGTH, fp) != NULL)
	{
		token = strtok(line, DIVIDE_BY);
		int result = sscanf(token, "%d", &gTableSize);
		if (result != 1 || gTableSize <= 0 || gTableSize > MAX_TABLE_SIZE ||
		    ((sqrt(gTableSize)) - (int)sqrt(gTableSize)) != 0.0)
		{
			printf("%s:not a valid suduku file\n", basename(filePath));
			fclose(fp);
			exit (ERROR);
		}

	}
	else
	{
		printf("%s:not a valid suduku file\n", basename(filePath));
		fclose(fp);
		exit (ERROR);
	}
	
	return buildHeadHelper(fp, filePath);

}
/**
*@brief:Prints a suduku table.
*
*@param: node the suduku table we need to print.
*
*/

void printMatrix(const sudukuTable node)
{
	int i, j;
	printf("%d\n", gTableSize);
	for (i = 0;  i < gTableSize; i++)
	{
		for (j = 0; j < gTableSize; j++)
		{
			if(j == 0)
			{
				printf("%d", node[i][j]);
			}
			else
			{
				printf(" %d", node[i][j]);
			}
			
		}
		printf("\n");
	}

}

/**
*@brief Validate if the initial table validty
*		before ruuning getBest.
*		
*		
*@param head - the table we need to validate.
*/
void checkHeadLegalty(int** head)
{
	int i, j;
	
	for(i = 0; i < gTableSize; i++)
	{
		for(j = 0; j < gTableSize; j++)
		{
			if((head[i][j] != 0) && ((checkSquare(head, i, j) == FALSE) ||
			   (validateRowsAndCols(head, i, j) == FALSE)))
			{
				printf("no solution!\n");
				freeNodeSoduku(head);
				exit(ERROR);
			}
		}
	}

}

/**
*@brief Main function of th suduku solver. activate
*       dfs search in order to solve the suduku table
*       presented in the file.
*		The function prints the table in case
*      	it finds a valid solution it prints it
*		otherwize it prints suiteable error message.
*@param argc - number of arguments
*@param argv[] - arguments.
*/
int main(int argc, char* argv[])
{
	if(argc != CORRENT_NUMBER_OF_PARAMETERS)
	{
		printf("please supply a file!\n");
		printf("usage: sudukusolver <filename>\n");
		return ERROR;
	}

	//Creating head
	sudukuTable head = buildHead(argv[1]);
	checkHeadLegalty(head);
	//Formula calculating the best value for the generic dfs.
	//According to tableSize best is the value of solved suduku.
	unsigned int best = ((gTableSize*(1 + gTableSize)) / 2)*gTableSize;
	//Calling the search
	pNode bestNode = getBest((pNode)head, getNodeChildrenSoduku, getValSoduku,\
	freeNodeSoduku, copyNodeSoduku, best);
	//Freeing the initial head.
	freeNodeSoduku(head);
	if(bestNode == NULL || getValSoduku(bestNode) !=  best)
	{
		printf("no solution!\n");
		freeNodeSoduku(bestNode);
		return ERROR;
	}
	//Printing solution
	printMatrix((sudukuTable)bestNode);
	//Freeing the solution before exiting.
	freeNodeSoduku(bestNode);
	//getchar();
	return SUCSSES;

}

