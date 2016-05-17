

/**
* @file waredb.c
* @author  refi950
*
* @brief The waredb program running a warehouse.
*        according to the excersize instructions.

* @section DESCRIPTION
* The system received and bulit a ware house recevied orders processs
* them and update the warehouse inventory accordingly.
*
* Input  : Warehouse db orders and clean orders.
* Process: digest the data and update the warehouse accordingly.
* Output : Error notifications and update the warehouse.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include <errno.h>


/**
* @def STILL_IN_PROCCESS 0
* @brief A flag sigining to the loop keep going.
*/
#define STILL_IN_PROCCESS 0
/**
* @def END_PROCCESS 1
* @brief A flag sigining to the loop to stop.
*/
#define END_PROCCESS 1
/**
* @def PRECISION 0.001
* @brief The value of precision we need to tak care of in our wareHouse.
*/
#define PRECISION 0.001
/**
* @def NAME_LIMIT 20
* @brief The name limit according to the instructions.
*/
#define NAME_LIMIT 20
/**
* @def EMPTY_ITEM  -1
* @brief Flag for an empty deleted item.
* when item is done i sgined with -1. to know not to write back to the file.
*/
#define EMPTY_ITEM  -1
/**
* @def ERROR 1
* @brief Flag for an ERROR, as instruct program returning value of 1.
*/
#define ERROR 1
/**
* @def BARCODE_LIMIT 4
* @brief barcode limit as shown in the instructions.
*/
#define BARCODE_LIMIT 4
/**
* @def DIVIDE_BY "\t\n"
* @brief symbols for dividing the lines by.
*/
#define DIVIDE_BY "\t\n"
/**
* @def DIVIDE_DATE_BY "-"
* @brief symbols for dividing the lines by.
*/
#define DIVIDE_DATE_BY "-"
/**
* @def PERRMISSION_ISSUE 13
* @brief Bulit in value of the function errno signal problem with permissions while openning
* the file.
*/
#define PERRMISSION_ISSUE 13
/**
* @def NO_FILE_ISSUE 2
* @brief Bulit in value of the function errno signal problem with founding the file.
*
*/
#define NO_FILE_ISSUE 2
/**
* @def MAXIMUM_FILE_SIZE 2000
* @brief Maximum of lines in file.
*
*/

#define MAXIMUM_FILE_SIZE 2000
/**
* @def EMPTY_LINE '\n'
* @brief Represnts empty line.
*
*/
#define EMPTY_LINE '\n'
/**
* @def EPSILON 0.00001
* @brief EPSILON value help me to determine if  float variables Are equal acoording
*        to our precision.
*/
#define EPSILON 0.00001
/**
* @def SUCSSES 0
* @brief Represents successful running program.
*/
#define SUCSSES 0
/**
* @def MONTH_UPPER_BOUND 12
* @brief Upper bonund of possible month value.
*/
#define MONTH_UPPER_BOUND 12
/**
* @def MONTH_LOWER_BOUND 0
* @brief Upper bonund of possible month value.
*/
#define MONTH_LOWER_BOUND 0
/**
* @def LINE_LENGTH 255
* @brief Maximum line length
*/
#define LINE_LENGTH 255
/**
* @def RECEIVED_ITEMS "received"
* @brief command for recevind new items
*/
#define RECEIVED_ITEMS "received"
/**
* @def SENT_ITEMS "sent"
* @brief command for sending items.
*/
#define SENT_ITEMS "sent"
/**
* @def CLEAN_ITEMS "clean"
* @brief command for cleaning items from the warehouse.
*/
#define CLEAN_ITEMS "clean"
/**
* @def NUMBER_OF_ACQUIRED_ARGUMENTS 2
* @brief The number of arguments i should get.
*/
#define NUMBER_OF_RCQUIRED_ARGUMENTS 4



/**
* @var int int countItems
* @brief Golbal size indicator for the warehouse..
*/
static int countItems;

/**
* @brief Struct for date value.
*
* @param year holds the year value.
* @param month holds the month value.
*/
struct date
{
	int year;
	int month;
};
typedef struct date date;

/**
* @brief Struct for a Item in warehouse.
*
* @param barcode holds the barcode value of the item.
* @param countSupply holds the amount of each item.
* @param name holds the name of the item.
* @param date a date val holds the experation date of the item.
*/
struct item
{
	int barcode;
	float countSupply;
	char name[NAME_LIMIT];
	struct date itemDate;
};
typedef struct item item;


/**
* @brief Helper function prints the correct error in file.
*
* @param errorIndex The error value according to errno.
* @param filePath[] The path of the file.
*/

