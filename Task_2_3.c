#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DELETE_FLAG '\n'
#define START_OF_ARR 0
#define SAME 0
#define LAST_CELL_ADD 1
#define END_OF_STRING '\0'
#define FIRST_CELL 0
#define MULTIPLY_ARRAY_BY_TWO 2
#define NONE 0 
#define INIT_SIZE 1


unsigned int  RemoveFromStrArray(char*** str_array, unsigned  int  str_array_size, char** ptr_to_chars_array);
char** setPtrToCharsArray(char** str_array);
void putEnterInCharPointerArray(char** ptr_to_chars_array);
void flagsCounterInStr(char* str, int* size, int* numOfFlags);
char* updatedStr(char* oldStr);
int removeEmptyStrings(char*** pstrArr,int size);
char** getStrArrayInput(int * str_array_size);
void printArray(char** strArray, int size);
void freeArray(char** strArray, int size);
void checkAllocCharP(char** pstr);
void checkAllocChar(char* pstr);
char* getStringFromUser();


int main()
{
	char** str_array;
	unsigned int str_array_size;
	char** ptr_to_chars_array;
	unsigned int res;
	str_array = getStrArrayInput(&str_array_size); // Get the size and strings from user (can't assume max size for each string)
	ptr_to_chars_array = setPtrToCharsArray(str_array);
	res = RemoveFromStrArray(&str_array, str_array_size, ptr_to_chars_array);
	printArray(str_array, str_array_size - res);
	// Free memory
	freeArray(str_array, str_array_size - res);
	free(ptr_to_chars_array);
}
void checkAllocChar(char* pstr)
{
	if (pstr == NULL)
	{
		printf("Memory allocation failed!! Exiting");
		exit(1);
	}
}
//The following function checks memory allocation for char* array
void checkAllocCharP(char ** pstr)
{
	if (pstr == NULL)
	{
		printf("Memory allocation failed!! Exiting");
		exit(1);
	}
}
//The following function frees all the strings in the array + the array itself
void freeArray(char** strArray, int size)
{
	int i;
	for ( i = START_OF_ARR; i < size; i++)
	{
		free(strArray[i]);
	}
	free (strArray);

}
//The following function prints the strings array
void printArray(char** strArray, int size)
{
	char* str;
	int i;
	for ( i = START_OF_ARR; i < size; i++)
	{
		printf("%s", strArray[i]);

	}

}

//The following function gets an input from the user and puts it in a string
//The function also returns the string starting adress
char* getStringFromUser()
{
	int logSize = NONE, physSize = INIT_SIZE;
	char* tmpStr = (char*)malloc(sizeof(char));//making an array for getting the input from the user
	checkAllocChar(tmpStr);
	tmpStr[START_OF_ARR] = END_OF_STRING;
	char ch;
	scanf("%c", &ch);
	scanf("%c", &ch);
	while (ch - '\n' != SAME)
	{
		if (logSize == physSize)//if the sizes are the same -> multiply the physical size by 2
		{
			physSize *= MULTIPLY_ARRAY_BY_TWO;
			tmpStr = (char*)realloc(tmpStr, physSize * sizeof(char));
			checkAllocChar(tmpStr);//checking if the allocation succeed

		}
		*(tmpStr + logSize) = ch;
		logSize++;
		scanf("%c", &ch);
	}
	tmpStr = (char*)realloc(tmpStr, (logSize + 1) * sizeof(char));//decreasing the array to the smallest length that needed
	tmpStr[logSize] = END_OF_STRING;
	return tmpStr;

}

//The following function get size + strings from user and returns the strings array
char** getStrArrayInput(int* str_array_size)
{
	int size;
	scanf("%d", &size);//number of strings
	char** stringsArr = (char**)malloc(sizeof(char*) * size);
	checkAllocCharP(stringsArr);
	int i;
	for (i = START_OF_ARR; i < size; i++)
	{
		stringsArr[i] = getStringFromUser();//getting strings into the array
	}
	*str_array_size = size;
	return stringsArr;
}

char** setPtrToCharsArray(char** str_array)
{
	char** res;
	int size, i;
	int str_array_row, str_array_col;
	scanf("%d", &size); // Get number of ptrs
	res = (char**)malloc(sizeof(char*) * (size + 1)); // Add 1 for NULL at the end
	checkAllocCharP(res);
	for (i = 0; i < size; i++)
	{
		scanf("%d", &str_array_row);
		scanf("%d", &str_array_col);
		res[i] = str_array[str_array_row] + str_array_col;
	}
	res[size] = NULL; // Set the last one to NULL
	return res;
}

