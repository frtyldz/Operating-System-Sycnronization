#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"
#include "hash.h"
#include <time.h> 

HashTable *ht1; // space allocated inside library


void * insertRunner( void * param)
{
	int * p = (int*)param;
	for( int i = 1; i <= 1000; i++)
	{
		hash_insert (ht1, i, p[i]);
	}
	pthread_exit(0);
}
int main(int argc, char **argv)
{

    int i; 
    /*
	    N = 1000 and the N value is constant 
	    M value will be increased 10 by 10 until 100
		to change K value which is K=N/M
	*/
    int * arr = malloc( 1000 * sizeof(int) );
    for( i = 0; i < 1000; i++)
    	arr[i] = rand() % 1000 + 1;
	
	printf("EXPERIMENT WITH CHANGING LOCK VALUES(K) \n"); 
	printf("\n"); 
	clock_t t; 
    t = clock(); 
	pthread_t threads[50];
    // N = 1000, T = 5
	for( i = 10; i <= 100; i+=10 ){    
		ht1  = hash_init (1000, i);
		for (int j = 0; j < 5; ++j) {
			pthread_create(&threads[j], NULL,insertRunner, arr);
		}
		for( int j = 0; j < 5; j++ )
			pthread_join(threads[j], NULL);

		hash_destroy (ht1);
		t = clock() - t; 
		double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
  
		printf("The elapsed time for experiment M= %d, N=1000, W=1000, T=5 is %f \n", i, time_taken); 
	}
	
	printf("\n"); 
	printf("\n"); 
	printf("\n"); 
	printf("EXPERIMENT WITH CHANGING THREAD VALUES(T) \n"); 
	printf("\n"); 
	i = 0; 
    /*
	    N = 1000 and M = 100 the N and M value are constant 
	    changing T(number of threads) values
	*/
    int * arr1 = malloc( 1000 * sizeof(int) );
    for( i = 0; i < 1000; i++)
    	arr1[i] = rand() % 1000 + 1;
	
	pthread_t threads1[50];
    // N = 1000, T = 5
	for( i = 5; i <= 30; i+=5 ){    
		ht1  = hash_init (1000, 100);
		for (int j = 0; j < i; ++j) {
			pthread_create(&threads1[j], NULL,insertRunner, arr);
		}
		for( int j = 0; j < i; j++ )
			pthread_join(threads1[j], NULL);

		hash_destroy (ht1);
		t = clock() - t; 
		double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
  
		printf("The elapsed time for experiment M= 100, N=1000, W=1000, T=%d is %f \n", i, time_taken); 
	}
}