void printFileError(int errorIndex, char filePath[])
{
	switch (errorIndex)
	{
		case PERRMISSION_ISSUE:
			printf("%s: Permission denied\n", basename(filePath));
			break;

		case NO_FILE_ISSUE:
			printf("%s: no such file\n", basename(filePath));
			break;

	}
}
/**
* @brief Helper function check the date structre of the
*        date string.
*
* @param dateString[] The string of the date we need to examine.
*/
int checkDateStructure(char dateString[])
{
	int year;
	int month;
	//If i dont get only 2 numeric values or my month value is invalid.
	if ((sscanf(dateString, "%d-%d", &year, &month) != 2) || (month < MONTH_LOWER_BOUND ||
		month > MONTH_UPPER_BOUND))
	{
		return ERROR;
	}
	return (SUCSSES);

}


/**
* @brief Helper function. updating the date values.
*        for an item.
*
* @param newItem pointer to a new Item to update.
* @param token[] the date in a string.
*/
void updateDate(date *newItemDate, char token[])
{
	const char deivideDateBy[2] = DIVIDE_DATE_BY;
	char *dateString = strtok(token, deivideDateBy);
	newItemDate->year = atoi(dateString);
	dateString = strtok(NULL, deivideDateBy);
	newItemDate->month = atoi(dateString);
}

/**
* @brief The comperator for the qsort function sort
* the warehouse first by barcode than by date
* and finally by name.
*
* @param item
*/
int compareItems(const void *firstItem, const void *secondItem)
{
	item *pFirstItem = (item*) firstItem;
	item *pSecondItem = (item*) secondItem;

	if (pFirstItem->barcode != pSecondItem->barcode)
	{
		return pFirstItem->barcode - pSecondItem->barcode;
	}

	if (pFirstItem->itemDate.year != pSecondItem->itemDate.year)
	{
		return pFirstItem->itemDate.year - pSecondItem->itemDate.year;
	}

	if (pFirstItem->itemDate.month != pSecondItem->itemDate.month)
	{
		return pFirstItem->itemDate.month - pSecondItem->itemDate.month;
	}

	return strcmp(pFirstItem->name, pSecondItem->name);
}

/**
* @brief Sorting the warehouse by the builtin  function qsort.
*        using a comperator adjust to the excersize demands.
*
* @param wareHouse[] our current warehouse stats.
*/
void sortWarehouse(item wareHouse[])
{
	qsort(wareHouse, countItems, sizeof(item), compareItems);
}


/**
* @brief Helper function to the addItems, by examine the current ware house.
*        the function decide if the add the item to the end of the ware house.
*         or merege it with another item
*
* @param wareHouse[] our current warehouse stats.
* @param newItem The new item we want to added.
*/
int addItemToWareHouse(item wareHouse[], item newItem)
{
	int index = 0;
	while (index < countItems)
	{
		//Check if the item already exsits and mereg them.
		//else added to the end of the warehouse.
		if ((newItem.barcode == wareHouse[index].barcode) &&
			(strcmp(wareHouse[index].name, newItem.name) == 0) &&
			(wareHouse[index].itemDate.year == newItem.itemDate.year) &&
			(wareHouse[index].itemDate.month == newItem.itemDate.month) &&
			newItem.countSupply - EPSILON > PRECISION)
		{

			wareHouse[index].countSupply += newItem.countSupply;
			return (SUCSSES);
		}
		index++;

	}
	if (newItem.countSupply - EPSILON > PRECISION)
	{
		wareHouse[index] = newItem;
		countItems++;

	}
	return (SUCSSES);

}

