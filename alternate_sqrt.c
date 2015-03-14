#include <stdio.h>
double square_root( int n, double epsilon_UNUSED )
{
    double a = (double) n;
    double x = 1;

    int i = 0;
    for( i = 0; i < n; i++)
    {
        x = 0.5 * ( x+a / x );
    }

    return x;
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
