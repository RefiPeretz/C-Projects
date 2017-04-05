
/*
* count.c
* @created on: 31/10/2014
* @author raphael peretz <refi950>
* @section DESCRIPTION
* The count program. Examine user text input and etermine number of lines
* total chars and words.
* Input : a string
* Output : Prints number of chars words and lines that appers in the user input
* string.
*/
#include <stdio.h>
 /** 
 * @def define SPACE ' '
 * @brief Representing a space and indicator for a new word.
 */
#define SPACE ' '
  /** 
 * @def NEW_LINE '\n'
 * @brief Representing a new line and indicator for a new word.
 */
#define NEW_LINE '\n'


/**
* @brief The main function. getting the user string as an input.
*        and scan it char by char. By examine the chars and the chars
*        before determines new lines, words and counting chars.
*
* @return 0 And printing the nimber of lines\chars\words it located
*          in the text.
*/
int main()
{
	//The next char.
	char inputChar;
	int countWords = 0;
	//Count new lines start from deafult first line.
	int countLines = 1;
	//The total number of chars in the text.
	int sumOfChars = 0;
	//First char after a not space or enter char is a word for
	//sure.
	char lastInputChar = SPACE;

	while ((inputChar = getchar()) != EOF)
	{

		sumOfChars++;
		
		if(inputChar == NEW_LINE)
		{
			countLines++;
		}

        //If also not space this is a char. check if it need to be a new word.
		else if(inputChar != SPACE)
		{
			if((lastInputChar == NEW_LINE) || (lastInputChar == SPACE))
			{
				countWords++;
			}

		}

		lastInputChar = inputChar;


	}


	printf("%d %d %d", sumOfChars, countWords, countLines);

	return (0);

}

