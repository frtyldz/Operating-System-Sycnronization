#include <stdio.h>
#include <pthread.h>
#include "hash.h"
#include <stdlib.h>

pthread_mutex_t lock[100];
pthread_mutex_t deletion_lock = PTHREAD_MUTEX_INITIALIZER;
int consecutive_m;
int bucket_size;


HashTable *hash_init (int N, int M)
{
  bucket_size = N;
  consecutive_m = M;
  HashTable * h = (HashTable*)malloc( N * sizeof(HashTable) );
  for( int i = 0; i < N; i++)
    {
      h[i].list = NULL;
      h[i].noOfElement = 0;
    }
  int regionNumber = 0;
  if( N % M == 0)
    regionNumber = N/M;
  else
    regionNumber = (N/M) + 1;
  
  for( int i = 0; i < regionNumber; i++)
    pthread_mutex_init(&lock[i], NULL);
  return h;
}

int hash_insert (HashTable *hp, int k, void *v)
{
  if( k <= 0)
  {
  	printf("Invalid key value\n");
  	return -1;
  }
  int index = k % bucket_size;
  int lockNum = index / consecutive_m;
  pthread_mutex_lock(&lock[lockNum]);
  Node * head = hp[index].list;
  if( head == NULL)
    {
      hp[index].list =(Node*) malloc(sizeof(Node));
      hp[index].list->value = v;
      hp[index].list->key = k;
      hp[index].list->next = NULL;
      hp[index].noOfElement++;
      pthread_mutex_unlock(&lock[lockNum]);
      return 0;
    }
    else{
      Node * tmp = hp[index].list;
      while( tmp != NULL )
        {
          if( tmp->key == k)
          {
          	pthread_mutex_unlock(&lock[lockNum]);
          	return -1;
          }
          tmp = tmp->next;
        }
    }
  pthread_mutex_unlock( &lock[lockNum] );
  return -1;
}

int hash_delete (HashTable *hp, int k) {
  if( k <= 0)
    return -1;
  int index = k % bucket_size;
  int lockNum = index / consecutive_m;
  pthread_mutex_lock(&lock[lockNum]);
  Node * tmp = hp[index].list;
  Node * prev = NULL;

  while( tmp != NULL )
  {
      if( tmp -> key == k)
      {
	     // k is the head
	      if( tmp ->key == hp[index].list->key)
	      {
  	      hp[index].list = hp[index].list->next;
  	      //free(tmp->value);
  	      free(tmp);
  	      hp[index].noOfElement--;
  	      pthread_mutex_unlock(&lock[lockNum]);
  	      return 0;
	      }
	      else // tail or middle element
	      {
  	      prev->next = tmp->next;
  	      //free(tmp->value);
  	      free(tmp);
  	      hp[index].noOfElement--;
  	      pthread_mutex_unlock(&lock[lockNum]);
  	      return 0;
	      }
	    }
      prev = tmp;
      tmp = tmp -> next;
  }
  pthread_mutex_unlock(&lock[lockNum]);
  return -1;
}

int hash_update (HashTable *hp, int k, void *v)
{
  if( k <= 0)
    return -1;
  int index = k % bucket_size;
  int lockNum = index / consecutive_m;
  pthread_mutex_lock(&lock[lockNum]);
  Node * tmp = hp[index].list;
  while( tmp != NULL )
  {
   	if( tmp->key == k)
	  {
  		tmp->value = v;
  		pthread_mutex_unlock(&lock[lockNum]);
  		return 0;
	  }
    tmp = tmp->next;
  }
	pthread_mutex_unlock(&lock[lockNum]);
  return -1;
}

int hash_get (HashTable *hp, int k, void **vp)
{
  if( k <= 0)
    return -1;
  int index = k % bucket_size;
  int lockNum = index / consecutive_m;
  pthread_mutex_lock(&lock[lockNum]);
  Node * tmp = hp[index].list;
  while(tmp != NULL)
    {
    	if( tmp->key == k ){
  			*vp = tmp->value;
  			pthread_mutex_unlock(&lock[lockNum]);
  			return 0;
   	  }
      tmp = tmp->next;
    }
  vp = NULL;
  pthread_mutex_unlock(&lock[lockNum]);
  return -1;
}

int hash_destroy (HashTable *hp)
{
  pthread_mutex_lock(&deletion_lock);
  for( int i = 0; i < bucket_size; i++)
  {
    if(hp[i].list != NULL)
    {
      Node * head = hp[i].list;
      Node * cur = hp[i].list->next;
      Node * del = cur;
      while(cur != NULL)
      {
        del = cur;
        cur = cur -> next;
        head->next = cur;
        free( del);
      }
      free( hp[i].list );
     
     }

  }
  free(hp);
  pthread_mutex_unlock(&deletion_lock);
  return 0;
}

int * traverseKeys(HashTable *hp, int * size)
{
  int arrSize = 0;
  for(int i = 0; i < bucket_size; i++)
  {
    arrSize += hp[i].noOfElement;
  }
  *size = arrSize;
  //printf("size: %d\n", *size);
  int * arr = malloc( arrSize * sizeof(int) );
  int index = 0;
  for( int i = 0; i < bucket_size; i++)
  {
    Node * np = hp[i].list;
    while( np != NULL)
    {
      arr[index] = np->key;
      index++;
      np = np -> next;
    }
  }

  return arr;
}
