
/*
* shift.c
* @created on: 31/10/2014
* @author raphael peretz <refi950>
* @section DESCRIPTION
* The shit program, implement the encrypt decrypt functions.
* which encrypt and decrypt uder inputs and display it.
* Input : a string
* Output : Prints according to the user choise and shift.The encryption.
* and decryption value of the user's string input.
*/
#include <stdio.h>
//User decisions
/** 
 * @def ENCRYPT 'e'
 * @brief The represents the choise of
 *        encryption.
 */
#define ENCRYPT 'e'
 /** 
 * @def DECRYPT  'd'
 * @brief The represents the choise of
 *        decryption.
 */
#define DECRYPT 'd'
 /** 
 * @def CAPITAL_LETTER  'C'
 * @brief Represents If we have char
 *        that is capital letter.
 */
#define CAPITAL_LETTER 'C'
/** 
 * @def NON_CAPITAL_LETTER 'N'
 * @brief Represents If we have char
 *        that is  non capital letter.
 */
#define NON_CAPITAL_LETTER 'N'
/** 
 * @def DIGIT 'D'
 * @brief Represents If we have char
 *        that is DIGIT.
 */
#define DIGIT 'D'
/** 
 * @def SYMBOL 'S'
 * @brief Represents If we have char
 *        that is  no letter nor digit.
 */
#define SYMBOL 'S'
//Ranges of ascii representations
//and domain ranges.
 /** 
 * @def DEAFULT_RANGE 1
 * @brief RANGE of group that holds one member (deafult group).
 */
#define DEAFULT_RANGE 1
 /** 
 * @def LETTERS_RANGE 26
 * @brief RANGE of the letters a-z
 */
#define LETTERS_RANGE 26
 /** 
 * @def DIGITS_RANGE 10
 * @brief RANGE of the digits 0-9
 */
#define DIGITS_RANGE 10
 /** 
 * @def CAPITAL_LETTERS_START 65
 * @brief The capital letters ascii lower bound.
 */
#define CAPITAL_LETTERS_START 65
 /** 
 * @def CAPITAL_LETTERS_END 90
 * @brief The capital letters ascii upper bound.
 */
#define CAPITAL_LETTERS_END 90
  /** 
 * @def NON_CAPITAL_LETTERS_START 97
 * @brief The non-capital letters ascii lower bound.
 */
#define NON_CAPITAL_LETTERS_START 97
  /** 
 * @def NON_CAPITAL_LETTERS_END 122
 * @brief The non-capital letters ascii upper bound.
 */
#define NON_CAPITAL_LETTERS_END 122
   /** 
 * @def DIGITS_START 48
 * @brief The digits ascii upper bound.
 */
#define DIGITS_START 48
  /** 
 * @def DIGITS_END 57
 * @brief The digits ascii upper bound.
 */
#define DIGITS_END 57
  /** 
 * @def DEAFULT_BOUND 0
 * @brief The deafult value for the parameters we need to
 * circle. for a-zA-Z and 0-9 thier ascii bound are more
 * than 0 for sure.
 */
#define DEAFULT_BOUND 0

/**
* @brief Helper function to the encrypt/decrypt functions.
*  which help to decide what kind of char we dealing with.
* @param asciiRepresentation, the numeric representation of the
*         input char.
* @return If the char is a capital/non capital letter digit or a symbol.
*/

char isLetterOrDigit(int asciiRepresentation)
{
	if ((asciiRepresentation >= CAPITAL_LETTERS_START &&
	    asciiRepresentation <= CAPITAL_LETTERS_END) ||
	    (asciiRepresentation >= NON_CAPITAL_LETTERS_START &&
	    asciiRepresentation <= NON_CAPITAL_LETTERS_END ))
	{

		if (asciiRepresentation >= NON_CAPITAL_LETTERS_START)
		{
			return NON_CAPITAL_LETTER;
		}
		else
		{
			return CAPITAL_LETTER;

		}

	}
	else if (asciiRepresentation >= DIGITS_START && asciiRepresentation <= DIGITS_END)
	{
		return DIGIT;

	}
	return SYMBOL;

}

