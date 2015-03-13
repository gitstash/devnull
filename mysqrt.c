#include <stdio.h>

//Given signature of the custom sqrt func takes 
//double. I am writing a method which is not fast
//enough, but works well for a double ( fraction 
// >1  both. Its based on binary search where in
// every result output is getting narrowed down
// to an interval close enough to the result
// Works for integers too

double square_root(double num, double epsilon)
{
    double upper , lower, guess;
    if (num < 0)
    {
        printf("Negative bumber, exiting ..");
        return ;
    }

    if(num < 1.0 )
    {
        lower = num;
        upper = 1;
    }
    else
    {
        lower = 1;
        upper = num;
    }

    while ( (upper-lower) > epsilon)
    {
        guess = (lower+upper)/2; 
        if(guess * guess - num > 0)
        {
            upper = guess;
        }
        else
        {
            lower = guess;
        }
    }

    //given this method of 
    //approximation, it will take multiple iteration 
    //to get the closest result
    //The alternate method is to use Quake 3 game methos
    //which uses Newton's equation for approximation,
    //where there is a magic number
    //to start with and use the proven formula to 
    //get new guess value every time till we get the
    //closest value. The number of iteration is very
    //less compare to the method provided here, 
    //thats out of scope in the assigment, as 
    //i can infer from the prototype that this is the
    //method desired
    return (lower + upper)/2;
}

int main(void)
{
    double num = 10.334;
    double epsilon = .001;
    printf("square_root(%lf) = %lf\n", num, square_root(num, epsilon));

    num = .3334; epsilon = 1e-6;
    printf("square_root(%lf) = %lf\n", num, square_root(num, epsilon));
    num =2;
    printf("square_root(%lf) = %lf\n", num, square_root(num, epsilon));
    num = 0;
    printf("square_root(%lf) = %lf\n", num, square_root(num, epsilon));
}
