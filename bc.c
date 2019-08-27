#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "hash_func.c"

/* initializes blockchain */ 
BlockChain *create_bc() {
	BlockChain *bc;

	bc = malloc(sizeof(BlockChain));
	bc->genesis = NULL;
	bc->tail = NULL; 
	bc->tail_hash = NULL_HASH; 

	return bc; 
}

BlockChain *copy_bc(BlockChain *bc) {

	BlockChain *bc_copy; 
	Block *block_copy; 
	Block *curr_block = bc->tail;  /* keeps track of original blockchain linked list */ 
	Block *curr_new_block;   /* keeps track of new bc linked list */

	bc_copy = malloc(sizeof(BlockChain));
	bc_copy = memcpy(bc_copy, bc, sizeof(BlockChain));  /* copying bc struct */ 

	block_copy = malloc(sizeof(Block));
	block_copy = memcpy(block_copy, curr_block, sizeof(Block));  /* copying block */ 
	bc_copy->hash_table[block_copy->hash] = block_copy; /* changing hash table entry */ 
	curr_new_block = block_copy; 

	bc_copy->tail = block_copy; /* setting new blockchain tail pointer */ 

	while (curr_block->prev_block) {
		curr_block = curr_block->prev_block; 
		block_copy = malloc(sizeof(Block));
		block_copy = memcpy(block_copy, curr_block, sizeof(Block));  
		bc_copy->hash_table[block_copy->hash] = block_copy; 
		curr_new_block->prev_block = block_copy;  /* adding block to linked list */ 
		curr_new_block = curr_new_block->prev_block;  /* incrementing new linked list */ 
	}

	bc_copy->genesis = block_copy;  /* setting genesis block */ 
	bc_copy->genesis->prev_block = NULL;
	
	return bc_copy; 
}

/* creates new block */ 
void create_block(Block *block, BlockChain *bc, Transfer_Type type, char *sender, char *receiver) {

	int hash_code; 

	block->type = type;
	block->sender = sender;
	block->receiver = receiver;

	hash_code = hash(block, bc);  /* creating and assigning hash */ 
}

/* adds block to block_chain */ 
Block *add_block(BlockChain *bc, Transfer_Type type, char *sender, char *receiver) {

	Block *block = block = malloc(sizeof(Block));

	/* adding block to blockchain linked list */ 
	if (!bc->genesis) {  /* if blockchain is empty */ 
		block->prev_hash = NULL_HASH; 
		block->prev_block = NULL; 
		bc->genesis = block; 
		bc->length = 1; 
	} else {
		block->prev_hash = bc->tail_hash;
		block->prev_block = bc->tail; 
		bc->length++; 
	}
	
	create_block(block, bc, type, sender, receiver);  /* setting block's information (hash, sender, receiver, type) */ 

	bc->tail = block; 
	bc->tail_hash = block->hash; 

	return block; 
}

/* checks to see if any blocks in bc have been tampered with */ 
BlockChain *validate_chain(BlockChain *bc) {
	Block *curr_block = bc->tail; 
	Block *block_to_free; 
	int rehash;  

	rehash = hash(curr_block, bc);  /* if tail block is invalid */ 
	
	if (rehash != bc->tail_hash) {
		block_to_free = curr_block; 
		bc->hash_table[bc->tail_hash] = NULL;
		bc->tail = curr_block->prev_block; 
		bc->tail_hash = curr_block->prev_hash; 
		free(block_to_free);	
		bc->length--; 
	}

	while (curr_block->prev_block) {  /* cycling through rest of blocks */ 
		rehash = hash(curr_block->prev_block, bc); 
		if (rehash != curr_block->prev_hash) {  /* if rehash is not equal to stored prev_hash value, tampered block deleted */ 
			block_to_free = curr_block->prev_block; 
			bc->hash_table[curr_block->prev_hash] = NULL; 
			curr_block->prev_hash = curr_block->prev_block->prev_hash;
			curr_block->prev_block = curr_block->prev_block->prev_block; 
			free(block_to_free);
			bc->length--; 
			if (curr_block->prev_hash == NULL_HASH) {  /* if block removed is genesis block */ 
				bc->genesis = curr_block; 
			}
			continue;
		}
		curr_block = curr_block->prev_block; /* something incorrect here */ 
	} 
	return bc; 
}

void print_bc(BlockChain *bc) {
	
	Block *curr_block = bc->tail; 

	while (curr_block != bc->genesis) {
		printf("Hash: %d\nPrevious Hash: %d\nSender: %s\nReceiver: %s\nType: %d\n", curr_block->hash, curr_block->prev_hash, curr_block->sender, curr_block->receiver, curr_block->type); 
		printf("------------------>\n\n");
		curr_block = curr_block->prev_block;
	}

	printf("Hash: %d\nPrevious Hash: %d\nSender: %s\nReceiver: %s\nType: %d\n", curr_block->hash, curr_block->prev_hash, curr_block->sender, curr_block->receiver, curr_block->type); 

}

