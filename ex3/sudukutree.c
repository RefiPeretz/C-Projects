/**
* @file sudukutree.c
* @author Raphael Peretz <refi950>
* @date Dec 2014
*
* @brief Generic implemention of the function of the genericdfs.c
*		 compitable to sudkutables.
*
* @section DESCRIPTION
* Implementing each function define in genericdfs.h making it
* possible to solve sudkutables using it.
*
*/

// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "genericdfs.h"
#include "sudukutree.h"

// -------------------------- const definitions -------------------------
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
/**
* @def EMPTY_CELL 0
* @brief In case of empty cell in matrix.
*/
#define EMPTY_CELL 0
/**
* @def NO_SONS 0
* @brief Represnts a case when node doesn't have sons.
*/
#define NO_SONS 0
/**
* @def ERROR 1
* @brief In case of an error.
*/

#define ERROR 1

// ------------------------------ functions -----------------------------


/**
* @brief Deep copy sudku table.
*@param copyNode the node we need to copy.
* @return copyMatrix copied sudkutable.
*/

pNode copyNodeSoduku(pNode copyNode)
{
	//Casting.
	sudukuTable nodeMatrix = (sudukuTable)copyNode;

	sudukuTable copyMatrix = malloc(gTableSize * sizeof(int*));
	//In case of failed allocation.
	if(copyMatrix == NULL)
	{
		exit(ERROR);
	}

	int i, j;
	for (i = 0; i < gTableSize; i++)
	{
		copyMatrix[i] = malloc(gTableSize * sizeof(int));
		if(copyMatrix[i] == NULL)
		{
			exit(ERROR);
		}
	}

	for (i = 0; i < gTableSize; i++) 
	{
		for (j = 0; j < gTableSize; j++)
		{
			copyMatrix[i][j] = nodeMatrix[i][j];
		}
	}

	return copyMatrix;
}

/**
* @brief free alloctes memory of the node it self.
*@param freeNode the node we need to free its memory.
* @return void funtion
*/

void freeNodeSoduku(pNode freeNode)
{
	sudukuTable freeNodeMatrix = (sudukuTable)freeNode;

	int i;
	for (i = 0; i < gTableSize; i++)
	{
		free(freeNodeMatrix[i]);
	}

	free(freeNodeMatrix);
}
/**
* @brief Helper fucntion to the creating children process.
*		one of the function which assert suduku table validty
*		by examine duplicates in a row or colum.
*		We assume that before the new cell insertion the
*		the matrix is valid.
*
*@param matrix - suduku table we need to check
*@param row - the row index of the new cell
*@param col - the colum index of the new cell.
* @return True in case of valid table otherwise false.
*/
int validateRowsAndCols(sudukuTable matrix, const int row, const int col)
{
    //The new cell value we need to validate.
	int checkVal = matrix[row][col];
	int i;
	//Validte colum
	for (i = 0; i < gTableSize; i++)
	{
		if ((matrix[row][i] == checkVal) && (i != col))
		{
			return FALSE;
		}
	}
	//Validate Row.
	for (i = 0; i < gTableSize; i++)
	{
		if ((matrix[i][col] == checkVal) && (i != row))
		{

			return FALSE;
		}
	}
	return TRUE;
}

/**
* @brief The second validate function. helps to valid
*		if a sudkutable is valid by checking the square mini
*		matrix for duplicates.
*
*@param matrix - suduku table we need to check.
*@param row - the row index of the new cell
*@param col - the colum index of the new cell.
* @return True in case of valid table otherwise false.
*/
int checkSquare(sudukuTable matrix, const int row, const int col)
{
	//Formula I inveted to find the first cell of the mini square where
	//the new cell is placed.
	int startRowSquare = (ceil((row + 1) / sqrt(gTableSize)) - 1) * sqrt(gTableSize);
	int startColSquare = (ceil((col + 1) / sqrt(gTableSize)) - 1) * sqrt(gTableSize);

	//Creating ture/false array to check duplicates.
	int* checkDuplicateArray = (int*)malloc(sizeof(int)*gTableSize);

	//In case of failed allocation.
	if(checkDuplicateArray == NULL)
	{
		exit(ERROR);
	}

	int index;

	for (index = 0; index < gTableSize; index++)
	{
		checkDuplicateArray[index] = FALSE;
	}
	//Ceating the bordres of the wanted mini squares.
	int stopValueRow = (startRowSquare + sqrt(gTableSize));
	int stopValueCol = (startColSquare + sqrt(gTableSize));
	int i, j;
	for (i = startRowSquare; i < stopValueRow; i++)
	{

		for (j = startColSquare; j < stopValueCol; j++)
		{

			if (matrix[i][j] != EMPTY_CELL && checkDuplicateArray[matrix[i][j] - 1] == TRUE)
			{
				free(checkDuplicateArray);
				return FALSE;
			}

			if (matrix[i][j] != EMPTY_CELL)
			{
				checkDuplicateArray[matrix[i][j] - 1] = TRUE;
			}
		}
	}
	free(checkDuplicateArray);
	return TRUE;
}



