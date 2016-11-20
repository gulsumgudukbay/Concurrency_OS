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
		ht->table[i] = (struct node*) malloc( sizeof( struct node));
		ht->table[i]->next = 0;
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

int hash_insert (HashTable *hp, int k, int v) {
	printf ("hash_insert called\n");
	return (0);
}

int hash_delete (HashTable *hp, int k) {

	printf ("hash_delete called\n");
	return (0);
}

int hash_get (HashTable *hp, int k, int *vptr) {
	printf ("hash_get called\n");
	return (0);
}

int hash_destroy (HashTable *hp)
{
	printf ("hash_destroy called\n");
	return (0);
}

int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	return 0;
}
