#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bc.c"

Network *create_network() {
	BlockChain *bc; 
	Network *nw = malloc(sizeof(Network)); 

	bc = create_bc();  /* creates blockchain and adds to network */
	nw->bc = bc; 
	nw->len_node_list = 0; 

	return nw; 

}

void push_node(Node *node, Network *nw) {
	assert(nw->len_node_list < MAX_NETWORK_SIZE);  /* overflow */ 

	node->index = nw->len_node_list; 
	nw->node_list[nw->len_node_list++] = node; 
}

Node *create_node(char *ip_address, Network *nw) {
	Node *node = malloc(sizeof(Node));

	node->ip_address = ip_address; 
	node->bc= copy_bc(nw->bc);

	push_node(node, nw); 

	return node;
}

void print_nodes(Network *nc) {
	int i = 0;

	while (nc->node_list[i]) {
		printf("Node %d (%s, index %d)\n---------------->\n", i, nc->node_list[i]->ip_address, nc->node_list[i]->index);
		++i; 
	}
}


void broadcast_node_bcs(Network *nw) {  /* appends each node's blockchain to potential blockchain list */ 
	int i = 0;
	Node *node; 

	while (i < nw->len_node_list) {
		node = nw->node_list[i]; 
		nw->potential_bcs[i] = node->bc; 
		i++; 
	}
}

BlockChain *update_nw_bc(Network *nw) {
	BlockChain *validated_bc; 
	BlockChain *updated_bc = nw->bc;  
	int max_bc_len = nw->bc->length;  /* length of longest blockchain */ 
	int i = 0;

	broadcast_node_bcs(nw);

	while (nw->potential_bcs[i]) {
		validated_bc = validate_chain(nw->potential_bcs[i]);  /* validates chain */ 
		if (validated_bc->length > max_bc_len) {
			max_bc_len = validated_bc->length;  /* makes longest validated chain the network's new blockchain */ 
			updated_bc = validated_bc; 
		}
		i++; 
	}

	i = 0; 
	while (i < nw->len_node_list) {  /* gives copy of final blockchain to all nodes in network */ 
		nw->node_list[i]->bc = copy_bc(updated_bc); 
		i++; 
	}

	return updated_bc;
}