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

struct hash_table {
	// put your table definition here
	struct node *head;
	int count;
};
//test123
struct node {
	int key;
	struct node *next;
};

typedef struct hash_table HashTable;

int n;
HashTable *hash_init (int N, int M);
int hash_insert (HashTable *hp, int k, int v);
int hash_delete (HashTable *hp, int k);
int hash_get (HashTable *hp, int k, int *vptr);
int hash_destroy (HashTable *hp);


//utility functions

#endif /* HASH_H */