/**
* @brief Creates the node children according to the genericdfs implemention
*  The function create valid children for sudukutable by adding opttional
*	value to the first empty cell it finds.
*		 
*@param parent - the node we want to create its children.
*@param arrayOfChildrens - the array of children we update.
*
* @return Number of children.
*/
int getNodeChildrenSoduku(pNode parent, pNode** arrayOfChildrens)
{
	sudukuTable parentMatrix = (sudukuTable)parent;

	//Find first zero cell.
	int foundEmptyCell = FALSE;
	int x = 0;
	int y = 0;
	int i, j;
	for (i = 0; i < gTableSize; i++)
	{
		for (j = 0; j < gTableSize; j++)
		{
			if (parentMatrix[i][j] == EMPTY_CELL)
			{
				foundEmptyCell = TRUE;
				x = i;
				y = j;
				break;

			}
		}
		if (foundEmptyCell == TRUE)
		{
			break;
		}
	}
	//No zeroes in the table we cant add more children.
	if (foundEmptyCell == FALSE)
	{
		return NO_SONS;
	}
	//Creates all valid sudukutables adding numbers to the
	//empty cell from 1-gTableSize.
	sudukuTable* createChildren = (sudukuTable*)malloc(0 * sizeof(sudukuTable));
	//In case of failed allocation.
	if(createChildren == NULL)
	{
		exit(ERROR);
	}

	int numberOfChildrens = 0;
	int opptionalNum;
	for (opptionalNum = 1; opptionalNum <= gTableSize; opptionalNum++)
	{
		sudukuTable checkMatrix = copyNodeSoduku(parentMatrix);
		checkMatrix[x][y] = opptionalNum;
		if (validateRowsAndCols(checkMatrix, x, y) == TRUE &&
		    checkSquare(checkMatrix, x, y) == TRUE)
		{
			numberOfChildrens++;
			createChildren = (sudukuTable*)realloc(createChildren,
			 (numberOfChildrens)*sizeof(sudukuTable));
			//In case of failed allocation.
			if(createChildren == NULL)
			{
				exit(ERROR);
			}
			createChildren[numberOfChildrens - 1] = checkMatrix;
		}
		else
		{
			//If we don't use it.
			freeNodeSoduku(checkMatrix);
		}

	}

	//No valid children
	if(numberOfChildrens == NO_SONS)
	{

		free(createChildren);
		return NO_SONS;

	}
	//Updating the childrem array.
	pNode *newArray = (pNode*)malloc(numberOfChildrens * sizeof(pNode));

	//In case of failed allocation.
	if(newArray == NULL)
	{
		exit(ERROR);
	}
	for (i = 0; i < numberOfChildrens; i++)
	{
		newArray[i] = (pNode)createChildren[i];

	}
	free(createChildren);
	*arrayOfChildrens = newArray;
	return numberOfChildrens;

}

/**
* @brief Another implemention of the genericdfs.c calculate
*		the suduku table value for the generic dfs.
*
*@param node -  The sudukutable we need to calculate its value.
*
* @return the value of the sudukutable.
*/
unsigned int getValSoduku(pNode node)
{
	sudukuTable nodeMatrix = (sudukuTable)node;
	int i, j;
	unsigned int sum = 0;

	for (i = 0; i < gTableSize; i++)
	{
		for (j = 0; j < gTableSize; j++)
		{
			sum = sum + nodeMatrix[i][j];
		}
	}

	return sum;
}


