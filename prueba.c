#include <stdio.h>

int main(){
	int c;
	printf("Ingresa un \"y\" o \"n\"");
	c =getchar();
	int suma=0;

	printf("\nIngresaste: %c\n", c);
	if(c=='y'){
		suma=1;
	}
	else if(c=='n'){
		suma=-1;
	}
	else{
		printf("Ingresa una respuesta válida\n");
	}
	printf("suma=%d\n", suma);
	return 0;
}
