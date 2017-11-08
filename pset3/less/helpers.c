/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
//#include <cs50.h>

#include <stdio.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
int search(int value, int values[], int n)
{
	printf("\nsearching...\n");
    // TODO: implement a searching algorithm
    int size = n;
    int start = 0;
    int end = n;
    int middle = (start + end) / 2;

    while (size != 0)
    {
    	if(value == values[middle])
    	{
    		return 1;
    	}
    	else if(value < values[middle])
    	{
    		end = middle - 1;
    	}
    	else if(value > values[middle])
    	{
    		start = middle + 1;
    	}

		middle = (start + end) / 2;
		size /= 2;
    }
    return 0;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement a sorting algorithm
	int swaps = 1;
    while(swaps != 0)
    {
		swaps = 0;
		for(int i = 0; i <= n - 1; i++)
		{
			if(values[i] > values[i + 1])
			{
				// swapping
				printf("swapping values[%d]:%d with values[%d]:%d\n", i, values[i], i + 1, values[i + 1]);
				values[i] = values[i] ^ values[i + 1];
				values[i + 1] = values[i] ^ values[i + 1];
				values[i] = values[i] ^ values[i + 1];
				swaps++;
			}
		}
	}
	for(int i = 0; i < n; i++)
		printf("%d / ", values[i]);
	return;
}