/**
* @brief The add items function is actually a helper function to the
*        received function and to the first creation of the warehouse.
*        The function build scanning the file line by line and adds the items to the warehouse.
*         even if its empty.
*
* @param wareHouse[] our current warehouse stats.
* @param filePath[] The file contains the items we need to add to the warehouse.
* @return True if the addition proccess went well other wise ERROR.
*/
int addItems(item wareHouse[], char filePath[])
{
	FILE *fp = fopen(filePath, "r");
	//Checking if there is a problem with file.
	if (fp == NULL)
	{
		printFileError(errno, filePath);
		return ERROR;
	}
	//Perepare new line and divding the line to token
	//each represents item parameter.
	char line[LINE_LENGTH];
	//verify if we encounter an error of unknow
	//file format during the process we know to stop.
	int fileFormatError = STILL_IN_PROCCESS;
	char devideBy[4] = DIVIDE_BY;
	char *token;
	char name[NAME_LIMIT];
	int barcode;
	float countSupply;
	int year;
	int month;
	int result;
	int countLines = 0;
	while (fgets(line, LINE_LENGTH, fp) != NULL)
	{

		//Creating a new item and update its fields.
		item newItem;
		//Check the legality of the parameters if the line strcture is legal which is
		//checked afterwards which not really matters.
		if ((result = sscanf(line, "%[^\t]\t%d\t%f\t%d-%d\n", name, &barcode, &countSupply, \
			&year, &month)) != 5)
		{
			if (line[0] == EMPTY_LINE)
			{
				continue;
			}
			fileFormatError = END_PROCCESS;
			break;
		}
		//Dividing to tokens and verify the line strcture.
		//Item Name.
		token = strtok(line, devideBy);
		if (token == NULL || strlen(token) > NAME_LIMIT)
		{
			fileFormatError = END_PROCCESS;
			break;
		}
		strcpy(newItem.name, token);

		//Barcode.
		token = strtok(NULL, devideBy);
		if (token == NULL || strlen(token) != BARCODE_LIMIT)
		{
			fileFormatError = END_PROCCESS;
			break;
		}
		newItem.barcode = atoi(token);

		//Checking for current invetory of the item.
		token = strtok(NULL, devideBy);
		if (token == NULL || atof(token) < 0)
		{
			fileFormatError = END_PROCCESS;
			break;
		}
		newItem.countSupply = atof(token);
		//Checking if i don't have extra parameters.
		token = strtok(NULL, devideBy);
		if (token == NULL || checkDateStructure(token) == ERROR)
		{
			fileFormatError = END_PROCCESS;
			break;
		}
		updateDate(&(newItem.itemDate), token);

		//Too many parameters in line.
		token = strtok(NULL, devideBy);
		if (token != NULL)
		{
			fileFormatError = END_PROCCESS;
			break;
		}
		//Joining the new item to the warehouse after updating its fields.
		addItemToWareHouse(wareHouse, newItem);
		countLines++;

	}
	fclose(fp);
	if (countLines == 0 || fileFormatError == END_PROCCESS)
	{
		printf("unknown file format\n");
		return ERROR;
	}
	//After i added all the item I sort them according to the instructions.
	sortWarehouse(wareHouse);
	//If the addition proccess went well. the function returning True.
	return SUCSSES;

}

/**
* @brief The received function. gets order to our wareHouse. using the addItems function.
*         since its doing exactly the same things.
*
* @param wareHouse[] our current warehouse stats.
* @param filePath[] the path of the received file.
*/
int received(item wareHouse[], char filePath[])
{
	//Just returning the result of additems.
	return(addItems(wareHouse, filePath));

}
/**
* @brief helper function for the sent function.
*        by sacnning the ware house locating the matching
*        barcode items we  want to sent. and save them.

* @param wareHouse[] The orginally ware house we want to sent items from.
*                     only if we can.
* @param sentItems[] Temprary warehouse which we are using to determine if we
*                     can sent the amount of items we demand without chaning our
*                     warehouse.
* @return The size of the matching items in the new array(its size).
*/

int locateSentItems(item wareHouse[], item sentItems[], int barcode)
{
	int i;
	int countMatchItems = 0;
	//Add items that matched in barcode.
	for (i = 0; i < countItems; i++)
	{
		if (wareHouse[i].barcode == barcode)
		{
			sentItems[countMatchItems] = wareHouse[i];
			countMatchItems++;
		}
	}
	return (countMatchItems);
}
/**
* @brief another sent helper function. This function scanning
*         The items that were found sutiable for shipping and
*         examine if outr ware house can deliver.

* @param sentItems the itmes in the warehouse that match the wanted barcode.
* @param size The size of the sentItems array.
* @param sentAmount the amount of the particilar item we want to sent.
* @return True if we have enough supply otherwise returns ERROR.
*/

