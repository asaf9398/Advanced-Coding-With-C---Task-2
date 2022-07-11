#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define INIT_SIZE 1//minimun string size
#define START_OF_ARR 0
#define SAME 0
#define END_OF_STRING '\0'
#define MULTIPLY_ARRAY_BY_TWO 2
#define NONE 0
#define MINUS_CHAR '-' 
#define ZERO_CHAR '0'


typedef struct monom {
	int coefficient;
	int power;
}Monom;

Monom* getPolynom(int* ppolynomSize);
char* getStringFromUser();
Monom* getMonomArr(char* data,int* size);
void checkAllocStr(char* str);
void checkAllocMonom(Monom* monom);
Monom* getFullPolynom(Monom* tmpMonom, int* size);
void copyArr(Monom dest[], Monom src[], int size);
void merge(Monom a1[], int n1, Monom a2[], int n2, Monom res[]);
void mergeSort(Monom* arr, int size);
void printPolyMul(Monom* polynom1, int size1, Monom* polynom2, int size2);
void printPolynom(Monom* polynom, int size);
void printPolySum(Monom* polynom1, int polynom1Size, Monom* polynom2, int polynom2Size);
Monom* checkForPartnerToSum(Monom* needParter, Monom* polynom2, int polynom2Size);
int strToInteger(char* str);

void main()
{
	Monom* polynom1, * polynom2;             // The input polynoms
	unsigned int polynom1Size, polynom2Size; // The size of each polynom

	printf("Please enter the first polynom:\n");
	polynom1 = getPolynom(&polynom1Size);   // get polynom 1
	printf("Please enter the second polynom:\n");
	polynom2 = getPolynom(&polynom2Size);   // get polynom 2
	printf("The multiplication of the polynoms is:\n"); // print the multiplication
	printPolyMul(polynom1, polynom1Size, polynom2, polynom2Size);
	printf("\n");

	printf("The sum of the polynoms is:\n"); // print the sum
	printPolySum(polynom1, polynom1Size, polynom2, polynom2Size);
	printf("\n");

	free(polynom1); // releasing all memory allocations
	free(polynom2);
}
//The following function gets 2 polynoms and prints their sum
void printPolySum(Monom * polynom1, int polynom1Size, Monom* polynom2, int polynom2Size)
{
	Monom* polyOfSum = (Monom*)malloc(sizeof(Monom) * (polynom1Size + polynom2Size));
	int physSize = polynom1Size + polynom2Size;
	checkAllocMonom(polyOfSum);
	int i,ind=NONE;
	Monom* partner;
	for (i = START_OF_ARR; i < polynom1Size; i++)
	{
		partner = checkForPartnerToSum(&polynom1[i],polynom2,polynom2Size);//checking if there is parter for sum in the other array
		polyOfSum[ind].coefficient = polynom1[ind].coefficient;
		polyOfSum[ind].power = polynom1[ind].power;
		if (partner != NULL)
		{
			polyOfSum[ind].coefficient += partner->coefficient;
		}
		ind++;
	}
	//checking for partners on the first array.If you don't have any partner you'll come into the sum array(if you had a partner your'e already in)
	for (i = START_OF_ARR; i < polynom2Size; i++)
	{
		partner = checkForPartnerToSum(&polynom2[i],polynom1,polynom1Size);
		if (partner == NULL)
		{
			polyOfSum[ind].coefficient = polynom2[i].coefficient;
			polyOfSum[ind].power = polynom2[i].power;
			ind++;
		}
		
	}
	physSize = ind;
	polyOfSum = (Monom*)realloc(polyOfSum, sizeof(Monom)*physSize);
	checkAllocMonom(polyOfSum);
	Monom* fullSumPoly = getFullPolynom(polyOfSum, &physSize);
	mergeSort(fullSumPoly, physSize);
	printPolynom(fullSumPoly, physSize);
	free(polyOfSum);
	free(fullSumPoly);




}
//The following function gets an adress of monom + polynom to search a partner for him(with size)
//The function will return the adress of the parter or NULL if he doesn't have any partner
Monom* checkForPartnerToSum(Monom * needParter, Monom * polynom2,int polynom2Size)
{
	int findThisPower = needParter->power;
	int i;
	for ( i = START_OF_ARR; i < polynom2Size; i++)
	{
		if (findThisPower == polynom2[i].power)
		{
			return &polynom2[i];
		}
	}
	return NULL;

}

