/* hash function for integer input */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bc_structs.c"

#define NULL_HASH 0000

int string_hash(void *string, int num_collisions)
{
	/* This is the djb2 string hash function */
	int result = 5381;
	unsigned char *p;
	p = (unsigned char *) string;
 
	while (*p != '\0') {
		result = (result << 5) + result + *p;
		++p;
	}

	if (result < 0) {  /* in case negative hash value is given */ 
		result = result * (-1);
	}

	result += num_collisions;  /* linear probing */ 
 
	return result;
}

/* compresses hash_code to fit in hash table array */ 
int compression(int hash_code) {

	hash_code = hash_code % MAX_HASHES; 
	return hash_code; 

}

int test_collision(int hash_code, Block *block, BlockChain *bc) {

	Block *existing_block = bc->hash_table[hash_code];

	if (existing_block) {
		if (block != existing_block) {  /* to allow for rehashing */ 
			return 1;
		}
	}
	return 0; 
}

/* concatenates strings */ 
void concat_strs(char *str, char item1[], char item2[], char *item3, char *item4) {
	strcpy(str, item1);   /* concatenate strings */ 
	strcat(str, item2);  
	strcat(str, item3);
	strcat(str, item4);
}

int get_hash(char *block_contents, int num_collisions) {
	int hash_code;

	hash_code = string_hash(block_contents, num_collisions);
	hash_code = compression(hash_code);
	return hash_code; 
}

/* hashes key - rudimentary, self-made alogrithm */ 
int hash(Block *block, BlockChain *bc) {
	
	char type_str[2];  
	char prev_hash_str[2]; 
	char *block_contents; 
	int hash_code; 
	int collision; 
	int num_collisions = 0; 

	if (!block) {
		hash_code = NULL_HASH; 
		return hash_code; 
	}

	snprintf(type_str, 2, "%d", block->type);  /* convert type to string */ 
	snprintf(prev_hash_str, 2, "%d", block->prev_hash);  /* convert type to string */ 

	block_contents = (void*)malloc(sizeof(type_str) + sizeof(block->sender) + sizeof(block->receiver) + sizeof(prev_hash_str));
	concat_strs(block_contents, type_str, prev_hash_str, block->sender, block->receiver);
	hash_code = get_hash(block_contents, num_collisions);

	collision = test_collision(hash_code, block, bc);
	
	/* linear probing to deal with collisions */ 
	while (collision) {
		num_collisions += 1;
		hash_code = get_hash(block_contents, num_collisions);
		collision = test_collision(hash_code, block, bc);
	}

	bc->hash_table[hash_code] = block; /* inserting hash and corresponding block to hash table */ 
	block->hash = hash_code; 

	// return hash_code; 
	return hash_code; 
}

