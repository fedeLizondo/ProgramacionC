#ifndef LISTA_H
#define LISTA_H
/*--------------------------------------------------------
 *			LIST:H
 *--------------------------------------------------------
 *
 * Autor : Federico A Lizondo 
 *
 * version: 22/04/2016
 *
 * Description:  It's a generic simple list 
 * *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct nodo * punt;
typedef struct nodo{
	void * data;
	punt  sig;
	punt  ant;
}nodo;

typedef struct lista
{	
	punt inicio;
	punt fin;
	punt recorrido;
	int length;
}lista;

/*
 *informa si la lista esta vacia
 *@params l = lista a verificar
 *@return 0 = FALSE or 1 = TRUE;	 
 */
int isEmpty(const lista * l)
{
	return !( l != NULL && l->length ) ;
}

/*
 * Agrega un dato al final de una lista
 * @param dato = es el dato a agregar 
 * @param l = Lista a agregar  
*/
void push_back( lista * l , void * dato )
{		
	
	if( l != NULL)
	{
		//Si tengo memoria suficiente creo el nodo
		nodo * n = malloc(sizeof(lista));
	      	if ( n != NULL)
		{
			n->data = dato;
			n->ant = NULL;
			n->sig = NULL;	

			if( l->inicio != NULL)
			{			
				(l->fin)->sig = n;	
				n->ant = l->fin;
				l->fin = n;		
			}
			else
			{
				l->inicio = n;
				l->fin = n;
			}
			//Incremento en 1 la cantidad de elementos
			l->length++;
		}
		else
			printf("ERROR : Out of memory.\n");
	}
	else
		printf("ERROR:List is NULL.\n");
}

/*
 *Agrega un dato al principio de la lista 
 *@param l = lista a agregar 
 *@param dato = dato a agregar a la lista
 */
void push_front(lista * l , void * dato)
{	
	if( l != NULL)
	{	
		nodo * n = malloc(sizeof(nodo));
	
		if(n!=NULL)
		{
			n->data = dato;
			n->ant = NULL;
			n->sig = NULL;
		
			if( l->inicio != NULL)
			{
				(l->inicio)->ant = n; 
				n->sig = l->inicio;
			       	l->inicio = n;	
			}
			else
			{
				l->inicio = n ;
				l->fin = n;
			}	
		
			l->length++;
		}
		else
			printf("ERROR : Out of memory.\n");
	}
	else
		printf("ERROR : List is NULL.\n");
}

/*
 * Saca el dato del principio de la lista
 * @param l = lista a agregar
 * @param dato = dato a agregar a la lista
 */
void pop_front(lista * l , void ** dato)
{
	if( l != NULL && l->inicio != NULL )
	{
		punt list = l->inicio;
		*dato = (list)->data;
			
		l->inicio = (list)->sig;
		

		if(l->inicio == NULL)
			l->fin = NULL;
		else
			(l->inicio)->ant = NULL;
		//Disminuyo en uno el tamaño
		l->length--;
		//Libero memoria 
		free(list);
	}	
	else
	printf("ERROR : List is NULL.\n");
}

/*
 * Saca el dato del final de la lista
 * @param l = lista a agregar
 * @param dato = dato a agregar a la lista
 */
void pop_back(lista * l ,void ** dato)
{
	if( l != NULL && l->inicio != NULL)
	{
		punt list = l->fin;
		*dato = (list)->data;
		
		l->fin = list->ant;

		if( list == l->inicio )
			l->fin = l->inicio = NULL;
	        else
		   list->ant->sig = NULL; 		

		//Disminuyo en uno el tamaño
		l->length--;
		//Libero memoria
		free((nodo *)list);
	}
	else
	printf("ERROR : List is NULL.\n");
}

/*
 *Devuelve si hay un puntero sig
 *@param l = lista 
 */

