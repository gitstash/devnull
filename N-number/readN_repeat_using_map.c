#include <fstream>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <map>

using namespace std;
int main(void)
{
    std::ifstream infile("/home/manisha/in-file.txt");
    if(!infile){
        cout << "Error opening file " << endl;
        return EXIT_FAILURE;
    }
    std::map< int, int > number;
    std::map <int, int>::iterator iter = number.begin();
    std::string line;
    int num =0;
    char *endptr;
    int N = 0;
    /*! read first element */
    infile >> N;
    int count = N;
    while(count-- && !infile.eof()){
        infile >> num; 
        /*! insert into the map with number as index and 
          no of occurances as value */

        /*TODO check if the num is valid integer and its not out of integer range */
        iter = number.find(num);
        if(iter == number.end()){
            cout <<"Number " << num << " not found, inserting as new data" << endl;
            number[num] = 1;
        }else {
            iter->second++;
            cout << "Found existing number " << num << endl;
            /* need n distinct numbers */
            count++;
        }
    }
    /*! printing N distinct numbers from the file */
    for(iter = number.begin(); iter != number.end(); iter++) {
        cout << iter->first << ":" << iter->second << endl;
    }

    /*! any point of time maintain N data */
    while(!infile.eof()){
        /*! map maintains data in increasing order */
        infile >> num;
        iter = number.find(num);
        if(iter == number.end()){
            cout << "Number not found, inserting as new data" << endl;
            /*! check if this number is smaller than the least,
                if so , ignore
                if not, delete the first element in the map and insert the new one */
            if(num > iter->first){
                number.erase ((number.begin())->first);
                number[num] = 1;
            }
        } else {
            iter->second++;
            /* need N distinct numbers */
        }
    }
    /*! print the map */
    for(iter = number.begin(); iter != number.end(); iter++) {
        cout << iter->first << ":" << iter->second << endl;
    }

    /*! printing N largest numbers, not necessarily distinct */
    cout << N << " Largest number, not necessarily distinct " << endl;
    for(iter = number.end(); iter != number.begin() && N-- ; iter--) {
        while(iter->second-- && N){
            cout << iter->first << endl;
            N--;
        }
    }
}
