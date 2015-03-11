#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ROW 11
#define MAX_COLUMN 20
int binSearch(const char* str, char* arr[], int size)
{
    int first = 0;
    int last = size;
    int mid = (first + last)/2;
    int save_mid ;

    while(last - first)
    {
        save_mid = mid;
        while(!strcmp(arr[mid], "")){ /* if empty, do not count it */
            /* recalculate mid */
            mid++;
        }
        printf("save_mid %d, mid %d\n", save_mid, mid);
        if( !strcmp(str, arr[mid] )){
            printf("Found \"%s\" at index %d\n", str, mid);
            return EXIT_SUCCESS;
        }else if(strcmp(str, arr[mid]) < 0){
            last = save_mid ;
        }else if (strcmp, arr[mid] > 0){
            first = mid;
        }
        mid = (first+last)/2;
    }
    return EXIT_FAILURE;
}

int main()
{
    int i = 0;
    char* arr[MAX_ROW] ;
    
    for (i = 0; i<MAX_ROW; i++){
        arr[i] = (char *)malloc(MAX_COLUMN);
        if(!arr[i]){
            printf("Mem allocation failed\n");
        }
    }
    strncpy(arr[0], "at", MAX_COLUMN);
    strncpy(arr[1], "", MAX_COLUMN);
    strncpy(arr[2], "", MAX_COLUMN);
    strncpy(arr[3], "", MAX_COLUMN);
    strncpy(arr[4], "ball", MAX_COLUMN);
    strncpy(arr[5], "", MAX_COLUMN);
    strncpy(arr[6], "bbb", MAX_COLUMN);
    strncpy(arr[7], "car", MAX_COLUMN);
    strncpy(arr[8], "dad", MAX_COLUMN);
    strncpy(arr[9], "", MAX_COLUMN);
    strncpy(arr[10], "", MAX_COLUMN);
    
    /* print array */
    for(i = 0 ; i <MAX_ROW ; i++)
        printf("%s,", arr[i]);

    if(binSearch("ball", arr, 10) != EXIT_SUCCESS)
        printf("\nPattern not found\n");
    if(binSearch("at", arr, 10) != EXIT_SUCCESS)
        printf("\nPattern not found\n");
    if(binSearch("dad", arr, 10) != EXIT_SUCCESS)
        printf("\nPattern not found\n");
    if(binSearch("car", arr, 10) != EXIT_SUCCESS)
        printf("\nPattern not found\n");
}

