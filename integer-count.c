#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"
#include "hash.h"

HashTable *ht1; // space allocated inside library
int paramSize;

typedef struct COUNT
{
  int key;
  int value;
}Count;


Count* shellSort(Count arr[], int n)
{
  for (int gap = n/2; gap > 0; gap /= 2)
  {
    for (int i = gap; i < n; i += 1)
    {
      Count temp = arr[i];
      int j;
      for (j = i; j >= gap && arr[j - gap].key > temp.key; j -= gap){
        arr[j].key = arr[j - gap].key;
        arr[j].value = arr[j - gap].value;
      }
      arr[j] = temp;
    }
  }
  return arr;   
}


void * insertOrUpdate( void * param )
{
	char ** p = (char*)(param);
	for( int i = 0; i < paramSize; i++)
	{
		int key = atoi(p[i]);
		//printf("key: %d\n", key);
		if( hash_insert(ht1, key, 1) != 0 )
		{
			void * val;
			if( hash_get(ht1, key, &val) == 0 )
			{	
        int newVal = (int)val;
        newVal++;
				hash_update(ht1, key, newVal);
			}
      else
        printf("The key cannot be got\n");
		}
	}
	pthread_exit(0);
}

int main(int argc, char **argv)
{

  //printf("main\n");
  int numberOfFile = atoi(argv[1]);

  if( argc < numberOfFile + 2 )
  {
  	printf( "The syntax is the following:\n" );
  	printf("integer-count 7 1.txt 2.txt 3.txt 4.txt 5.txt 6.txt 7.txt out.txt\n");
  	return -1;
  }
 // printf("numberOfFile: %d\n", numberOfFile );
  ht1 = hash_init(1000,100);

  pthread_t threads[numberOfFile];

  for( int i = 2; i < numberOfFile + 2; i++ )
  {
  	FILE *fd = fopen( argv[i], "r" );
  	char line[20];

  	if( fd == NULL )
  	{
  		printf("The file couldn't find or in a wrong format(should be .txt)\n");
  		return -1;
  	}
  	int numberOfLine = 0;
  	while( fgets( line, 20, fd) != NULL)
  	{
  		numberOfLine++;
  	}
  	paramSize = numberOfLine;
  	fd = fopen( argv[i], "r" );
  	char ** paramArr = malloc( numberOfLine * sizeof(char*) );
  	for( int i = 0; i < numberOfLine; i++)
  	{
  		if(fgets( line, 20, fd) != NULL ){
  			paramArr[i] = malloc( 20 * sizeof(char) );
  			for( int j = 0; j < 20; j++ )
  				paramArr[i][j] = line[j];
  		}
  		else{
  			printf("Line cannot be got by file descriptor\n");
  			return -1;
  		}
  	}
  	//for( int i = 0; i < numberOfLine; i++)
  		//printf("paramArr[i]: %s\n", paramArr[i]);

  	pthread_create(&threads[i-2], NULL, insertOrUpdate, (void*)paramArr);
  }

  for( int i = 0; i < numberOfFile; i++)
    pthread_join(threads[i],NULL);

  int size;
  int * keys = traverseKeys(ht1,&size);
  Count counter[size];
  for( int i = 0; i < size; i++)
  {
    counter[i].key = keys[i];
    void * val;
    if( hash_get(ht1, keys[i], &val) == 0 ){
      counter[i].value = (int)val;
    }
  }

  Count * sorted = shellSort(counter,size);
  FILE * fp;
  fp = fopen( argv[argc-1], "w" );

  if( fp == NULL )
    printf("File creation is failed!\n");
  else
  {
    for( int i = 0; i < size; i++){
      char key[41];
      char value[20];
      sprintf(key,"%d",sorted[i].key);
      strcat(key, ":");
      sprintf(value,"%d",sorted[i].value);
      strcat(key,value);
      strcat(key,"\n");
      fprintf(fp, key, i + 1);
    }
  }
  return 0;
}