punt next(const lista * l )
{
	if( l != NULL && l->recorrido != NULL )
	{
		const punt aux = l->recorrido;
		l->recorrido = (l->recorrido)->sig; 
		return aux;
	}
	else	
	if(l==NULL)
		printf("ERROR : List is NULL.\n");
	return NULL; 
}

/*
 *Inicializa variable de recorrido desde el inicio
 *@param l = lista
 */
void startNext(const lista * l)
{
	if( l != NULL )
	{
		l->recorrido = l->inicio; 	
	}
	else
	printf("ERROR : List is NULL.\n");
}

/*
 * Devuelve si hay un puntero anterior
 * @param l = lista
 */
punt prev(const lista * l)
{
	if( l != NULL && l->recorrido != NULL )
	{
		const punt aux = l->recorrido;
		l->recorrido = l->recorrido->ant;
		return aux;
	}
	else
	if(l != NULL)
		l->recorrido = l->fin;
	return NULL;
}	
/*
 * Inicializa la variable de recorrido desde el final
 * @param l = lista
 */
void startPrev(const lista *l)
{
	if(l != NULL && l->fin!=NULL )
	{
		l->recorrido = l->fin;
	}
	else
		printf("ERROR: List is NULL.\n");
}
/*
 * Inicializa la variable de recorrido desde una posicion cualquiera
 * @param l = lista
 * @param pos = desde donde empezar a recorrer
 */
void setRecorrido( const lista * l , punt pos)
{
	if(l!=NULL)
	{
		l->recorrido = pos;
	}
	else
		printf("ERROR : List is NULL.\n");
}
/*
 * Retorna la variable de recorrido
 * @param l = lista
 */
punt getRecorrido(const lista * l)
{
	if(l!=NULL)
	{
		const punt aux = l->recorrido;
		return aux;
	}
	else
		printf("ERROR : List is NULL.\n");
	return NULL;
}

/*
 * Retorna el puntero al primer Elemento
 * @param l = lista
 */
const punt begin(const lista * l)
{
	if( l != NULL)
	{
		const punt aux = l->inicio;
		return aux;
	}
	else
		printf("ERROR : List is NULL.\n");
	return NULL;
}

/*
 * Retorna el puntero al ultimo Elemento
 * @param l = lista
 */
const punt end(const lista * l)
{
	if( l != NULL)
	{
		const punt aux = l->fin;
		return aux;
	}
	else
		printf("ERROR : List is NULL.\n");
	return NULL;
} 
/*
 * Retorna la cantidad de elementos de la lista
 * @param l = lista
 */
int size(const lista * l )
{
	if( l != NULL )
	{
		return l->length;		
	}
	return 0; 
}

/*
 * Limpia la lista de todos los elementos
 * @param l = lista
 */
void clear(lista * l)
{
	if( l != NULL && l->inicio != NULL )
	{
		punt r = l->inicio;
		while( r != NULL)
		{
			l->inicio = r->sig;
			free((nodo *)r);
			r = l->inicio;
		}
		l->fin = NULL;
		l->recorrido = NULL;
		l->length = 0;		
	}
	else
		printf("Error : list is %s.\n",!l?"NULL":"Empty");	
}

/*
 *Elimina logicamente un elemento de una lista
 *@params l = lista
 *@param eliminar = elemento a eliminar logicamente
 */
void eliminarLogico( lista * l , punt  eliminar)
{
	if( l != NULL && l->inicio != NULL  && eliminar != NULL )
	{	
		punt r = eliminar;
		
		if( r->sig != NULL)
			(r->sig)->ant = r->ant;
		else
			l->fin = r->ant;
		
		if( r->ant != NULL)
			(r->ant)->sig = r->sig;
		else
			l->inicio = r->sig;
	}
	else
	printf("ERROR: list is NULL.\n");
}

/*
 * Eliminara un elemento de la lista
 * @param l = lista
 * @param pos = puntero a nodo a eliminar
 */
