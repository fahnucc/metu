#include "the1.h"
#include <iostream>

void swapHelper(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void sillyHelper(int* arr, int start, int end, long &comparison, long & swap, int size, int &calls) 
{

	int i, j;
	
	if(size < 4){
	    for (i = start; i < end; i++){
            for (j = 0; j < end - start; j++){
                if (arr[i+j] > arr[i+j+1]){
                    swapHelper(&arr[i+j], &arr[i+j+1]);
                    swap++;
                }
                
            }
        }
        comparison++;
	}else{
	    
	    sillyHelper(arr, start + 0, start + size/2-1, comparison, swap, size/2, calls);
	    sillyHelper(arr, start + ((size/4)), start + (3*size/4)-1, comparison, swap, size/2, calls);
	    sillyHelper(arr, start + (size/2), start + size-1, comparison, swap, size/2, calls);
	    sillyHelper(arr, start + 0, start + size/2-1, comparison, swap, size/2, calls);
	    sillyHelper(arr, (start + (size/4)), start + (3*size/4)-1, comparison, swap, size/2, calls);
	    sillyHelper(arr, start + 0, start + size/2-1, comparison, swap, size/2, calls);
	    calls += 6;
	    
	    
	}
}

//You can add your own helper functions

int sillySort(int* arr, long &comparison, long & swap, int size) 
{

    int num_of_calls=1;
	
	sillyHelper(arr, 0, size-1, comparison, swap, size, num_of_calls);
	
	return num_of_calls;
}

void crossHelper(int* arr, int start, int end, long &comparison, int size, int &calls) 
{
	int i, j;

	if(size < 4){
	    for (i = start; i < end; i++){
            for (j = 0; j < end - start; j++){
                if (arr[i+j] >= arr[i+j+1]){
                    swapHelper(&arr[i+j], &arr[i+j+1]);
                }
                
            }
        }
        comparison++;
	}else{
	    
	    crossHelper(arr, start + 0, start + size/4 - 1, comparison, size/4, calls);
	    crossHelper(arr, (start + (size/4)), start + (size/2)-1, comparison, size/4, calls);
	    crossHelper(arr, (start + (size/2)), start + (3*size/4)-1, comparison, size/4, calls);
	    crossHelper(arr, start + (3*size/4), start + size-1, comparison, size/4, calls);
	    calls += 4;
	    
	}


}
int crossMergeSort(int *arr, long &comparison, int size)
{
	
	int num_of_calls=1;
	int h1[size/2];
	int h2[size/2];
	
	if(size == 1){
	}
	else if(size == 2){
	    if(arr[0] > arr[1]){
	       swapHelper(&arr[0], &arr[1]);
	    }
	    comparison++;
	}else{
	    crossHelper(arr, 0, size, comparison, size, num_of_calls);


	    
	}


	
	return num_of_calls;
	
}