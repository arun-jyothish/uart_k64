typedef struct {
	int num;
	int num2;
	float doub;
	char name[10];
	float fl;
}st;

#include <stdio.h>

int main(int argc, char *argv[])
{
	st d1= {
		.num = 20,
		.fl = 23.0,
	};

	printf("\r\n%d", d1.num);
	printf("\r\n%d", d1.num2);
	printf("\r\n%s", d1.name);
	printf("\r\n%f", d1.fl);

}
