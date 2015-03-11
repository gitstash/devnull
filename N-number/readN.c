#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int comparison(const void *x, const void *y)
{
    if(*(int *)x == *(int *)y)
        return 0;
    return  *(int *)x > *(int *)y  ? 1 : -1;
}

/* assuming all the numbers are distinct */
int binsearch(int num, int a[], int size)
{
    int first = 0;
    int last = size;
    int mid = ( last - first )/2;
    while((last - first )/2 ){
        if(num > a[mid]){
            first = mid;
        }else if(num < a[mid]) {
            last = mid;
        }
        mid = first + (last - first)/2;
    }
    return mid;
}


/* code for reading from a file */
int readFile(const char *filename, int **p_arr, int *size)
{
    FILE *fp = fopen(filename, "r");

    /*! 10 for storing max int limit + new line and null char*/
    char str[12];

    char *endptr;
    int idx = 0;
    int N = 0;
    int i = 0;
    int *a = NULL;
    if(!fp){
        printf("Error opening file %d\n", errno);
        return EXIT_FAILURE;
    }
    /*! first integer is the count of numbers to be read */
    if(fgets(str, sizeof(str), fp)){
        N = strtol(str, &endptr, 10);
        *size = N;
        if(errno == ERANGE || (endptr == str)){
            printf("Invalid count\n"); 
            return EXIT_FAILURE;
        }
        *p_arr = (int *)malloc(sizeof(int) * N);
        a = *p_arr;
        if(!*p_arr)
        {
            printf("Memory allocation failed\n");
            return EXIT_FAILURE;
        }
    }

    /*! first number from the stream */
    if(fgets(str, sizeof(str), fp)){
        a[0] = strtol(str, &endptr, 10);
        if(errno == ERANGE){
            printf("Invalid input\n"); 
            return EXIT_FAILURE;
        }
    }

    for(i = 1; i < N; i++)
    {
        /* assume every element is seperated by new line */
        if(!fgets(str, sizeof(str), fp)){
            /* no more char to read, exit ..*/
            printf("Input file doesnt have %d numbers\n", N);
            return EXIT_FAILURE;
        }

        /* push into array of N elements */
        a[i] = atoi(str);
    }
    qsort(a, N, sizeof(int), comparison);
    /*! read rest of the numbers */
    while(fgets(str, sizeof(str), fp))
    {
        int num = 0;
        /*! check if the incoming number is smaller than min_num in the array*/
        /*since the array is sorter, first element should be the smallest */
        num = strtol(str, &endptr, 10);
        if(errno == ERANGE){
            printf("Invalid value\n"); 
            break;
        }
        /* 1. need to see if the first element needs to be dropped
           2. if yes where should the new elemente be placed 
           3. may require shifting elements */

        if(num > a[0]){
            /*! bnary search to find where to place new item */
            idx = binsearch(num, a,N);
            /* left shift the element till idx and then place new elem at idx pos */
            /* this will ensure min element gets dropped */
            for(i =0 ; i < idx ; i++){
                a[i] = a[i+1];
            }
            a[i] = num;
        }
    }
    /* print the final array */
}

int main(void)
{
    char* file = "/home/manisha/input.txt";
    int *arr = NULL;
    int size = 0;
    int i = 0;
    int rc = 0;

    /* assume, numbers are seperated by newline */
    rc = readFile(file, &arr, &size);
    if( rc == EXIT_FAILURE )
        return EXIT_FAILURE;
    for(i = 0; i < size ; i++)
    {
        printf("%d,", arr[i]);
    }
    printf("\n");
    /*! sort the array in ascending order */
    return EXIT_SUCCESS;
}
