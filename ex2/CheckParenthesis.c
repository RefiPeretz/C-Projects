/**
* @file CheckParenthesis.c
* @author  refi950
*
* @brief The CheckParenthesis program received file
*        with parenthesis and check thier legilty.
*
* @section DESCRIPTION
* The system recevied files containg parenthesis examine them and
* checks thier leglity. 
*
* Input  : Files with parenthesis expressions.
* Process: Examing the parenthesis of the given file.
* Output : Error messagne in case of a error bad structre
*          in case of bad parenthesis structre or message
*          that everything okay.
*/

#include <stdio.h>

/**
* @def MAXIMUM_ELEMENTS 5000
* @brief The maximum allowed in the file
*/
#define MAXIMUM_ELEMENTS 5000
/**
* @def EMPTY 0
* @brief Rpresented an empty stack.
*/
#define EMPTY 0
/**
* @def TRUE 1
* @brief Rpresented true statement.
*/
#define TRUE 1
/**
* @def FALSE 0
* @brief Rpresented false statement.
*/
#define FALSE 0
/**
* @def EMPTY_CHAR_SYMBOL 'e'
* @brief Rpresented an empty char in case of an empty stack.
*/
#define EMPTY_CHAR_SYMBOL 'e'
/**
* @def ERROR 1
* @brief Rpresenting an error occurance.
*/
#define ERROR 1
/**
* @def SUCSSES 0
* @brief Represents successful running program.
*/
#define SUCSSES 0
/**
* @def NUMBER_OF_ACQUIRED_ARGUMENTS 2
* @brief The number of arguments i should get.
*/
#define NUMBER_OF_RCQUIRED_ARGUMENTS 2



/**
* @brief Struct for examine the parenthesis.
*        By push openning parenthesis and poping them
*        in case of encountring a close parenthesis
*        If we meet its match so we ok otherwise.
*        we have bad structre.
*
* @param topElement the last elemet
* @param stackArray the body of the struct
*/

	struct Stack
	{
		int  topElement;
		char stackArray[MAXIMUM_ELEMENTS];

	};
	typedef struct Stack Stack;





	/**
	* @brief Part of the implementation of the stack.
	*        pushes new element to the stack.
	*
	* @param myStack - pointer to the current stack.
	* @param putChar the new element we need to add.
	*/
	void push(Stack *myStack, char pushChar)
	{
		//If we didn't reach the max capacity of the struct.
		if (myStack->topElement < MAXIMUM_ELEMENTS)
		{
			//push element.
			myStack->stackArray[myStack->topElement] = pushChar;
			myStack->topElement++;

		}

	}
	/**
	* @brief Part of the implemention of the struct.
	*        poping out an element.
	*         while updating the struct.
	*        in case of empty stack returns empty char
	*
	*
	* @param stackParenthesos pointer to my stack.
	* @return The poping char or empty char in case of empty stack.
	*
	*/
	char pop(Stack *stackParenthesis)
	{
		if (stackParenthesis->topElement != EMPTY)
		{
			stackParenthesis->topElement--;
			return stackParenthesis->stackArray[stackParenthesis->topElement];
		}
		else
		{
			return EMPTY_CHAR_SYMBOL;
		}
	}
	/**
	* @brief Helper function. determine if the char
	*        we got is an open char so we could determine
	*        weather or not we need to add it to the stack.
	*
	* @param checkChar The current char we examine and check.
	* @return True 1 in case of open char otherwise false 0.
	*/

	int isOpenChar(char checkChar)
	{
		return (checkChar == '{' || checkChar == '(' || checkChar == '<' || checkChar == '[');

	}

	/**
	* @brief Helper function. determine if the char
	*        we got is an close char.
	*
	* @param checkChar The current char we examine and check.
	* @return True 1 in case of open char otherwise false 0.
	*/

	int isCloseChar(char checkChar)
	{
		return (checkChar == '}' || checkChar == ')' || checkChar == '>' || checkChar == ']');
	}

	char getMatchOpenChar(char closeChar)
	{
		switch (closeChar)
		{
			case '}':
				return '{';
			case ')':
				return '(';
			case '>':
				return '<';
			case ']':
				return '[';
			default:
				return EMPTY_CHAR_SYMBOL;
		}
	}
	/**
	* @brief Helper function. activate in case of closing char.
	*        validate that the stack holds its matching openning char.
	*
	*
	* @param *myStack The current stack as a pointer.
	* @param closeChar the closing char we need to examine.
	*
	* @return True 1 in case of matching close open char otherwise false 0.
	*/

	int checkValidPair(Stack *myStack, char closeChar)
	{
		//Gest matching open char.

		char matchOpenChar = getMatchOpenChar(closeChar);
		//compare between chars.
		if (matchOpenChar != pop(myStack))
		{
			printf("Bad structure\n");
			return ERROR;
		}
		return (SUCSSES);
	}

/**
* @brief The main function. recevied the arguments of tholding the
*        file which we need to validate its structre.
*
* @param argv[] an array of the arguments - warehouse order....
* @param argc size of the array.
*/

	int main(int argc, char* argv[])
	{
		if (argc > NUMBER_OF_RCQUIRED_ARGUMENTS)
		{
			printf("too many parameters\n");
			printf("usage:CheckParenthesis <filename>\n");
			return ERROR;
		}
		else if (argc < NUMBER_OF_RCQUIRED_ARGUMENTS)
		{
			printf("please supply a file!\n");
			printf("usage:CheckParenthesis <filename>\n");
			return ERROR;
		}
		FILE* fp = fopen(argv[1], "r");
		if (fp == NULL)
		{
			printf("file \"%s\" not found\n", argv[1]);
			return ERROR;

		}
		Stack myStack;
		myStack.topElement = EMPTY;
		char readChar;
		while (fscanf(fp, "%c", &readChar) != EOF)
		{

			if (isOpenChar(readChar) == TRUE)
			{

				push(&myStack, readChar);
			}
			else if (isCloseChar(readChar) == TRUE)
			{



				if (checkValidPair(&myStack, readChar) == ERROR)
				{
					fclose(fp);
					return (ERROR);
				}
			}
		}
		printf("Ok\n");
		fclose(fp);
		return SUCSSES;
	}