int updateSentItemsSupply(item sentItems[], int size, float sentAmount)
{

	int i = 0;
	int isSentAmountEnd = STILL_IN_PROCCESS;

	//Running on the array and sending items. until our
	//order is done wihich means less than 0.001
	while ((i < size) && (sentAmount > PRECISION))
	{
		//If it is not an empty item.
		if (sentItems[i].countSupply != EMPTY_ITEM)
		{
			if ((sentItems[i].countSupply - sentAmount) >= (-1)*PRECISION)
			{
				//It means that we manage to complete our order.
				sentItems[i].countSupply -= sentAmount;
				isSentAmountEnd = END_PROCCESS;

			}
			else
			{
				//Else we updating the current item to zero and
				//scan the rest of the matching items to see if
				//we can complete the gap with another item.
				sentAmount -= sentItems[i].countSupply;
				sentItems[i].countSupply = 0;
			}
		}


		//If we out from some product we delete it.
		if (sentItems[i].countSupply <= 0)
		{
			sentItems[i].countSupply = EMPTY_ITEM;
		}
		//Stop if signle that we manage to complete the
		//send request
		if (isSentAmountEnd == END_PROCCESS)
		{
			return SUCSSES;
		}

		i++;

	}
	//If we got here it means we didn't have enough from the item
	//that ordered
	return (ERROR);

}
/**
* @brief Function which triggred after we managed to keep up with the send demands
*        The function update the original warehouse about the items new supply
*        after carring out the order.
* @param wareHouse[] the original warehouse.
* @param sentItems[] the array which holds the items witch meet the demands of the order.
*                    and which manipulated according to the order.
* @param             The size of the sentItems[] array.
*/

void updateOriginalWareHouse(item wareHouse[], item sentItems[], int sizeSentItems)
{
	int i;
	int j;
	for (i = 0; i < sizeSentItems; i++)
	{
		for (j = 0; j < countItems; j++)
		{
			//Locating the item which copied from the original warehouse and updating its
			//current supply.
			if ((wareHouse[j].barcode == sentItems[i].barcode) &&
				(wareHouse[j].itemDate.year == sentItems[i].itemDate.year) &&
				(wareHouse[j].itemDate.month == sentItems[i].itemDate.month) &&
				(strcmp(wareHouse[j].name, sentItems[i].name) == 0))
			{
				wareHouse[j].countSupply = sentItems[i].countSupply;
			}
		}
	}
}
/**
* @brief The sent function. gets an order to the wareHouse.
*        checks if it can manage it and carry it out while
*        deleting items that got runned out.
*
* @param wareHouse[] our current warehouse stats.
* @param sentFilePath[] a path to our file with the order for our wareHouse.
* @return True if the sent proccess went well otherwize sent back ERROR.
*/
int sent(item wareHouse[], char sentFilePath[])
{
	FILE *fp = fopen(sentFilePath, "r");
	if (fp == NULL)
	{
		printFileError(errno, sentFilePath);
		return ERROR;
	}

	//Preparing the line to read
	//the orders
	char line[LINE_LENGTH];
	//Divide the orders to barcode and ammount.
	char devideBy[4] = DIVIDE_BY;
		//verify if we encounter an error of unknow
	//file format during the process we know to stop.
	int fileFormatError = STILL_IN_PROCCESS;
	char *token;
	int barcode;
	float sentAmount;
	int amountOfSentItems;
	int result;
	int countLines = 0;

	while (fgets(line, LINE_LENGTH, fp) != NULL)
	{
		item sentItems[MAXIMUM_FILE_SIZE];
		//Verify sent file structre .
		if ((result = sscanf(line, "%d%f\n", &barcode, &sentAmount)) != 2)
		{
			if (line[0] == EMPTY_LINE)
			{
				continue;
			}
			fileFormatError = END_PROCCESS;
			break;
		}
		token = strtok(line, devideBy);
		if (token == NULL || strlen(token) != BARCODE_LIMIT)
		{
			fileFormatError = END_PROCCESS;
			break;
		}
		barcode = atoi(token);
		//If we have to much parameters in the order.
		token = strtok(NULL, devideBy);
		if (token == NULL || atof(token) < 0)
		{
			fileFormatError = END_PROCCESS;
			break;
		}
		else if (atof(token) < PRECISION)
		{
			countLines++;
			continue;
		}
		sentAmount = atof(token);

		token = strtok(NULL, devideBy);
		if (token != NULL)
		{
			fileFormatError = END_PROCCESS;
			break;
		}

		//condition for no supply.
		amountOfSentItems = locateSentItems(wareHouse, sentItems, barcode);
		//If there is no item at all with the wanted barcode.
		//return error if we don't have enough supply

		if (amountOfSentItems == 0 ||
			updateSentItemsSupply(sentItems, amountOfSentItems, sentAmount) == ERROR)
		{
			printf("not enough items in warehouse\n");
			fclose(fp);
			return (ERROR);
		}

		//Updating the warehouse with the current amount of each item.
		updateOriginalWareHouse(wareHouse, sentItems, amountOfSentItems);
		countLines++;

	}
	fclose(fp);

	if (countLines == 0 || fileFormatError == END_PROCCESS)
	{
		printf("unknown file format\n");
		return ERROR;
	}
	//If all went well.
	return (SUCSSES);

}

