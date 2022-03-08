#include "the3.h"

// do not add extra libraries here


/*
    arr       : array to be sorted, in order to get points this array should contain be in sorted state before returning
    ascending : true for ascending, false for descending 
    n         : number of elements in the array
    l         : the number of characters used in counting sort at each time
    
    you can use ceil function from cmath
    
*/



int radixSort(std::string arr[], bool ascending, int n, int l){
    
    int result = 0;
    int stringLength = arr[0].length();
    int numArray[n][stringLength];
    
    for(int i=0; i < n; i++){
        int newArray[stringLength];
        for(int k=0; k < stringLength; k++){
            numArray[i][k] = int(arr[i][k]) - 65;
        }
    }
    std::cout<<"[";
    for(int i=0; i<n; i++){
        std::cout<<"[";
        for(int k=0; k<stringLength; k++){
            std::cout<<std::to_string(numArray[i][k])<<", ";
        }
        std::cout<<"]";
    }
    std::cout<<"]";
    return result; 
    
}
