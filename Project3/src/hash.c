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
	printf ("hash_init called\n");

	hash_table.table = (struct node**) malloc( sizeof(struct node*)*N);
	hash_table.count = 0;

	

	return (0);
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

int hash_destroy (HashTable *hp) {
	printf ("hash_destroy called\n");
	return (0);
}

int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	return 0;
}
