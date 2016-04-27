#include <stdio.h>
#include "lista.h"

int compare(void * d1,void *d2)
{
   int aux = *(int*)d1 - *(int*)d2;
   printf("Estoy comparando %d con %d y el resultado es %d.\n",*(int*)d1,*(int*)d2,aux);
   if(aux==0)
     return 0;
  else
     if( aux > 0 )
	   return 1;
     else
           return -1;
}

int main()
{
      	printf("Hello, World!\n");
	punt r = NULL;
	int nro[10];
	lista * l = createList();
	int i = 10;
	
	 while(i--)
	 {
		 nro[i] = rand();
		 printf("En %d El numero es %d.\n",i,nro[i]);
		 push_back(l, &nro[i] );
	 }
	                                
	 sort(l,*compare);
	 printf("\n------------------------------------\n");
	 startNext(l);
	 r = next(l);
	 printf("\n------------------------------------\n");
	 while( r != NULL )
	 {
		 i++;
		 int aux = *(int*) r->data;
		 printf(" %d El numero es %d.\n",i,aux);
		 r = next(l);   
	}
	return EXIT_SUCCESS;
}
