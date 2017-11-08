/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <string.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    int start = 0; //first index of array
	int end = n; //last index of array
		
		while (n >= 0)
		{
				//discontinue search once we've complete the search
				if(n == 0)
				{
						break;
				}
				int middle = (start + end) / 2; //middle of array
				//check if middle is the value we are searching
				if (value == values[middle])
				{
						return true;
				}
				//check left of array if middle value is less than target value 
				else if(value < values[middle])
				{
						end = middle - 1; //we shift the end value one step to the left of middle
				}
				//check right of array if middle value is less than target value 
				else if( value > values[middle])
				{
						start = middle + 1; //we shift the start value one step to the right of middle
				}
				//split the array in half
				n /= 2;
		}
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement a sorting algorithm
		//int *values_count = (int *) malloc((int) sizeof(int) * (n + 2)); //array to store the count of each values
		int values_count[65536];
		memset(values_count, 0, (int) sizeof(values_count)); //reseting the array to zero
		//int *values_out = (int *) malloc((int) sizeof(int) * (n + 2)); //array to store the output
		int values_out[65536];
		memset(values_out, 0, (int) sizeof(values_out)); //reseting the array to zero
		
		//counting the values of the unsorted array (using the value as index)
		for(int i = 0; i < n; i++)
		{
				values_count[values[i]]++;
		}
		
		//for debugging
		for(int i = 0; i < 65536; i++)
		{
				printf("values_count[%d] is %d\n", i, values_count[i]);
		}
		
		//getting and index for to sort the array by adding adjacent values in the array
		for(int i = 1; i < n + 2; i++)
		{
		   values_count[i+1] = values_count[i] + values_count[i + 1];
		}
		
		printf("\n");
		for(int i = 0; i < 31; i++)
		{
				printf("values_count[%d] is %d\n", i, values_count[i]);
		}
		
		for(int i = 0; i < n; i++)
		{
				values_out[values_count[values[i]]] = values[i];
				values_count[values[i]]--;
		}
		
		printf("cloning\n");
		
		//copyin the values of the sorted array to the initially unsorted array
		memcpy(values, values_out, (int) sizeof(int) * n);
		//free(values_out);
		//free(values_count);
		
		//for debugging
		for(int i = 0; i < 31; i++)
		{
				printf("values[%d] = %d\n", i, values[i]);
		}
    return;
}