//The following function gets 2 polynoms and prints their multiplication
void printPolyMul(Monom* polynom1, int size1, Monom* polynom2, int size2)
{
	Monom* tmpArr = (Monom*)malloc(sizeof(Monom) * (size1 * size2));//array for multiplication outcome
	checkAllocMonom(tmpArr);
	int phySize = size1 * size2;
	int i,k,ind=START_OF_ARR;

	//multipling the monoms into one araay
	for (i = START_OF_ARR; i < size1; i++)
	{
		for (k = START_OF_ARR; k < size2; k++)
		{
			tmpArr[ind].coefficient = polynom1[i].coefficient * polynom2[k].coefficient;//multipling the coefficient
			tmpArr[ind].power = polynom1[i].power + polynom2[k].power;//summing the powers
			ind++;
		}
	}
	//creating a new sorted array for the updated polynom
	Monom * fullMultPoly=getFullPolynom(tmpArr,&phySize);
	checkAllocMonom(fullMultPoly);
	mergeSort(fullMultPoly, phySize);
	printPolynom(fullMultPoly,phySize);
	free(fullMultPoly);
	free(tmpArr);
}
//The following function gets a polynom + size and prints it
void printPolynom(Monom* polynom, int size)
{
	int i;
	
	for (i = START_OF_ARR; i < size; i++)
	{
		if (i != START_OF_ARR)
		{
			if (polynom[i].coefficient > 0)
			{
				printf(" +");
			}
			if (polynom[i].coefficient < 0)
			{
				printf(" -");
			}
			
		}
		if (polynom[i].power == NONE)
		{
			if (polynom[i].coefficient>0)
			{
				printf(" %d", polynom[i].coefficient);
			}
			else
			{
				printf(" %d", (-1)*polynom[i].coefficient);
			}
		}
		else
		{
			if (polynom[i].power > 1)
			{
				if (polynom[i].coefficient > 1)
				{
					//if the coefficient is positive than add + to the printing 
					printf(" %dx^%d", polynom[i].coefficient, polynom[i].power);
				}
				if (polynom[i].coefficient < -1)
				{ //if the coefficient is negative than add - to the printing 
					printf(" %dx^%d", (-1) * polynom[i].coefficient, polynom[i].power);
				}
				if (polynom[i].coefficient == 1)
				{ //if the coefficient is negative than add - to the printing 
					if (i!=0)
					{
						printf(" x^%d", polynom[i].power);
					}
					else
					{
						printf("x^%d", polynom[i].power);
					}
					
				}
				if (polynom[i].coefficient == -1)
				{ //if the coefficient is negative than add - to the printing 
					if (i != 0)
					{
						printf(" x^%d", polynom[i].power);
					}
					else
					{
						printf("- x^%d", polynom[i].power);
					}
				}
			}
			if (polynom[i].power == 1)
			{
				if (polynom[i].coefficient > NONE)
				{
					//if the coefficient is positive than add + to the printing 
					printf(" %dx", polynom[i].coefficient);
				}
				else
				{ //if the coefficient is negative than add - to the printing 
					printf(" %dx", (-1) * polynom[i].coefficient);
				}
			}
		}
		
	}
}

//The following function get an input from the user and returns polynom
Monom* getPolynom(int* ppolynomSize)
{
	int size;
	char * tmpStr=getStringFromUser();
	Monom* tmpMonom = getMonomArr(tmpStr,&size);
	free(tmpStr);//getting the heap free from this array
	Monom* polynom = getFullPolynom(tmpMonom, &size);
	free(tmpMonom);//getting the heap free from this array
	*ppolynomSize = size;
	mergeSort(polynom,size);
	return polynom;

}
//The following function gets an array of monoms and getting together all the coefficients with the same power
//The function returns the updated array + updated size
Monom* getFullPolynom(Monom* tmpMonom, int* size)
{
	int tmpSize = *size;
	int i, k;
	for (i = START_OF_ARR;i < tmpSize;i++)
	{
		for ( k = i+1; k < tmpSize; k++)
		{
			if (tmpMonom[i].power == tmpMonom[k].power)
			{
				tmpMonom[i].coefficient+= tmpMonom[k].coefficient;//getting all the coefficients together
				tmpMonom[k].coefficient = NONE;
			}
		}
	}
	//counting the number of different coefficients 
	int counter = NONE;
	for (i = START_OF_ARR; i < tmpSize; i++)
	{
		if (tmpMonom[i].coefficient != NONE )
		{
			counter++;//counting the updated size
		}
	}
	*size = counter;//putting the updated size to the original variable
	
	//creating an updated polynom
	int ind = START_OF_ARR;
	Monom* polynom = (Monom*)malloc(sizeof(Monom) * counter);
	for (i = START_OF_ARR; i < tmpSize; i++)
	{
		if (tmpMonom[i].coefficient != NONE )
		{
			polynom[ind] = tmpMonom[i];
			ind++;//counting the updated size
		}
	}
	return polynom;

}
//The following function gets a string and returns an array of monoms
Monom* getMonomArr(char* data,int * size)
{
	int logSize = INIT_SIZE, physSize = INIT_SIZE;
	Monom* tmpMonom = (Monom*)malloc(sizeof(Monom) * physSize);//making an array for putting the monoms inside
	checkAllocMonom(tmpMonom);//checking for memory allocation
	char seps[] = " ";
	char* token=strtok(data,seps);
	int num;
	char* tmpStr;
	while (token != NULL)
	{
		if (logSize == physSize)//if the sizes are the same -> multiply the physical size by 2
		{
			physSize *= MULTIPLY_ARRAY_BY_TWO;
			tmpMonom = (Monom*)realloc(tmpMonom, sizeof(Monom)*physSize);
			checkAllocMonom(tmpMonom);//checking for memory allocation
		}
		//putting values into the monom array
		tmpStr = strcat(token, "\0");//making string for putting into monom
		num=strToInteger(tmpStr);//casting string to number 
		tmpMonom[logSize - 1].coefficient=num;
		token=strtok(NULL, seps);
		tmpStr = strcat(token, "\0");
		num = strToInteger(tmpStr);
		tmpMonom[logSize - 1].power = num;
		logSize++;
		token=strtok(NULL, seps);
	}
	logSize--;
	tmpMonom = (Monom*)realloc(tmpMonom, sizeof(Monom) * (logSize));//decreasing the array to the smallest length that needed
	*size = logSize;
	return tmpMonom;


}

