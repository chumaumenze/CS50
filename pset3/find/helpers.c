/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
//#include <cs50.h>
#include <string.h>
#include <stdio.h>

#include "helpers.h"

int get_int()
{
    int temp;

    scanf("%d", &temp);
    return temp;
}

/**
 * Returns true if value is in array of n values, else false.
 */
int search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    int start = 0;
    int end = n - 1;
    int middle = 0;
    middle = (start + middle) / 2;
    for(int i = n; (i != 0); i /= 2)
    {
        if(values[middle] == value)
        {
            return 1;
        }
        if(values[middle] < value)
        {
            end = middle - 1;
        }
        else
        {
            start = middle + 1;
        }
    }
    end++;
    return 0;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int size = n;
    int max_key = 0;

    // determine the maximum key value
    for (int i = 0; i < size; ++i)
    {
        if (values[i] > max_key)
        {
            max_key = values[i];
        }
    }

    printf("max_key is %d\n", max_key);
    //array to count values
    int count[max_key + 1];
    // array to store sorted values
    int output[size];

    //printf("sizeof count: %lu, output: %lu, max_key: %d\n", sizeof count, sizeof output, max_key);

    // resetting array values to zeros
    memset(count, 0, sizeof count);
    memset(output, 0, sizeof output);

    for(int i = 0; i < size - 1; i++)
    {
        // count each value in the array
        count[values[i]]++;
        printf("count[values[%d]]: %d\n", i, count[values[i]]);
    }

    // performs a prefix sum computation to determine the starting position
    for(int i = 1; i < size - 1; ++i)
    {
        count[i + 1] += count[i]; //+ count[i + 1];
    }

    // sanity check
    for(int i = size / 3; i < size; ++i)
    {
        printf("count[%d]: %d\n", i, count[i]);
    }

    for(int i = 0; i < size - 1; i++)
    {
        // insert values in output array using their values as key index to the count array
        output[count[values[i]]] = values[i];
        printf("Sorting... %d\n", values[i]);
    }

    // for(int i = 0; i < size - 1; i++)
    // {
    //     printf("output[%d] = %d\n", i, output[i]);
    // }

    // copy values back to original array
    memcpy(values, output, size);

    for (int i = 0; i < size; ++i)
    {
        printf("values[%d] = %d\n", i, values[i]);
    }
}
