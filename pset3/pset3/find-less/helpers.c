/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
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
		int swaps = 1; //initialize the swaps counter to any number greater/less than zero
		while(swaps != 0)
		{
				//reset swaps to start counting swaps
				swaps = 0;
				for(int i = 0; i < n - 1; i++)
				{
						//compare the i and i+1th
						if(values[i] > values[i+1])
						{
								//swap the values and increment the swap counter
								values[i] = values[i] ^ values[i+1];
								values[i+1] = values[i] ^ values[i+1];
								values[i] = values[i] ^ values[i+1];
								swaps++;
						}
				}
		}
    return;
}