void removeElement( lista * l, punt * pos){
	if( l != NULL && pos != NULL )
	{
		punt r = (*pos);

		eliminarLogico(l,r);	
		l->length--;
		free(r);
		*pos = NULL;	
	}
	else
	if( l == NULL)
		printf("ERROR : list is NULL.\n");
	else
		printf("ERROR : position to erase is NULL.\n");
}

/*
 * Eliminara un elemento de la lista
 * @param l = lista
 * @param pos = numero de nodo a eliminar
 */
void erase(lista * l, int pos)
{
	if(l!=NULL && l->inicio != NULL )
	{
		punt r = NULL;
		if( pos >= 0 )
		{
			r = l->inicio;
			int aux = pos % l->length;
			while( aux-- ) 
				r = r->sig;
		}
		else
		{
			r = l->fin;
			int aux = pos % l->length;
			while(++aux)
				r = r->ant;
		}		
		eliminarLogico(l,r);		
		l->length--; 
		free(r);
	}
	else
		printf("ERROR : List is NULL.\n"); 	
}
/*
 * Se podra obtener un dato apartir de una posicion de memoria
 * @param pos = direccion del nodo
 */
void *getData(const punt * pos)
{
	if(pos!=NULL)
		return (*pos)->data;			
	else
		printf("ERROR : position to get data is NULL.\n");
	return NULL;
}

/*
 * Se podra ingresar un dato a una direccion de memoria 
 * @param pos
 * @param dato
 */ 
void setData(const punt * pos , void * dato )
{
	if(pos != NULL)
		(*pos)->data = dato;
	else
		printf("ERROR : position to get data is NULL.\n");
}

/*
 * Permitira intercambiar los datos entre 2 punteros
 * @param origen = primer datos para intercambiar
 * @param destino = segundo dato para intercambiar
 */
void swap( punt * origen , punt * destino )
{
	if(origen != NULL && destino != NULL)
	{
		void * aux = (*origen)->data;
		(*origen)->data = (*destino)->data;
		(*destino)->data = aux;
	}
	else
	printf("ERROR : %s is NULL.\n", (origen == NULL)?"Origin":"Destination");
}

/*
 * Devuelve el puntero en una posicion dada
 * @param l = lista
 * @param pos = numero de posicion que se quiere tener el puntero
 */
punt getPuntPos(const lista * l ,const int pos)
{
	if( l != NULL && l->inicio != NULL )
	{
		punt r = NULL;
		int aux = pos % l->length;

		if(pos>=0)
		{
			r = l->inicio;				 
			while(aux--)
				r=r->sig;
		}
		else
		{
			r = l->fin;
			while(++aux)
				r = r->ant;	
		}
		return r;				
	}
	else
		printf("ERROR : list is NULL.\n");
	return NULL;
}

/*
 * Permite separar una lista en otra , con perdida de informacion
 * @param l = lista
 * @para poscion = posicion a cual partir la lista en 2;
 */
lista *split(const lista * l,const int posicion)
{
	lista * list = malloc(sizeof(lista));
	if( list != NULL)
	{
		list->length = 0;
		list->inicio = NULL;
		list->fin = NULL ;
		list->recorrido = NULL;
		
		if(l != NULL && l->inicio != NULL)
		{
			list->fin = l->fin;

			punt r = NULL;
			int aux = posicion%l->length;
			int contador = 0 ;
		
			if(posicion >=0 )
			{
				r = l->inicio;
				while(aux++)
					r = r->sig;	
				
			}
			else
			{
				r = l->fin;
				while(--aux)
					r = r->ant;
			}
			
			list->inicio = r;
			l->fin = r->ant;
	
			if(r->ant != NULL)
				(r->ant)->sig = NULL;
			else
				l->inicio = NULL;
			
			contador++;
	
			r = list->inicio;
			
			while(r->sig != NULL)
			{
				contador++;
				r = r->sig;
			}
			l->length -= contador;
			list->length += contador;
		}
		else
			printf("ERROR : List is %s.\n",!l?"NULL":"Empty");	
	}
	else
		printf("ERROR : Out of memory.\n");
	
	return list;
}

