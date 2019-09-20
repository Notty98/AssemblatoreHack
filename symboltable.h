// ARCH1718.stefano.notari.0000832003
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "constants.h"

typedef struct symbolAddressTable{
	string symbol;
	int value;
	struct symbolAddressTable *next;
} symbolAddressTable_t;

typedef symbolAddressTable_t *ptr_table;

ptr_table addSymbol(ptr_table head,string symbol,int value);

void printSymbolTable(ptr_table head);

bool existSymbol(ptr_table head,string symbol);

ptr_table insertDefaultSymbol(ptr_table head);