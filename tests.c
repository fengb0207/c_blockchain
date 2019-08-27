#include <stdio.h>
#include <stdlib.h>
#include "network.c"

/* creating blockchain and adding blocks */ 
void test1() {

	BlockChain *bc; 
	Block *block; 
	Block *tamp_block1, *tamp_block2, *tamp_tail; /*tampered blocks*/ 

	bc = create_bc(); 

	printf("*Adding 4 Blocks...*\n\n");
	block = add_block(bc, Money_Transfer, "Brandon", "Ryan");
	block = add_block(bc, Money_Transfer, "Jim", "Francis");
	tamp_block1 = add_block(bc, Crypto_Transfer, "Jimmy", "Bobby");
	tamp_block2 = add_block(bc, Crypto_Transfer, "Mildred", "Hannah");
	print_bc(bc);

	/* testing collisions */ 
	printf("\n\n*Testing collisions...*\n\n");
	block = add_block(bc, Crypto_Transfer, "Jimmy", "Bobby");
	print_bc(bc);

	/* testing validation function */ 
	printf("\n\n*Testing Validation Function!*\n\n");
	tamp_block1->sender = "Kendrick"; /* changed participants */ 
	tamp_block1->receiver = "Derrick";  /* changed hash information */ 
	tamp_block2->prev_hash = 282;
	validate_chain(bc);
	print_bc(bc);

	printf("\n\n*Testing Validation Function with tampered tail block*\n\n");
	tamp_tail = block; 
	tamp_tail->sender = "Jeff";
	tamp_tail->receiver = "Daniel";
	validate_chain(bc);
	print_bc(bc);

}

/* using network */ 
void test2() {
	
	Network *nw; 
	BlockChain *og_bc, *updated_bc; 
	Block *block; 
	Node *node1, *node2, *node3; 
	BlockChain  *n1_bc, *n2_bc, *n3_bc; 

	nw = create_network();
	og_bc = nw->bc; 
	block = add_block(og_bc, Money_Transfer, "Brandon", "Ryan");
	block = add_block(og_bc, Money_Transfer, "Jim", "Francis");

	printf("\n\n*Pushing Nodes...*\n\n");
	node1 = create_node("12.76.98.01", nw);  /* random ip addresses */
	node2 = create_node("45.23.78.09", nw);
	node3 = create_node("65.34.61.28", nw);
	print_nodes(nw);
	printf("\n");

	printf("\n\n*Printing original network blockchain\n\n*");
	print_bc(og_bc);

	printf("\n\n*Node 1's modidfied blockchain (added a block) ...*\n\n");

	block = add_block(node1->bc, Money_Transfer, "Robert", "Chelsea"); 
	print_bc(node1->bc);

	printf("\n\n*Node 2's modified blockchain (added block but modified 1 block)... *\n\n");
	block = add_block(node2->bc, Money_Transfer, "Robert", "Chelsea"); 
	node2->bc->genesis->sender = "Rupert";
	node2->bc->genesis->receiver = "Natalie";
	print_bc(node2->bc);

	printf("\n\nNode 3's blockchain remains unmodified\n\n");

	printf("\n\nValidating Network Blockchain...\n\n"); 

	updated_bc = update_nw_bc(nw);

	printf("\n\nNew Network Blockchain...\n\n");
	print_bc(updated_bc);
	printf("\n\nOld Network BlockChain...\n\n");
	print_bc(og_bc);
	printf("\n\nPrinting Node 2's new BlockChain copy\n\n");
	print_bc(node2->bc);

}


