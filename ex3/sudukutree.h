/**
* @file sudukutree.h
* @author Raphael Peretz <refi950>
* @date Dec 2014
*
* @brief Header fot the sudkutree.c which implements
*		the genericdfs functions.
*
* @section DESCRIPTION
*		contains functions and parameters delcrations implemented
*		in sudku tree.
*/
#ifndef SODUKU_H
#define SODUKU_H
#include "genericdfs.h"

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
int getNodeChildrenSoduku(pNode, pNode** /*for the result*/);

/**
* @brief free alloctes memory of the node it self.
*@param freeNode the node we need to free its memory.
* @return void funtion
*/
void freeNodeSoduku(pNode);
/**
* @brief Deep copy sudku table.
*@param copyNode the node we need to copy.
* @return copyMatrix copied sudkutable.
*/
pNode copyNodeSoduku(pNode);

/**
* @brief Another implemention of the genericdfs.c calculate
*		the suduku table value for the generic dfs.
*
*@param node -The sudukutable we need to calculate its value.
*
* @return the value of the sudukutable.
*/
unsigned int getValSoduku(pNode node);
/**
* @def gTableSize
* @brief Suduku table size.
*/
extern int gTableSize;
//I decided to implement each node as a matrix.
//which represents a sudkutable.
typedef int** sudukuTable;
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
int checkSquare(sudukuTable matrix, const int row, const int col);
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
int validateRowsAndCols(sudukuTable matrix, const int row, const int col);

#endif