/**
* @brief Clean helper that do the actual cleanning of the warehouse.
*
* @param wareHouse[] our current warehouse stats.
* @param year the year part of the expration date..
* @param month the month part of the expration date.
* @param isMonth a true false value(0/1) indicates if I have a month value
*        that i should looking out for in the cleanning process.
*/
void cleanExpiredAndEmptyItem(item wareHouse[], int year, int month, int isMonth)
{
	int i;
	for (i = 0; i < countItems; i++)
	{
		//We need to clean runned up items anyway.
		if (wareHouse[i].countSupply <= PRECISION)
		{
			wareHouse[i].countSupply = EMPTY_ITEM;
		}
		//If we have an expration date.
		if (year != 0)
		{
			if (wareHouse[i].itemDate.year < year)
			{
				wareHouse[i].countSupply = EMPTY_ITEM;
			}
			//By multiplay in is month i know if i should take notice to the month value.
			//If  my is month value is zero this if will never triggered cuz month is always
			//more than 0.
			else if ((wareHouse[i].itemDate.year == year) &&
			 		(wareHouse[i].itemDate.month < month*isMonth))
			{
				wareHouse[i].countSupply = EMPTY_ITEM;
			}
		}
	}


}
/**
* @brief The clean function triggred when we get clean order
*        in the arguments cleaning expired and runnedout items.
*
* @param wareHouse[] our current warehouse stats.
* @param dateString[] our experation date.
*/
int clean(item wareHouse[], char dateString[])
{

	//Extract the items,
	int year;
	int month;
	sscanf(dateString, "%d-%d", &year, &month);
	if (year < 0)
	{
		return ERROR;
	}

	//If month is zero sending not to consider it.
	cleanExpiredAndEmptyItem(wareHouse, year, month, month != 0);
	return (SUCSSES);

}
/**
* @brief The main function. recevied the arguments of the warehouse
*        with an order the main activates the functions which manipulating
*        and updating the warehouse according to the excersize instructions
*
* @param argv[] an array of the arguments - warehouse order....
* @param argc size of the array.
*/
int main(int argc, char *argv[])
{

	//If we didn't get enough arguments.
	if (argc != NUMBER_OF_RCQUIRED_ARGUMENTS)
	{

		printf("USAGE: waredb <db file> <command> <command arg file>\n");
		return (ERROR);

	}


	item wareHouse[MAXIMUM_FILE_SIZE];

	FILE *fp = fopen(argv[1], "r");
	//Build warehouse from first file.
	if (fp == NULL)
	{

		printFileError(errno, argv[1]);
		return ERROR;
	}
	else
	{
		//Building the wareHouse
		countItems = 0;
		if (addItems(wareHouse, argv[1]) == ERROR)
		{
			return ERROR;
		}
	}

	fclose(fp);
	//Reading the command value.

	char* order = argv[2];
	if (strcmp(RECEIVED_ITEMS, order) == 0)
	{
		if (received(wareHouse, argv[3]) == ERROR)
		{

			return ERROR;
		}
	}
	else if (strcmp(SENT_ITEMS, order) == 0)
	{

		if (sent(wareHouse, argv[3]) == ERROR)
		{
			return (ERROR);
		}


	}
	else if (strcmp(CLEAN_ITEMS, order) == 0)
	{


		if (checkDateStructure(argv[3]) == ERROR || clean(wareHouse, argv[3]) == ERROR)
		{

			printf("USAGE: waredb <db file> <command> <command arg file>\n");

			return ERROR;
		}


	}
	else
	{

		printf("USAGE: waredb <db file> <command> <command arg file>\n");
		return ERROR;
	}
	//Writing back to the file.
	if ((fp = fopen(argv[1], "w+")) != NULL)
	{

		int i = 0;
		while (i < countItems)
		{
			if (wareHouse[i].countSupply > PRECISION)
			{
				fprintf(fp, "%s\t%d\t%.3f\t%d-%d\n", wareHouse[i].name, wareHouse[i].barcode, \
					wareHouse[i].countSupply, wareHouse[i].itemDate.year, \
					wareHouse[i].itemDate.month);

			}
			i++;

		}
		fclose(fp);
		return (SUCSSES);
	}
	else
	{
		printFileError(errno, argv[1]);
		return ERROR;
	}
	//In case of warehouse which contains files with less the 0.001 amount.
	if (countItems == 0)
	{
		fclose(fopen(argv[1], "w"));
	}

	return SUCSSES;

}