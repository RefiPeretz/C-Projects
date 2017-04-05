/**
* @file genericdfs.c
* @author Raphael Peretz <refi950>
* @date Dec 2014
*
* @brief Generic implemention which return greatet or best
*		 value by scanning a graph using DFS.
*
* @section DESCRIPTION
* Generic implemention which return greatet or best
*		 value by scanning a graph using DFS.
*
* Input : root node and functions according to dfs implemetion.
* Process: Base on dfs search, searching for best value.
* Output : the best Solution.
*/
// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "genericdfs.h"
// -------------------------- const definitions -------------------------
/**
* @def EMPTY 0
* @brief If a node doesn't have sons.
*/
#define EMPTY 0

// -------------------------- global definitions -------------------------
/**
* @def gHighest
* @brief Save the node with the highest value it found a long the way.
*/
pNode gHighest = NULL;
/**
* @def gHighestValue
* @brief Save the value of the highest node preventing uneccery calculation.
*/
unsigned int gHighestValue = 0;
// ------------------------------ functions -----------------------------

/**
* @brief Update the highest value found in the graph until now.
*		 If we don't find the best we need to return the highest.
*@param getNodeValFunc - get value from node function
*@param copyNodeFunc - deep copy nodes function.
*@param freeNodeFunc - freeing nodes from memory function.
*@param best - best the value we looking for.
* @return it's a void function.
*/

void updateHighest(pNode curNode, getNodeValFunc getVal, copyNodeFunc copy,
				   freeNodeFunc freeNode, unsigned int best)
{
	unsigned int curNodeValue = getVal(curNode);
	if (gHighest == NULL || (curNodeValue > gHighestValue && curNodeValue <= best))
	{
		if(gHighest != NULL)
		{
			//Free the former highest value.
		 	freeNode(gHighest);
		}
		gHighestValue = curNodeValue;
		gHighest = copy(curNode);
	}
}
/**
* @brief Helper function, helps freeing the memory of the node children.
*
*@param childtrenArray - Array we nodes to free.
*@param freeNodeFunc - freeing nodes from memory function.
*@param size - Number of children.
*
* @return it's a void function.
*/

void freeChildren(pNode* childrenArray, freeNodeFunc freeNode, const int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		freeNode(childrenArray[i]);

	}

}

/**
* @brief Helper function, for get best. immitate the explore
*        function of original DFS. searching the graph until it
*        find the node with the value of best
*		 while updating the highest values we found along the way. in case we find the best we
*		 break the recursion and return the frist we find.
*		  In case it don't find the best it update
*		 highest to be the node with the most close to value to best.
*
*@param pNode head - the root node we start the search from.
*@parm getNodeChildrenFunc - Function which calcutes children of a node and returns amount.
*@param getNodeValFunc - get value from node function
*@param freeNodeFunc - freeing nodes from memory function.
*@param copyNodeFunc - deep copy nodes function.
*@param best - best the value we looking for.
* @return it's a void function.
*/

void dfsExplore(pNode head, getNodeChildrenFunc getChildren, getNodeValFunc getVal,
				freeNodeFunc freeNode, copyNodeFunc copy, unsigned int best)
{
	//Update the highest according node.
	updateHighest(head, getVal, copy, freeNode, best);
	if (gHighestValue == best)
	{
		//Break we found the best.
		return;
	}
	
	pNode *childrenArray;
	//Creating the node childrens.
	int numOfChildren = getChildren(head, &childrenArray);
	//If we don't have children we are done with this node.
	if (numOfChildren == EMPTY)
	{
		return;
	}
	int i;

   	//check for best in all the node children.
	for (i = 0; i < numOfChildren; i++)
	{

		if (gHighestValue == best)
		{
			break;
		}

		dfsExplore(childrenArray[i], getChildren, getVal, freeNode, copy, best);

	}
	//Kill the children after we finished we the node.
	freeChildren(childrenArray, freeNode, numOfChildren);
	free(childrenArray);
	return;
}


/**
* @brief The gest best function finds the node we the best value based on DFS
*		 algorithem.
*
*@param pNode head - the root node we start the search from.
*@parm getNodeChildrenFunc - Function which calcutes children of a node and returns amount.
*@param getNodeValFunc - get value from node function
*@param freeNodeFunc - freeing nodes from memory function.
*@param copyNodeFunc - deep copy nodes function.
*@param best - best the value we looking for.
* @return returnValue the node with the highest value or best value.
*/
pNode getBest(pNode head, getNodeChildrenFunc getChildren, getNodeValFunc getVal,
			  freeNodeFunc freeNode, copyNodeFunc copy, unsigned int best)
{
	pNode returnValue = NULL;
	dfsExplore(head, getChildren, getVal, freeNode, copy, best);
	if(gHighest != NULL)
	{
		returnValue = copy(gHighest);
		freeNode(gHighest);

	}
	return returnValue;


}


