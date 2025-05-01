#include <stdio.h>
#include "foo.h"

int main() {
	int a = 5;
	foo(&a);
	printf("%d hej", a);
	while (1) {};
}