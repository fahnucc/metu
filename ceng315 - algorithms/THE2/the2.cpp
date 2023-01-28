#include "the2.h"
#include <iostream>
using namespace std;
//You may write your own helper functions here
void swapHelper(unsigned short & a, unsigned short & b, long& swap)
{
    unsigned short temp = a;
    a = b;
    b = temp;
    swap++;
}

int positive(int x)
{
    if(x<0){
        return -x;
    }
    else{
        return x;
    }
}

int partition(unsigned short* arr, long &swap, double & sum_dist, double & max_dist, int size){
    int pivot = arr[size - 1];
    int i = -1;
    for(int j = 0; j < size - 1; j++){
        if(arr[j] >= pivot){
            i = i+1;
            swapHelper(arr[i], arr[j], swap);
            sum_dist += positive(j - i);
            if(positive(j-i) > max_dist){
                max_dist = positive(j-i);
            }
        }
    }
    swapHelper(arr[i+1], arr[size - 1], swap);
    sum_dist += positive(size - i - 2);
    if(positive(size - i - 2) > max_dist){
        max_dist = positive(size - i - 2);
    }
    return i+1;
}

void helperQuick(unsigned short* arr, long &swap, double & sum_dist, double & max_dist, int size)
{
    if(size > 1){
        int p = partition(arr, swap, sum_dist, max_dist, size);
        helperQuick(arr, swap, sum_dist, max_dist, p);
        helperQuick(arr + p + 1, swap, sum_dist, max_dist, size - p- 1);
    }
}


int hoare(unsigned short* arr, long &swap, double & sum_dist, double & max_dist, int size){
    int pivot = arr[((size - 1)/2)];
    int i = -1;
    int j = size;
    
    while(true){
        j--;
        while(arr[j] < pivot){
            j--;
        }
        i++;
        while(arr[i] > pivot){
            i++;
        }
        if (i<j){
            swapHelper(arr[i], arr[j], swap);
            sum_dist += positive(j - i);
            if(positive(j - i) > max_dist){
                max_dist = positive(j - i);
            }
        }else{
            return j;
        }
    }
}

void helperHoare(unsigned short* arr, long &swap, double & sum_dist, double & max_dist, int size)
{
    if(size > 1){
        int p = hoare(arr, swap, sum_dist, max_dist, size);
        helperHoare(arr, swap, sum_dist, max_dist, p + 1);
        helperHoare(arr + p + 1, swap, sum_dist, max_dist, size - p- 1);
    }
}

void quickSort(unsigned short* arr, long &swap, double & avg_dist, double & max_dist, bool hoare, int size)
{
    if(size>0){
        if(hoare){
            double sum_dist = 0;
            helperHoare(arr, swap, sum_dist, max_dist, size);
            if(swap > 0){
                avg_dist = sum_dist / swap;
            }
        }
        else{
            double sum_dist = 0;
            helperQuick(arr, swap, sum_dist, max_dist, size);
            if(swap > 0){
                avg_dist = sum_dist / swap;
            }
        }
    }
	
}

int min(int x, int y){
    if(x < y){
        return x;
    }
    return y;
}

void partition3(unsigned short* arr, long &swap, long &comparison, int size, int &left, int &right){
    int i = 0;
    int j = 0;
    int p = size - 1;
    
    while(i < p){
        comparison++;
        if(arr[i] > arr[size-1]){
            swapHelper(arr[i], arr[j], swap);
            i++;
            j++;
        }else if(arr[i] == arr[size - 1]){
            comparison++;
            p--;
            swapHelper(arr[i], arr[p], swap);
        }else{
            comparison++;
            i++;
        }
    }
    int m = min(p-j, size-p);
    
    for(int k=0; k < m ; k++){
        swapHelper(arr[j + k], arr[size - m + k], swap);
    }
    left = j;
    right = p-j;
}

void helper3way(unsigned short* arr, long &swap, long &comparison, int size)
{
    if(size > 1){
        int right, left;
        partition3(arr, swap, comparison, size, left, right);
        helper3way(arr, swap, comparison, left);
        helper3way(arr + size - right, swap, comparison, right);
    }
}

void quickSort3(unsigned short *arr, long &swap, long &comparison, int size) {
    helper3way(arr, swap, comparison, size);
}