/*The following function get an adress of string's array + number of strings + array of pointers to char	
The function deletes the characters that in the array and updates the strings array according to the strings that left.
The function returns the number of strings that has been deleted from the array*/
unsigned int  RemoveFromStrArray(char*** str_array, unsigned  int  str_array_size, char** ptr_to_chars_array)
{
	char** stringsArray = *str_array;
	putEnterInCharPointerArray(ptr_to_chars_array);
	int i;
	char* oldStr;
	char *newStr;
	int numOfDeleted;
	for ( i = START_OF_ARR; i < str_array_size; i++)
	{
		oldStr = stringsArray[i];
		newStr = updatedStr(oldStr);
		//free(oldStr);
		strcpy(stringsArray[i] , newStr);
	}
	numOfDeleted=removeEmptyStrings(&stringsArray,str_array_size);
	return numOfDeleted;

}
/*The following function gets adress of strings array and deletes the strings that are empty(has \0 on their first cell)
The function returns the number of strings she deleted*/
int removeEmptyStrings(char*** pstrArr,int size)
{
	char** strArray = *pstrArr;
	int counter = 0;//counting how many cells has been deleted
	char** newStrArray = (char**)malloc(sizeof(char*) * size);//making array with the same size
	checkAllocCharP(newStrArray);
	int i, ind = START_OF_ARR;
	for (i = START_OF_ARR; i < size; i++)
	{
		if (strArray[i][FIRST_CELL] - END_OF_STRING == SAME)//if the string's first cell is \0 -> delete it 
		{
			//delete this cell
			counter++;//promote counter of the deleted cells
		}
		else
		{
			//if it's not the end of the string than copy it to updated array
			newStrArray[ind] = strArray[i];

		}
	}
	int newSize = size - counter;
	newStrArray = (char**)realloc(newStrArray, sizeof(char*) * newSize);
	checkAllocCharP(newStrArray);
	//coping the values from the updated array to the old one
	strArray= (char**)realloc(strArray, sizeof(char*) * newSize);
	checkAllocCharP(strArray);
	for ( i = START_OF_ARR; i < newSize; i++)
	{
		strArray[i] = (char*)malloc(sizeof(char) * (strlen(newStrArray[i]) + 1));
		strcpy(strArray[i] , newStrArray[i]);
	}
	free(newStrArray);
	return counter;
}

//The following function gets string and creates new string after deleted all the DELETE_FLAGS
//The function returns the adress of the new string
char* updatedStr(char* oldStr)
{
	int size, numOfFlags;
	flagsCounterInStr(oldStr, &size, &numOfFlags);
	char* newStr = (char*)malloc(sizeof(char) * (size - numOfFlags + LAST_CELL_ADD));
	checkAllocChar(newStr);
	int i,ind=START_OF_ARR;
	for ( i = START_OF_ARR; i < size; i++)
	{
		if (oldStr[i] - DELETE_FLAG != SAME)
		{
			newStr[ind] = oldStr[i];
			ind++;
		}
	}
	newStr[size - numOfFlags] = END_OF_STRING;
	return newStr;
}

//The following function gets a string and returns his size + how many DELETE_FLAGS he got in him
void flagsCounterInStr(char * str,int * size,int * numOfFlags)
{
	int len = strlen(str);//length of the str
	int counter = 0;//flags counter
	int i;
	for ( i = 0; i < len; i++)
	{
		if (str[i] - DELETE_FLAG == SAME)//if the character is our flag -> promote the counter
		{
			counter++;
		}
	}
	*size = len;
	*numOfFlags = counter;

}

/*The following function gets a char* array and puts DELETE_FLAG in all adresses's values */
void putEnterInCharPointerArray(char ** ptr_to_chars_array)
{
	char* curr;//using local pointer for running on the adresses
	int ind = START_OF_ARR;
	curr = ptr_to_chars_array[ind];
	while (curr!=NULL)
	{
		*curr = DELETE_FLAG;//putting into the char our DELETE_FLAG
		ind++;//running on the array
		curr = ptr_to_chars_array[ind];//updating the pointer in the array
	}

}