//The following function gets a string and returns his value in int
int strToInteger(char* str)
{
	int i,num = 0;
	int len = strlen(str);
	for (i = 0; i < len; i++)
	{
		if (str[i] - MINUS_CHAR == SAME)
		{
			num += 0;
		}
		else
		{
			num += (str[i] - ZERO_CHAR) * pow(10 , (len - i - 1));
			
		}
		
	}
	if (str[START_OF_ARR] - MINUS_CHAR == SAME)
	{
		num *= -1;
	}
	return num;

}

//The following function gets an input from the user and puts it in a string
//The function also returns the string starting adress
char* getStringFromUser()
{
	int logSize = NONE, physSize = INIT_SIZE;
	char * tmpStr = (char*)malloc(sizeof(char));//making an array for getting the input from the user
	checkAllocStr(tmpStr);
	tmpStr[START_OF_ARR] = END_OF_STRING;
	char ch;
	scanf("%c", &ch);
	while (ch - '\n' != SAME)
	{
		if (logSize == physSize)//if the sizes are the same -> multiply the physical size by 2
		{
			physSize *= MULTIPLY_ARRAY_BY_TWO;
			tmpStr = (char*)realloc(tmpStr, physSize*sizeof(char));
			checkAllocStr(tmpStr);//checking if the allocation succeed

		}
		*(tmpStr+logSize) = ch;
		logSize++;
		scanf("%c", &ch);
	}
	tmpStr = (char*)realloc(tmpStr, (logSize+1)*sizeof(char));//decreasing the array to the smallest length that needed
	tmpStr[logSize] = END_OF_STRING;
	return tmpStr;

}
void checkAllocStr(char* str)
{
	if (str == NULL)
	{
		printf("Memory Allocation Failed!!, Exiting now ! ");
		exit(1);
	}
}
void checkAllocMonom(Monom* monom)
{
	if (monom == NULL)
	{
		printf("Memory Allocation Failed!!, Exiting now ! ");
		exit(1);
	}
}
//MergeSort by coefficient
void mergeSort(Monom* arr, int size)
{
	Monom* tmpArr = NULL;
	if (size <= 1)
		return;

	mergeSort(arr, size / 2);
	mergeSort(arr + size / 2, size - size / 2);

	tmpArr = (Monom*)malloc(size * sizeof(Monom));
	if (tmpArr)
	{
		merge(arr, size / 2, arr + size / 2, size - size / 2, tmpArr);
		copyArr(arr, tmpArr, size); // copy values from tmpArr to arr
		free(tmpArr);
	}
	else
	{
		printf("Memory allocation failure!!!\n");
		exit(1);	// end program immediately with code 1 (indicating an error)
	}
}

void merge(Monom a1[], int n1, Monom a2[], int n2, Monom res[])
{
	int ind1, ind2, resInd;
	ind1 = ind2 = resInd = 0;

	while ((ind1 < n1) && (ind2 < n2)) {
		if (a1[ind1].power > a2[ind2].power) {
			res[resInd] = a1[ind1];
			ind1++;
		}
		else {
			res[resInd] = a2[ind2];
			ind2++;
		}
		resInd++;
	}

	while (ind1 < n1) {
		res[resInd] = a1[ind1];
		ind1++;
		resInd++;
	}
	while (ind2 < n2) {
		res[resInd] = a2[ind2];
		ind2++;
		resInd++;
	}
}

//The following function copies the values from array src to dest
void copyArr(Monom dest[], Monom src[], int size)
{
	int i;

	for (i = 0; i < size; i++)
	{
		dest[i] = src[i];
	}
		
}