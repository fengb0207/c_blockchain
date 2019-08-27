#include <stdio.h>
#include <stdlib.h>
#define MAX_HASHES 1024
#define MAX_NETWORK_SIZE 100

typedef enum {
	No_Type, 
	Money_Transfer,
	Crypto_Transfer, 
} Transfer_Type; 

/* blockchain block */ 
typedef struct block {
	Transfer_Type type; 
	char *sender;
	char *receiver; 
	int hash;  
	int prev_hash; 
	struct block *prev_block; 

} Block; 

typedef struct blockchain {
	Block *genesis; 
	Block *tail; 
	int tail_hash;  /* to be able to validate last block in chain */ 
	int length; 
	Block *hash_table[MAX_HASHES];
} BlockChain; 

typedef struct node {  /* node in network */ 
	char *ip_address; 
	BlockChain *bc;  /* individual blockchain copy */ 
	int index; /* position in list */ 
} Node; 

typedef struct network {
	Node *node_list[MAX_NETWORK_SIZE];
	int len_node_list; 
	BlockChain *potential_bcs[MAX_NETWORK_SIZE];
	BlockChain *bc;
} Network; 


