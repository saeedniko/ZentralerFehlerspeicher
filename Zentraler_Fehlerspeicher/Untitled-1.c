#include <stdio.h>
#include <time.h>


int main() {
    clock_t start = clock();

    // Perform some operations or delay for a while
    // ...

    for (size_t i = 0; i < 10000; i++)
    {
        i=i+i-i/1;
        printf(".");
    }
    
    double milliseconds = start ;
    clock_t end = clock();
    
    double milliseconds2 = end;

    printf("Elapsed time: %.0f milliseconds\n", milliseconds);
    printf("ok\n");
printf("Elapsed time: %.0f milliseconds\n", milliseconds2);
    return 0;
}
