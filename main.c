// ARCH1718.stefano.notari.0000832003
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hackinstruction.h"
#include "symboltable.h"
#include "constants.h"

string fileInputName;

const string ext = "hack";

char specialSymbol [13] = {'=',';','-','+','|','&','!','_','.','$','@','(',')'};

bool existSpecialSymbol(char s){
	int i = 0;
	bool exist = false;
	while(i < 13 && !exist){
		if(specialSymbol[i] == s)
			exist = true;
		i++;
	}
	return exist;
}

int getSymbolValue(ptr_table head,string symbol){
	ptr_table itr = malloc(sizeof(ptr_table));
	itr = head;
	bool found = false;
	int v = -1;
	if(itr != NULL)
		do{
			if(strcmp(itr->symbol, symbol) == 0){
				found = true;
				v = itr->value;
			}
			itr = itr->next;
		}while(itr != NULL && !found);
	return v;
}

bool isAinstruction(string line){
	int i = 0;
	bool blank = true;
	while(blank && line[i] != '\0'){
		if(line[i] != ' ')
			blank = false;
		else
			i++;
	}
	if(line[i] == '@')
		return true;
	return false;
}

string getInstructionValue(string line,int length,bool aInstruction,string to){
	int indexStart = 0;
	int indexEnd = 0;
	int itr = 0;

	bool endString = false;

	if(aInstruction)
		itr++;
	indexStart = itr;

	while(!endString)
		if(isdigit(line[itr]) || isalpha(line[itr]) || existSpecialSymbol(line[itr]))
			itr++;
		else 
			endString = true;

	indexEnd = itr;


	char *start = &line[indexStart];
	char *end = &line[indexEnd];
	string value = (char *)calloc(1, end - start + 1);
	memcpy(value, start, end - start);
	strcpy(to,value);
}

bool isEtichetta(string line){
	if(line[0] == '(')
		return true;
	return false;
}

void getEtichettaValue(string line,int read,string to){
	int i = 1;
	while(line[i] != '\0' && line[i] != ')' && i < read)
		i++;
	char *start = &line[1];
	char *end = &line[i];
	
	string value = (char *)calloc(1, end - start + 1);
	memcpy(value, start, end - start);
	strcpy(to,value);
}

void  decToBinary(long dec,bool aInstruction,string to){
	int remainder; 
	int idx = 15;
	char bin [max_length];
	int j;
	for(j = 0; j < 16; j++)
		bin[j] = '0';
    while(dec != 0) {
        remainder = dec%2;
        dec = dec/2;
        if(remainder == 0)
        	bin[idx] = '0';
        else
        	bin[idx] = '1';
        idx--;
    }
    bin[16] = '\0';
    strcpy(to,bin);
}

bool isJump(string cInstruction,int read){
	bool jump = false;
	int i = 0;
	while(i < read && !jump){
		if(cInstruction[i] == ';')
			jump = true;
		i++;	
	}
	return jump;
}

void cInstructionBinary(string cInstruction,int read,string ret){
	char sx[max_length];
	char dx[max_length];
	char c;
	if(isJump(cInstruction,read))
		c = ';';
	else
		c = '=';

	bool found = false;
	int i = 0;
	int indexSx =0;
	int indexDx = 0;
	while(isdigit(cInstruction[i]) || isalpha(cInstruction[i]) || cInstruction[i] == c || existSpecialSymbol(cInstruction[i])){
		if(cInstruction[i] == c){
			found = true;
		}else if(!found){
			sx[indexSx] = cInstruction[i];
			indexSx++;
		}else{
			dx[indexDx] = cInstruction[i];
			indexDx++;
		}
		i++;
	}

	sx[indexSx] = '\0';
	dx[indexDx] = '\0';

	char ins[max_length];
	for(i = 0; i < 3;i++)
		ins[i] = '1';
	string comp;
	if(isJump(cInstruction,read))
		comp = getCompValue(sx);
	else
		comp = getCompValue(dx);
	//
	int j = 0;
	for(i = 3; i < 10;i++){
		ins[i] = comp[j];
		j++;
	}

	string dest;
	if(isJump(cInstruction,read))
		dest = getDestValue("");
	else
		dest = getDestValue(sx);

	j = 0;
	for(i = 10; i < 13;i++){
		ins[i] = dest[j];
		j++;
	}

	string jump;
	if(isJump(cInstruction,read))
		jump = getJumpValue(dx);
	else
		jump = getJumpValue("");

	j = 0;
	for(i = 13; i < 16;i++){
		ins[i] = jump[j];
		j++;
	}

	ins[16] = '\0';

	strcpy(ret,ins);
}