/*
 * Permitira a partir de 2 listas formar una sola
 * @param destino = lista a donde se agregaran todos los nodos;
 * @param origen = lista de la cual se sacaran los nodos;
 */
void merge(lista * destino, lista * origen)
{
	if(	
		destino != NULL && 
		origen != NULL && 
		destino->inicio != NULL &&
	       	origen->inicio != NULL
	   )
	{
		(destino->fin)->sig = origen->inicio;
	       	(origen->inicio)->ant = destino->fin;
		destino->fin = origen->fin;

		origen->inicio = NULL;
		origen->fin = NULL;

		destino->length += origen->length;
		origen->length = 0;
	}
	else
	{
		if(!origen || !destino)
			printf("ERROR : %s %s NULL .\n",!origen?"Origin":"Destination",origen == destino?"and Destination are":"is");
       		else
			printf("ERROR : %s %s Empty.\n",!origen->inicio?"Origin":"Destination",!destino->inicio?" and Destination are":"is");	

	}	
}

/*
 * Permitira duplicar fisicamente una lista en memoria
 * @param list = lista origen la cula duplicar)
 */
lista * copy(const lista * origen)
{
	lista * l = malloc(sizeof(lista));
	if( l != NULL )
	{
		l->length = 0;
		l->inicio = NULL;
		l->fin = NULL;
		l->recorrido = NULL;

		if( origen != NULL )
		{	
			punt r = origen->inicio;
			
			while(r != NULL)
			{
				push_back(l,r->data);
				r = r->sig;
			};			
		}
		else
			printf("ERROR : list is NULL.\n");	
	}
	else
		printf(" ERROR : Out of memory.\n");

	return l;
}

/*
 * Podra ingresar un elemento dado en una posicion determinada
 * @param l = lista a donde insertar elemento
 * @param dato = elemento a agregar
 * @param pos = lugar donde agregar
 */ 
void insert(lista * l , void * dato , int pos )
{
	if( l != NULL)
	{
		nodo * n = malloc(sizeof(nodo));
		
		if( n != NULL )
		{
			n->sig = NULL;
			n->ant = NULL;
			n->data = dato;
		
			punt r = NULL;
			int aux = pos % l->length;
			if( pos >= 0 )
			{
				r = l->inicio;
				while(aux--)
					r = r->sig;	
			}
			else
			{
				r = l->fin;
				while(++aux)
					r= r->ant;		
			}
		
			n->ant = r->ant;
			n->sig = r->sig;
			
			if( r->ant != NULL)
				(r->ant)->sig = n ;
		       	else		
				l->inicio = n;
	
			if( r->sig != NULL )
				(r->sig)->ant = n;
			else
				l->fin = n;
		}
		else
			printf("ERROR: Out of memory.\n");
	}		
	else
		printf("ERROR: List is NULL.\n");
}

/*
 * Devolvera una lista vacia
 * @param l = void
 */
lista *createList()
{	
	lista * l = malloc(sizeof(lista));
		
	if( l != NULL)
	{
		l->inicio = NULL;
		l->fin = NULL;
		l->recorrido = NULL;
		l->length = 0;
	}	
	else
	       printf("ERROR : Out of Memory.\n");	
	return l;
}

/*
 * Cambiara el sentido de la lista Ultimo a primero
 * @param lista
 */
void reverse( lista * l )
{
	if(l != NULL && l->inicio != NULL )
	{
		punt r = l->inicio;
		punt aux = NULL;
		
		l->inicio = l->fin ;
		l->fin = r;
		
		while(r!=NULL)
		{
			aux = r->sig;
			r->sig = r->ant;
			r->ant = aux;
			r = aux;
		}	
	}
	else
		printf("ERROR : List is %s .\n",(l==NULL)?"NULL":"Empty");
}	

#endif /* LISTA_H */
