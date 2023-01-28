#include "the4.h"



int recursive_sln(int i, int*& arr, int &number_of_calls){ //direct recursive
    number_of_calls+=1;

    int x, y;
    
    if(i == 0) return arr[0];
    if(i == 1) return std::max(arr[0], arr[1]);
    if(i == 2) return std::max(std::max(arr[0], arr[1]), arr[2]);
    
    x = recursive_sln(i - 3, arr, number_of_calls) + arr[i];
    y = recursive_sln(i - 1, arr, number_of_calls);
    
    return std::max(x, y);
}



int memoization_sln(int i, int*& arr, int*& mem){ //memoization

    int x, y;
    
    if(i == 0) {
        mem[0] = arr[0];
        return arr[0];
    }
    else if(i == 1) {
        mem[0] = arr[0];
        mem[1] = std::max(mem[0], arr[1]);
        return mem[1];
    }
    else if(i == 2) {
        mem[0] = arr[0];
        mem[1] = std::max(mem[0], arr[1]);
        mem[2] = std::max(std::max(arr[0], arr[1]), arr[2]);
        return mem[2];
    }
    else{
        

        if((i >= 3) && (mem[i-3] == -1)){
            x = memoization_sln(i-3, arr, mem) + arr[i];
        } else{
            x = mem[i-3] + arr[i];
        }
    
        if((i >= 3) && (mem[i-1] == -1)){
            y = memoization_sln(i-1, arr, mem);
        } else{
            y = mem[i-1];
        }
    
        mem[i] = std::max(y, x);
        return (mem[i]);

    }
    

}



int dp_sln(int size, int*& arr, int*& mem){ //dynamic programming

    if (size == 1) {
 
        mem[0] = arr[0];
    }
    
    else if (size == 2) {
 
        mem[0] = arr[0];
        mem[1] = std::max(arr[1], mem[0]);
    }
    else if (size >= 3) {
 
        mem[0] = arr[0];
        mem[1] = std::max(arr[1], arr[0]);
        mem[2] = std::max(std::max(arr[0], arr[1]), arr[2]);
 
        int k = 3;
 
        while (k < size) {
            mem[k] = std::max(mem[k - 1], arr[k] + mem[k - 3]);
            k++;
        }
    }
 
    return mem[size - 1];
}