int getPointPosition(string name){
	int i = 0;
	bool found = false;
	while(!found && name[i] != '\0'){
		if(name[i] == '.' && name[i+1] == 'a' && name[i+2] == 's' && name[i+3] == 'm')
			found = true;
		i++;
	}
	if(found)
		return i;
	else
		return -1;
}

void removeSapces(char* line,int length){
  char* i = line;
  char* j = line;
  int itr = 0;
  while(*j != 0 && itr < length){
    *i = *j++;
    if(*i != ' ')
      i++;
  	itr++;
  }
  *i = 0;
}

int main(int argc,char *argv[]){

	if(argc == 1){
		printf("Error, no Arguments!");
		return 1;
	}

	if(argc > 2){
		printf("Error, too much Arguments!");
		return 1;
	}

	fileInputName = argv[1];

	string output = (char *)malloc (max_length * sizeof (char));
	int p = getPointPosition(fileInputName);
	if(p != -1)
		memcpy(output,&fileInputName[0],p);
	output[p] = '\0';
	strcat(output,ext);
	output[p+5] = '\0';

	FILE *fin;
	fin = fopen(fileInputName,"r");

	ptr_table head;
	head = NULL;
	head = insertDefaultSymbol(head);

	int countLine = 0;
	int countAinstruction = 0;
	int variable = 0;

	string str = (char*)malloc(max_length* sizeof(char));
	size_t len;
	int read;

	while((read = getline(&str,&len,fin)) != -1){
		removeSapces(str,read);
		if(str[0] != '/' && str[1] != '/' && (isdigit(str[0]) || isalpha(str[0]) || existSpecialSymbol(str[0]))){
			if(isEtichetta(str)){
				string to = (char*)malloc(max_length* sizeof(char));
				getEtichettaValue(str,read,to);
				int line = countLine;
				head = addSymbol(head,to,line);
			} else
				countLine++;
		}
	}
	fclose(fin);

	fin = fopen(fileInputName,"r");

	FILE *fout;
	fout = fopen(output,"w");


	while((read = getline(&str,&len,fin)) != -1){
		removeSapces(str,read);
		if(str[0] != '/' && str[1] != '/' && (isdigit(str[0]) || isalpha(str[0]) || existSpecialSymbol(str[0]))){
			if(isAinstruction(str)){
				string to = (char *)malloc (max_length * sizeof (char));
				getInstructionValue(str,read,true,to);
				bool isNumber = true;
				int j = 0;
				while((isdigit(to[j]) || isalpha(to[j])) && isNumber){
					if(isalpha(to[j]))
						isNumber = false;
					j++;
				}
				if(!isNumber)
					if(!existSymbol(head,to)){
						variable++;
						int count = 15 + variable;
						head = addSymbol(head,to,count);
					}
				int symbolValue;
				if(isNumber)
					symbolValue = atoi(to);
				else
					symbolValue = getSymbolValue(head,to);
				string bin = (char *)malloc (max_length * sizeof (char));
				decToBinary(symbolValue,true,bin);
				fprintf(fout, "%s\n",bin);
			}
			else{
				string to = (char *)malloc (max_length * sizeof (char));
				getInstructionValue(str,read,false,to);
				if(isalpha(to[0]) || isdigit(to[0])){
					string ret = (char *)malloc (max_length * sizeof (char));
					cInstructionBinary(to,read,ret);
					fprintf(fout, "%s\n",ret);
					free(ret);
				}

			}
		}
	}

	fclose(fin);
	fclose(fout);

	return 0;
}