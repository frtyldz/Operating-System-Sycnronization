#ifndef HASH_H
#define HASH_H

#include <pthread.h>

#define MIN_N 100
#define MAX_N 1000
#define MIN_M 10
#define MAX_M 1000

struct node
{
    int key;
    void * value;
    struct node * next;
};

typedef struct node Node;
struct hash_table
{
    Node * list;
    int noOfElement;
};

typedef struct hash_table HashTable;
HashTable *hash_init (int N, int M);
int hash_insert (HashTable *hp, int k, void* v);
int hash_delete (HashTable *hp, int k);
int hash_update (HashTable *hp, int, void *v); 
int hash_get (HashTable *hp, int k, void **vp);
int hash_destroy (HashTable *hp);
int * traverseKeys(HashTable *hp, int * size);


#endif /* HASH_H */

