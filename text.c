#include <stdio.h>
#define swap(v,t,x,y) (t) = (x);\
					(x)=(y);\
					(y) =(t);\
					v = &(y)
					


int main(int argc, char *argv[]){
	int num1 =5;
	int num2=10;
	int temp=0;
	int *p;
	swap(p,temp,num1,num2);
	printf("num1 is %d\n", num1);
	printf("num2 is %d\n", num2);
}
