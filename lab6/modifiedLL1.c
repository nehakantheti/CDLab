#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

void addFirstSet(char symbol, int prodIndex, int symbolIndex);
void addFollowSetFirst(char symbol, int prodIndex, int symbolIndex);
void findFollowSet(char symbol);

int numProductions, numFirstSet=0;
char firstSetResults[10][100];
char followSetResults[10][100];
int followSetCount=0;
char productions[10][10], firstSet[10];
char followSet[10];
int firstSetIndex;
char currentSymbol;
int epsilonFlag;

int main(int argc,char **argv)
{
	int firstSetPointer=0;
	int followSetPointer=0;
	int i, choice;
	char nonTerminal, nextSymbol;
	printf("How many productions? : ");
	scanf("%d",&numProductions);
	printf("\nEnter %d productions in form A=B where A and B are grammar symbols:\n\n",numProductions);
	for(i=0;i<numProductions;i++)
	{	
		scanf("%s%c",productions[i],&nextSymbol);
	}
	
	char processedFirstSet[numProductions];
	int processedPtr = -1;

	// Initialize firstSetResults array
	for(int prod=0; prod<numProductions; prod++){
		for(int sym=0; sym<100; sym++){
			firstSetResults[prod][sym] = '!';
		}
	}
	
	// Compute First Set for each non-terminal
	for(int prod=0; prod<numProductions; prod++)
	{
		nonTerminal = productions[prod][0];
		int firstSetPos = 0;
		int alreadyProcessed = 0;

		// Check if the first set for this non-terminal is already computed
		for(int index = 0; index <= processedPtr; index++)
			if(nonTerminal == processedFirstSet[index])
				alreadyProcessed = 1;
				
		if (alreadyProcessed == 1)
			continue;
		
		addFirstSet(nonTerminal, 0, 0);
		processedPtr += 1;
		processedFirstSet[processedPtr] = nonTerminal;
		printf("\n First(%c) = { ",nonTerminal);
		firstSetResults[firstSetPointer][firstSetPos++] = nonTerminal;

		// Add the first set symbols to firstSetResults
		for(i=0+firstSetPointer; i<numFirstSet; i++){
			int exists = 0, alreadyAdded = 0;
  			for(int check = 0; check<firstSetPos; check++){
  				if (firstSet[i] == firstSetResults[firstSetPointer][check]){
  					alreadyAdded = 1;
  					break;
				}
			}
			if(alreadyAdded == 0){
  		 		printf("%c, ",firstSet[i]);
  				firstSetResults[firstSetPointer][firstSetPos++] = firstSet[i];
			}
		}
		printf("}\n");
		firstSetPointer = numFirstSet;
	}
	printf("\n");
	printf("-----------------------------------------------\n\n");
	
	// Initialize followSetResults array
	char processedFollowSet[numProductions];
	processedPtr = -1;
	for(int prod=0; prod<numProductions; prod++){
		for(int sym=0; sym<100; sym++){
			followSetResults[prod][sym] = '!';
		}
	}

	firstSetPointer = 0;
	int ruleCounter = 0;
	
	// Compute Follow Set for each non-terminal
	for(int prod=0; prod<numProductions; prod++)
  	{
		currentSymbol = productions[prod][0];
		int followSetPos = 0;
		int alreadyProcessed = 0;

		// Check if the follow set for this non-terminal is already computed
		for(int index = 0; index <= processedPtr; index++)
			if(currentSymbol == processedFollowSet[index])
				alreadyProcessed = 1;
				
		if (alreadyProcessed == 1)
			continue;

  		ruleCounter += 1;
		findFollowSet(currentSymbol);
  		processedPtr += 1;
		processedFollowSet[processedPtr] = currentSymbol;
  		printf(" Follow(%c) = { ",currentSymbol);
  		followSetResults[firstSetPointer][followSetPos++] = currentSymbol;

		// Add follow set symbols to followSetResults
  		for(i=0+followSetPointer; i<followSetCount; i++){
  			int exists = 0, alreadyAdded = 0;
  			for(int check=0; check<followSetPos; check++){
  				if (followSet[i] == followSetResults[firstSetPointer][check]){
  					alreadyAdded = 1;
  					break;
				}
			}
			if(alreadyAdded == 0){
  		 		printf("%c, ",followSet[i]);
  				followSetResults[firstSetPointer][followSetPos++] = followSet[i];
  			}
  		}
  		printf(" }\n\n");
		followSetPointer = followSetCount;
		firstSetPointer++; 
	}
	
	// Remaining code for LL(1) parsing table construction goes here
}

void findFollowSet(char symbol)
{
	int prodIndex, symbolIndex;
	if(productions[0][0]==symbol){
 		followSet[followSetCount++]='$';
 	}
 	
 	for(prodIndex=0; prodIndex<10; prodIndex++)
 	{
  		for(symbolIndex=2; symbolIndex<10; symbolIndex++)
  		{
   			if(productions[prodIndex][symbolIndex]==symbol)
   			{
    			if(productions[prodIndex][symbolIndex+1]!='\0'){
					addFollowSetFirst(productions[prodIndex][symbolIndex+1], prodIndex, symbolIndex+2);
 				}
    			if(productions[prodIndex][symbolIndex+1]=='\0' && symbol!=productions[prodIndex][0]){
     				findFollowSet(productions[prodIndex][0]);
				}
   			}   
  		}
 	}
}

void addFirstSet(char symbol ,int prodIndex , int symbolIndex)
{
	int i;
	if(!(isupper(symbol))){
		firstSet[numFirstSet++] = symbol;
	}
	for(i=0; i<numProductions; i++)
	{
		if(productions[i][0]==symbol)
		{
			if(productions[i][2]=='#'){  // Epsilon production
				if(productions[prodIndex][symbolIndex] == '\0')
					firstSet[numFirstSet++]='#';
				else if(productions[prodIndex][symbolIndex] != '\0' && (prodIndex != 0 || symbolIndex != 0))
				{
					addFirstSet(productions[prodIndex][symbolIndex], prodIndex, symbolIndex+1);
				}
				else
					firstSet[numFirstSet++]='#';
			}
			else if(!isupper(productions[i][2])){  // Terminal
				firstSet[numFirstSet++] = productions[i][2];
			}
			else {
				addFirstSet(productions[i][2], i, 3);
			}
		}
	}	
}

void addFollowSetFirst(char symbol, int prodIndex, int symbolIndex)
{
    int i;
    if(!(isupper(symbol)))
		followSet[followSetCount++] = symbol;
	else{
		int firstSetIndex=0, firstSetPos=1;
		for(i=0; i<numProductions; i++)
		{
			if(firstSetResults[i][0] == symbol)
				break;
		}
		while(firstSetResults[i][firstSetPos] != '!')
		{
			if(firstSetResults[i][firstSetPos] != '#'){
				followSet[followSetCount++] = firstSetResults[i][firstSetPos];
			}
			else{
				if(productions[prodIndex][symbolIndex] == '\0'){
					findFollowSet(productions[prodIndex][0]);
				}
				else{
					addFollowSetFirst(productions[prodIndex][symbolIndex],prodIndex,symbolIndex+1);
				}
			}
			firstSetPos++;
		}
	}
}