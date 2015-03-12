#include <iostream>
#include <cstdlib>
using namespace std;

//When passed as argument , arr decays into pointer 
//and hence there is no way to find the size.
//need to pass the size as in param. same param is used
// to carr updated size for new array
int *remove_duplicate(int arr[], int *size)
{
	int i = 0;
	int k = 0;
	//check if incoming array is empty
	if(*size == 0)
	{
		cout << "Incoming array is empty, exiting..." << endl;
		return NULL;
	}

	// there will be atmost size elements in new array
	int *newarr = new int[*size];
	if(newarr == NULL)
	{
	   cout << "Memory allocation for new array failed " << endl;
	   return NULL;
	}

	newarr[k++] = arr[0];
	for(i = 0; i < *size - 1 ; i++)
	{
		if(arr[i] != arr[i+1]){
			newarr[k++] = arr[i+1];
		}
	}
	*size = k;
	return newarr;
}

int main(void)
{
    //1. Test empty array
	//int arr[] = {};
    //2. all repeating
	//int arr[] = {1,1,1,1,1};
    //3. initial few elements repeating
    //int arr[] = {2,2,2,4,5};
    //3. last few elements repeating
    //int arr[] = {2,4,5,5,5,5};
    //4. few elem repeating in beginning and few in the end
    //int arr[] = {2,2,2,4,7,5,5,5,5};

    //5. Testing given array in Question
	int arr[] = {1, 2, 3, 3, 3, 4, 4, 10, 13, 15, 15, 17};
	int size = sizeof(arr)/sizeof (arr[0]);
    if(size){
        cout << "Original array " << endl;	
        for(int i = 0; i < size; i++)
            cout << arr[i]  << " " ;
        cout << endl;
    }
	int *newarr = remove_duplicate(arr, &size); //size if in/out param
	if(newarr == NULL)
	{
		cout << "Received null buffer " << endl;
		return EXIT_FAILURE ;
	}
	cout << "Reduced array " << endl;	
	for(int i = 0; i < size; i++)
		cout << newarr[i]  << " " ;
	cout << endl;
	delete [] newarr;
	return EXIT_SUCCESS;
}
