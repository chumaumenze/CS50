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
  scanf("%7d", &temp);
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
    for(int i = 0; i < n - 1; i++)
    {
      printf("%d / %d - values[%d] == output[%d] = %s\n", values[i], output[i], i, i, (values[i] == output[i]) ? "yes" : "no" );
    }
    
    return;
}
