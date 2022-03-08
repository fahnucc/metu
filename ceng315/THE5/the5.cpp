#include "the5.h"
// do not add extra libraries here

/*
W: the width of the field
L: the length of the field
partitions: 2D Boolean array for partitions, if a partition of w x l exists then partitions[x][y] is true.
numberOfCalls/numberOfIterations: method specific control parameter
*/



int recursiveMethod(int W, int L, bool** partitions, int* numberOfCalls){
    (*numberOfCalls)++;
	int wastedArea = 0;
	
	if(partitions[W][L]){
	    return wastedArea;
	}else{
	    wastedArea = W*L;
	    int wastedArea2 = W*L;
	    int wastedArea3 = W*L;
	    for(int M = 1; M <= W/2; M++){
	        int dummy = recursiveMethod(W - M, L, partitions, numberOfCalls) + recursiveMethod(M, L, partitions, numberOfCalls);
	        if(dummy < wastedArea2) wastedArea2 = dummy;
	    }
	    for(int N = 1; N <= L/2; N++){
	        int dummy = recursiveMethod(W, L - N, partitions, numberOfCalls) + recursiveMethod(W, N, partitions, numberOfCalls);
	        if(dummy < wastedArea3) wastedArea3 = dummy;
	    }
	    return std::min(wastedArea, std::min(wastedArea2, wastedArea3));
	}
}

int memoHelper(int W, int L, bool** partitions, int* numberOfCalls, int** memo){
	(*numberOfCalls)++;
	
	if(memo[W][L] != -1) return memo[W][L];
	else{
	    if(partitions[W][L]){
	        memo[W][L] = 0;
	    }else{
	        int wastedArea = W*L;
    	    int wastedArea2 = W*L;
    	    int wastedArea3 = W*L;
    	    for(int M = 1; M <= W/2; M++){
    	        int dummy = memoHelper(W - M, L, partitions, numberOfCalls, memo) + memoHelper(M, L, partitions, numberOfCalls, memo);
    	        if(dummy < wastedArea2) wastedArea2 = dummy;
    	    }
    	    for(int N = 1; N <= L/2; N++){
    	        int dummy = memoHelper(W, L - N, partitions, numberOfCalls, memo) + memoHelper(W, N, partitions, numberOfCalls, memo);
    	        if(dummy < wastedArea3) wastedArea3 = dummy;
    	    }
    	    memo[W][L] =  std::min(wastedArea, std::min(wastedArea2, wastedArea3));
    	}
	    return memo[W][L];
	}
	
}

int memoizationMethod(int W, int L, bool** partitions, int* numberOfCalls){
    (*numberOfCalls)++;
	int** memo = new int*[W+1];
	for(int i = 0; i < W+1; i++){
		memo[i] = new int[L+1];
		for (int j = 0; j < L+1; j++){
			memo[i][j] = -1;
		}
	}
	
	return memoHelper(W, L, partitions, numberOfCalls, memo);
}

int bottomHelper(int W, int L, bool** partitions, int* numberOfIterations, int** bottom){
	
	
	if(bottom[W][L] != -1) {
	    return bottom[W][L];
	    
	}
	else{
	    if(partitions[W][L]){
	        bottom[W][L] = 0;
	    }else{
	        int wastedArea = W*L;
    	    int wastedArea2 = W*L;
    	    int wastedArea3 = W*L;
    	    
    	    for(int M = 1; M < W; M++){
    	        (*numberOfIterations)++;
    	        int dummy = bottomHelper(M, L, partitions, numberOfIterations, bottom);
    	        if(dummy < wastedArea2) wastedArea2 = dummy;
    	    }
    	    for(int N = 1; N < L; N++){
    	        (*numberOfIterations)++;
    	        int dummy = bottomHelper(W, N, partitions, numberOfIterations, bottom);
    	        if(dummy < wastedArea3) wastedArea3 = dummy;
    	    } 
    	    
	
	    }
	    return bottom[W][L];
    }
}

int bottomUpMethod(int W, int L, bool** partitions, int* numberOfIterations){
	int** bottom = new int*[W+1];
	for(int i = 0; i < W+1; i++){
		bottom[i] = new int[L+1];
		for (int j = 0; j < L+1; j++){
			bottom[i][j] = -1;
		}
	}
	
	return bottomHelper(W, L, partitions, numberOfIterations, bottom);
}