/**
* @brief Enycrpts chars by shifting them away in a 
*        circle way(only to digits and abc chars while
*		 the function doesn't shift other chars like symbols.
* @param shift the amount of chars to shift in digits and abc chars.
* @param inputChar The char we need to encrypt.
* @return The encrypted char.
*/
char encrypt(int shift, char inputChar)
{
	//Switching to ascii representation, which make the proccess
	//of shifting the char esaier.
	int asciiRepresentation = (int)inputChar;
	//The upper bound determine weather or not we have
	//to make a circle in case of abc chars or digits 
	//I will explain it later.
	int upperBound = DEAFULT_BOUND;
	//The domain range is the ragne of the specific group of chars
	//we examine. for example in digits its 10. 
	//The deafult value will be 1. this is the base case of
	//group with only one char.
	int domainRange = DEAFULT_RANGE;
	//Determine our case. in case of symbol the function will return
	//the original value as instructed.
	//In case of groups we will determine their bounds and thier
	//domain range.
	char letterOrDigit = isLetterOrDigit(asciiRepresentation);
	switch (letterOrDigit)
	{
		case CAPITAL_LETTER:
			upperBound = CAPITAL_LETTERS_END;
			domainRange = LETTERS_RANGE;
			break;

		case NON_CAPITAL_LETTER:
			upperBound = NON_CAPITAL_LETTERS_END;
			domainRange = LETTERS_RANGE;
			break;

		case DIGIT:
			upperBound = DIGITS_END;
			domainRange = DIGITS_RANGE;
			break;
	}
	//According to my formula we can get rid of the circle.
	//In that way in case of shift 95 in digits we need to
	//examine only the 5 steprs the other 90 are just loops.
	//That way we don't need loop that make us 9 circles.
	//And creating runtime of O(shift). while in this way,
	//its only O(1).
	asciiRepresentation += shift % domainRange;
	//Making sure if I need to do a circle.
	if (upperBound > DEAFULT_BOUND && asciiRepresentation > upperBound)
	{
		asciiRepresentation -= domainRange;
	}

	return (char)asciiRepresentation;

}
/**
* @brief decrypts chars by shifting them away in a
*       circle way(only to digits and abc chars while
*       the function doesn't shift other chars like symbols.
* @param shift the amount of chars to shift in digits and abc chars.
* @param inputChar The char we need to encrypt.
* @return The decrypted char.
*/
char decrypt(int shift, char inputChar)
{
    //Very similler to the encrypt function.
	//But we doing the oppisite action so we use
	//lowerbounds and turn over the encryption
	//Please see the comments on the encryption.
	int asciiRepresentation = (int)inputChar;
	int lowerBound = DEAFULT_BOUND;
	int domainRange = DEAFULT_RANGE;
	char letterOrDigit = isLetterOrDigit(asciiRepresentation);
	switch (letterOrDigit)
	{
		case CAPITAL_LETTER:
			lowerBound = CAPITAL_LETTERS_START;
			domainRange = LETTERS_RANGE;
			break;

		case NON_CAPITAL_LETTER:
			lowerBound = NON_CAPITAL_LETTERS_START;
			domainRange = LETTERS_RANGE;
			break;

		case DIGIT:
			lowerBound = DIGITS_START;
			domainRange = DIGITS_RANGE;
			break;
	}

	asciiRepresentation -= shift % domainRange;

	if (lowerBound > DEAFULT_BOUND && asciiRepresentation < lowerBound)
	{
		asciiRepresentation += domainRange;
	}

	return (char)asciiRepresentation;

}

/**
* @brief The main function. gather the user wants.which means
*        the shift value, the action and the string he want
*        to encrypt\decrypt.
* @param shift the amount of chars to shift in digits and abc chars.
* @param inputChar The char we need to encrypt.
* @return 0 to end program and print encrypted/decrypted chars.
*/
int main()
{
	//Save user decision de or en.
	char userDecision;
	int shift;

	printf("please enter the shift:\n");
	scanf("%d", &shift);

	do
	{
		printf("would you like to encrypt (e) or decrypt (d)?\n");
		scanf(" %c%1*[\n]", &userDecision);
	} while ((userDecision != 'e') && (userDecision != 'd'));

	char getLetter;

	//Getting the user input. and translate it base on the userDecision result.
	if (userDecision == ENCRYPT)
	{
		while ((getLetter = getchar()) != EOF)
		{
			putchar(encrypt(shift, getLetter));

		}

	}
	else if (userDecision == DECRYPT)
	{
		while ((getLetter = getchar()) != EOF)
		{
			putchar(decrypt(shift, getLetter));

		}
		
	}




	return (0);

}