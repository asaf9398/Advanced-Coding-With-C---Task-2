#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define SIZE 100
#define UP 1

void pointerSort(int* arr, unsigned int size,int ascend_flag, int*** pointers);

//int** pointerSort(int* arr, unsigned int size, int ascend_flag);
void mergeSort(int** arr, int size, int ascend_flag);
void mergeUp(int** a1, int n1, int** a2, int n2, int** res);
void mergeDown(int** a1, int n1, int** a2, int n2, int** res);
void copyArr(int** dest, int** src, int size);
void printPointers(int** pointers, int size);

void main()
{
	unsigned int size, i;
	int arr[SIZE];
	int** pointers;
	int ascend_flag;
	printf("Please enter the number of items:\n");
	scanf("%u", &size);
	for (i = 0; i < size; i++)
		scanf("%d", &arr[i]);
	scanf("%d", &ascend_flag);
	pointerSort(arr, size, ascend_flag, &pointers);
	printf("The sorted array:\n"); //Print the sorted array
	printPointers(pointers, size);
	free(pointers);
}

void pointerSort(int* arr, unsigned int size, int ascend_flag, int*** pointers)
{
	//making an array of int* for the pointers
	int** pointersArr = (int**)malloc(sizeof(int*) * size);
	if (pointersArr == NULL)
	{
		printf("Memory allocation failure!!!\n");
		exit(1);	// end program immediately with code 1 (indicating an error)
	}
	int i;
	//initializing the array to be the addreses of the variables of arr
	for (i = 0; i < size; i++)
	{
		pointersArr[i] = &arr[i];
	}
	mergeSort(pointersArr, size, ascend_flag);
	*pointers=pointersArr;

}

//mergeSort by using ascens_flag
void mergeSort(int** arr, int size, int ascend_flag)
{
	int** tmpArr = NULL;
	if (size <= 1)
		return;

	mergeSort(arr, size / 2, ascend_flag);
	mergeSort(arr + size / 2, size - size / 2, ascend_flag);

	tmpArr = (int**)malloc(size * sizeof(int*));
	if (tmpArr != NULL)
	{
		if (ascend_flag == UP)
		{
			mergeUp(arr, size / 2, arr + (size / 2), size - (size / 2), tmpArr);
		}
		else
		{
			mergeDown(arr, size / 2, arr + (size / 2), size - (size / 2), tmpArr);
		}

		copyArr(arr, tmpArr, size); // copy values from tmpArr to arr
		free(tmpArr);
	}
	else
	{
		printf("Memory allocation failure!!!\n");
		exit(1);	// end program immediately with code 1 (indicating an error)
	}
}

void mergeUp(int** a1, int n1, int** a2, int n2, int** res)
{
	int ind1, ind2, resInd;
	ind1 = ind2 = resInd = 0;

	while ((ind1 < n1) && (ind2 < n2)) {
		if (*a1[ind1] <= *a2[ind2]) {
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

void mergeDown(int** a1, int n1, int** a2, int n2, int** res)
{
	int ind1, ind2, resInd;
	ind1 = ind2 = resInd = 0;

	while ((ind1 < n1) && (ind2 < n2)) {
		if (*a1[ind1] > *a2[ind2]) {
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
//coping the sorted arr to the unsorted
void copyArr(int** dest, int** src, int size)
{
	int i;

	for (i = 0; i < size; i++)
		dest[i] = src[i];
}

//printing all the numbers
void printPointers(int** pointers, int size)
{
	int i;
	int num;
	for (i = 0; i < size; i++)
	{
		num = *(pointers[i]);
		printf("%d ", num);
	}

}