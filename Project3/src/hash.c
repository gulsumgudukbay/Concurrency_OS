/*
 ============================================================================
 Name        : Project3.c
 Author      : Gulsum Gudukbay & Dogukan Yigit Polat
 Version     :
 Copyright   :
 ============================================================================
 */


#include <stdio.h>
#include <pthread.h>
#include "hash.h"

// remove printf debug statements at the end


HashTable *hash_init (int N, int M)
{
	HashTable* ht;
	K = N / M;
	if( N > MAX_N || N < MIN_N || M > MAX_M || M < MIN_M || N % M != 0 || K > 100 || K < 1)
	{
		printf( "camda yaraqshow\n");
		return 0;
	}

	ht->table = (struct node**) malloc( sizeof(struct node*) * N);
	for( int i = 0; i < N; i++)
	{
		ht->table[i] = 0;
	}

	ht->count = 0;
	ht->N = N;
	ht->M = M;

	locks = (pthread_mutex_t*) malloc( sizeof( pthread_mutex_t) * K);
	for( int i = 0; i < K; i++)
	{
		if( pthread_mutex_init( &locks[i], 0))
		{
			printf( "yaraq\n");
			return 0;
		}
	}

	printf( "hash_init done.\n");
	return (ht);
}

int hash_insert( HashTable *hp, int k, int v)
{
	int hash = k % hp->N;
	struct node* cur, prev;

	cur = hp->table[k];
	prev = cur;
	while( cur)
	{
		if( v == cur->value)
			return -1;

		cur = cur->next;
		prev = cur;
	}

	return (0);
}

int hash_delete (HashTable *hp, int k) {

	int success = 0;
	int hash = k % hp->N;
	pthread_mutex_lock(&locks[hash/hp->M]);

	printf ("hash_delete called\n");
	struct node *cur, *nextOfDeleted;
	cur = hp->table[hash];
	if(!cur){
		printf("k is not present!");
		return -1;
	}
	nextOfDeleted = cur;
	while(cur != NULL){
		if(cur->key == k){
			success = 1; // data is found
			if(cur == hp->table[hash])
				hp->table[hash] = cur->next; // change the head
			else
				nextOfDeleted->next = cur->next;
			free(cur);
			break;
		}
		nextOfDeleted = cur;
		cur = cur->next;
	}

	pthread_mutex_unlock(&locks[hash/hp->M]);

	if(success == 1)
		return 0;
	else
		return -1;

}

int hash_get (HashTable *hp, int k, int *vptr) {
	printf ("hash_get called\n");
	return (0);
}

int hash_destroy (HashTable *hp)
{
	printf ("hash_destroy called\n");
	struct node* currnd;
	struct node* prevnd;
	for(int i = 0; i < K; i++)
		pthread_mutex_destroy(&locks[i]);
	for(int i = 0; i < hp->N; i++){
		currnd = hp->table[i];
		prevnd = hp->table[i];
		while(currnd != NULL){
			currnd = prevnd->next;
			free(prevnd);
			prevnd = currnd;
		}
	}
	return (0);
}

int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	return 0;
}
