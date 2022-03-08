//This file is entirely for your test purposes. 
//This will not be evaluated, you can change it and experiment with it as you want.
#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include "the1.h"

//the1.h only contains declaration of the function sillySort  and crossMergeSort which are:
//int sillySort(int* arr, long &comparison, long & swap, int size) ;
//int crossMergeSort(int *arr, long &comparison, int size);

using namespace std;

void randomFill(int*& arr, int size, int minval, int interval)
{
	arr = new int [size];
	for (int i=0; i <size; i++)
	{
		arr[i] = minval + (random() % interval);
	}
}

void print_to_file(int* arr, int size)
{
	ofstream ofile;
	ofile.open("sorted.txt");
	for(int i=0;i<size; i++) 
		ofile<<arr[i]<<endl;
}

void read_from_file(int*& arr, int& size)
{

	char addr[]= "input01.txt";
	ifstream infile (addr);
	
	if (!infile.is_open())
	{
		cout << "File \'"<< addr 
			<< "\' can not be opened. Make sure that this file exists." <<endl;
		return;
	}
	infile >> size;
	arr = new int [size];
	
	for (int i=0; i<size;i++) {
		
		infile >> arr[i];
	}
	
}
	void mergeArrays2(int* arr1, int* arr2, int n1, int n2, int* res, long &comparison, int &calls)
{
    int i = 0, j = 0, k = 0;

    while (i<n1 && j <n2)
    {
        if (arr1[i] < arr2[j])
            res[k++] = arr1[i++];
        else
            res[k++] = arr2[j++];
    }
 
    while (i < n1)
        res[k++] = arr1[i++];
 
    while (j < n2)
        res[k++] = arr2[j++];
        
    
}

void test()
{
	
	clock_t begin, end;
	double duration;
	
	//data generation and initialization- you may test with your own data
	long comparison=0;
	long swap=0;
	int num_of_calls;
	int size= 1 << 11;	 // maximum 2^11 for sillySort and 2^16 ( 1 << 16) for crossMergeSort 
	int minval=0;
	int interval= size*2; // you can try to minimize interval for data generation to make your code test more equality conditions
	int *arr;
	int arr2[] =  {0, -5, 2, 6, 4, 18, 22, -14, 0, -5, 2, 6, 4, 18, 22, -14, } ;
	
	//Randomly generate initial array:
	randomFill(arr, size, minval, interval);
	
	//Read the test inputs. input01.txt through input04.txt exists.
	//read_from_file(arr, size);
	
	//data generation or read end
	
	if ((begin = clock() ) ==-1) 
		cerr << "clock error" << endl;
	
	//Function call for the solution
	
	num_of_calls=sillySort(arr2, comparison, swap, 16) ;
	
	//num_of_calls=crossMergeSort(arr2, comparison, 16);
	//mergeArrays2(arr2, arr2+(size/2), 2, 2, h1, comparison, num_of_calls);
	for(int i=0; i< 16; i++){
	    cout << arr2[i] << ",";
	}
	//Function end
	
	if ((end = clock() ) ==-1) 
		cerr << "clock error" << endl;
	
	//Calculate duration and print output
	
	duration = ((double) end - begin) / CLOCKS_PER_SEC;
	cout << "Duration: " << duration << " seconds." <<endl;
	cout<<"Number of Comparisons: " << comparison <<endl;
	cout<<"Number of sillySort or crossMergeSort calls: " << num_of_calls <<endl;
	cout<<"Number of Swaps(0 for crossMergeSort): " << swap <<endl;
	print_to_file(arr,size);
	//Calculation and output end
	
}

int main()
{
	srandom(time(0));
	test();
	return 0;
}
	
	