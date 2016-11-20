/*
 * hash.h
 *
 * Created on: Nov 20, 2016
 * Author: Gulsum Gudukbay
 * Student ID: 21401148
 */
#ifndef HASH_H
#define HASH_H

#include <pthread.h>

#define MIN_N 100
#define MAX_N 1000
#define MIN_M 10
#define MAX_M 1000

struct node{
	int key;
	int value;
	struct node *next;
};

struct hash_table
{
	int N;
	int M;
	int count;
	struct node** table;
};
typedef struct hash_table HashTable;

//data
pthread_mutex_t* locks;
int K;

//func
HashTable *hash_init (int N, int M);
int hash_insert (HashTable *hp, int k, int v);
int hash_delete (HashTable *hp, int k);
int hash_get (HashTable *hp, int k, int *vptr);
int hash_destroy (HashTable *hp);
void printLL(struct node* head);


//utility functions

#endif /* HASH_H */
