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
    int count[n + 1];
    memset(count, 0, sizeof(int) * (n + 1));
    int output[n];
    memset(output, 0, sizeof(int) * 10);
  
    for(int i = 0; i < n; i++)
    {
      printf("1:count[%d] = %d\n1:output[%d] = %d\n", i, count[i], i, output[i]);
    }
  
    for(int i = 0; i < n - 1; i++)
    {
      count[values[i]]++;
    }
  
    for(int i = 0; i < n; i++)
    {
      printf("count[%d] = %d\n", i, count[i]);
    }
  
    for(int i = 2; i < n; ++i)
    {
      int temp = count[i] + count[i - 1];
      count[i] = temp;
    }
  
    for(int i = 0; i < n; i++)
    {
      printf("count[%d] = %d\n", i, count[i]);
    }
  
    for(int i = 0; i < n - 1; i++)
    {
      output[count[values[i]]] = values[i];
    }
  
    for(int i = 0; i < n - 1; i++)
    {
      printf("output[%d] = %d\n", i, output[i]);
    }
    memcpy(values, output, sizeof(int) * n);
    return;
}
