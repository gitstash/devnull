#include <stdio.h>
#include <math.h>
int main(void)
{
    //double num = 10.334;
    //double epsilon = .001;
    printf("square_root(%lf) = %lf\n", 10.334, sqrt(10.334));

    //num = .3334; epsilon = 1e-6;
    printf("square_root(%lf) = %lf\n", .3334, sqrt(.3334));
    printf("square_root(%lf) = %lf\n", 2.0, sqrt(2.0));
    printf("square_root(%lf) = %lf\n", 0.0, sqrt(0.0));
}
