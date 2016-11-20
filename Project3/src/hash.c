/*
 ============================================================================
 Name        : Project3.c
 Author      : Gulsum Gudukbay & Dogukan Yigit Polat
 Version     :
 Copyright   :
 ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>
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

	ht = (HashTable*) malloc(sizeof(HashTable));
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
	struct node* cur, *prev, *ins;

	pthread_mutex_lock( &locks[hash/(hp->M)]);

	ins = (struct node*) malloc( sizeof(struct node));
	ins->key = k;
	ins->value = v;
	ins->next = 0;

	if( !hp->table[hash])
	{
		hp->table[hash] = ins;
	}
	else
	{
		cur = hp->table[hash];
		prev = cur;
		while( cur)
		{
			if( v == cur->value)
			{
				free(ins);
				pthread_mutex_unlock( &locks[hash/(hp->M)]);
				return -1;
			}

			prev = cur;
			cur = cur->next;
		}
		prev->next = ins;

	}

	pthread_mutex_unlock( &locks[hash/(hp->M)]);

	return 0;
}

int hash_delete (HashTable *hp, int k) {

	int success = 0;
	int hash = k % hp->N;
	pthread_mutex_lock(&locks[hash/hp->M]);

	struct node *cur, *nextOfDeleted;
	cur = hp->table[hash];
	printLL(cur);
	if(!cur){
		printf("k is not present!");
		pthread_mutex_unlock(&locks[hash/hp->M]);
		return -1;
	}
	nextOfDeleted = cur;
	while(cur){
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

int hash_get (HashTable *hp, int k, int *vptr)
{
	int hash = k % hp->N;
	struct node* cur, *prev, *ins;

	pthread_mutex_lock( &locks[hash/(hp->M)]);

	cur = hp->table[hash];
	while( cur)
	{
		if(cur->key == k)
		{
			*vptr = cur->value;
			pthread_mutex_unlock( &locks[hash/(hp->M)]);
			return 0;
		}

		cur = cur-> next;
	}

	pthread_mutex_unlock( &locks[hash/(hp->M)]);
	return -1;
}

int hash_destroy (HashTable *hp)
{
	printf ("hash_destroy called\n");
	struct node* currnd;
	struct node* prevnd;

	for(int i = 0; i < K; i++)
		pthread_mutex_destroy(&locks[i]);
	free(locks);
	for(int i = 0; i < hp->N; i++)
	{
		currnd = hp->table[i];
		prevnd = hp->table[i];
		while(currnd != NULL)
		{
			currnd = prevnd->next;
			free(prevnd);
			prevnd = currnd;
		}
	}

	return (0);
}

void printLL(struct node* head){
	if(head!=NULL){
		printf("\n%s", "Contents of linkedlist:\n");
		struct node *cur = head;
		while (cur != NULL){
			printf("Key: %d\t", cur->key);
			printf("Value: %d\t", cur->value);
			cur = cur->next;
		}
	}
	else
		printf("\n%s", "NO LL\n");
}


int main(void)
{

	HashTable* hpt = hash_init( 100, 10);

	int val, i, j;
	hash_insert( hpt, 13, 31);
	hash_insert( hpt, 113, 69);
	hash_get( hpt, 113, &val);
		printf("%d\n", val);

	hash_get( hpt, 13, &val);

	printf("%d\n", val);

	i = hash_delete(hpt, 13);
	printf("%d \n", i);
	i = hash_delete(hpt, 13);
	printf("%d \n", i);
	i = hash_delete(hpt, 113);
	printf("%d \n", i);
	i = hash_delete(hpt, 113);
	printf("%d \n", i);

	hash_destroy( hpt);


	return 0;
}
