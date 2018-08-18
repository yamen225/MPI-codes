#include <stdio.h>
#include <stdlib.h>

int IsPrime (unsigned int number);

int main(int argc, char const *argv[])
{
	int a, b, i, ip, counter=0;
	scanf("%d", &a);
	scanf("%d", &b);
	for (i = a; i < b; i++)
	{
		ip = IsPrime(i);
		if (ip) counter++;
	}
	printf("count = %d\n", counter);
	return 0;
}
int IsPrime(unsigned int number) {
    if (number <= 1) return 0; // zero and one are not prime
    unsigned int i;
    for (i=2; i*i<=number; i++) {
        if (number % i == 0) return 0;
    }
    return 1;
}