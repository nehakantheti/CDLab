#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

void followfirst(char , int , int);
void findfirst(char , int , int);
void follow(char c);

int count,n=0;
char calc_first[10][100];
char calc_follow[10][100];
int m=0;
char production[10][10], first[10];
char f[10];
int k;
char ck;
int e;

int main(int argc,char **argv)
{
	int firstIndex=0;
	int followIndex=0;
	int i,choice;
	char c;
	FILE *file = fopen("input.txt", "r");
	if (file == NULL) {
		printf("Error opening file input.txt\n");
		exit(1);
	}

	fscanf(file, "%d", &count);

	printf("\nReading %d productions from input.txt file:\n\n", count);
	for(i=0;i<count;i++)
	{
		fscanf(file, "%s", production[i]);
		printf("%s\n", production[i]);
	}

	fclose(file);
	int kay;
	char done[count];               // to not calculate duplicates
	int doneFirstIndex = -1;        //pointer to trace done array
	for(k=0;k<count;k++){   //initialising calc_first arrays
		for(kay=0;kay<100;kay++){
			calc_first[k][kay] = '!';
		}
	}
    //First Row to keep track of Calculating first variable
    //First col to keep track of calculated first symbols
    // isDone to check if first is already calulated or not(existing in done array or not)
	int firstRow = 0,firstCol,isDone;
    
	for(k=0;k<count;k++)    //Going through the productions to calculate first
	{
		c = production[k][0];
		firstCol = 0;
		isDone = 0;
		for(kay = 0; kay <= doneFirstIndex; kay++)  //checking if current character c is already present in done array
			if(c == done[kay])
				isDone = 1;
		if (isDone == 1)    //If already done, ignore all the process to find first
			continue;
		findfirst(c,0,0);   //If not found, calculate first with productionIndex = 0 and symbolIndex = 0
		doneFirstIndex+=1;  //Increment the doneFirst pointer by one and append the character to doneFirst since first is calculated
		done[doneFirstIndex] = c;
		// printf("\n First(%c)= { ",c);
		calc_first[firstRow][firstCol++] = c;   //Inserting first value into final calc_first array
		for(i=firstIndex;i<n;i++)
        {  //n to keep track of number of elements in first
			int lark = 0,chk = 0;   // lark is used to traverse in calc_first array in current row
  			for(lark=0;lark<firstCol;lark++)
            {
  				if (first[i] == calc_first[firstRow][lark]){    // Chk is used to chk whether first[i] is already in calc_first
  					chk = 1;
  					break;
				}
			}
			if(chk == 0){       // If not present add it to calc_first or else break
  		 		// printf("%c, ",first[i]);
  				calc_first[firstRow][firstCol++] = first[i];
			}
		}
		// printf("}\n");
		firstIndex=n;       // Incrementing first index  by 1 to  store next iteration symbols at crct places(because next iteration starts directly from first Index)
		firstRow++;         //Incrementing the first row to calculate first for next production.
	}
    for(int i = 0; i < firstRow;i++){
        printf("First(%c) : {", calc_first[i][0]);
        for(int j = 1;calc_first[i][j] != '!';j++){
            printf("%c, ", calc_first[i][j]);
        }
        printf("}\n");
    }
	printf("\n");
	printf("-----------------------------------------------\n\n");
	char donee[count];
	doneFirstIndex = -1;
	for(k=0;k<count;k++){
		for(kay=0;kay<100;kay++){
			calc_follow[k][kay] = '!';
		}
	}
	firstRow = 0;
	int land = 0;
	for(e=0;e<count;e++)
  	{
		ck=production[e][0];
		firstCol = 0;
		isDone = 0;
		for(kay = 0; kay <= doneFirstIndex; kay++)
			if(ck == donee[kay])
				isDone = 1;
		if (isDone == 1)
			continue;
  		land += 1;
		follow(ck);
  		doneFirstIndex+=1;
		donee[doneFirstIndex] = ck;
  		printf(" Follow(%c) = { ",ck);
  		calc_follow[firstRow][firstCol++] = ck;
  		for(i=0+followIndex;i<m;i++){
  			int lark = 0,chk = 0;
  			for(lark=0;lark<firstCol;lark++){
  				if (f[i] == calc_follow[firstRow][lark]){
  					chk = 1;
  					break;
				}
			}
			if(chk == 0){
  		 		printf("%c, ",f[i]);
  				calc_follow[firstRow][firstCol++] = f[i];
  			}
  		}
  		printf(" }\n\n");
		followIndex=m;
		firstRow++; 
	}
	char ter[10];
	for(k=0;k<10;k++){
		ter[k] = '!';
	}
	int ap,vp,sid = 0;
	for(k=0;k<=count;k++){
		for(kay=0;kay<=count;kay++){
			if(!isupper(production[k][kay]) && production[k][kay]!= '#' && production[k][kay] != '=' && production[k][kay] != '\0'){
				vp = 0;
				for(ap = 0;ap < sid; ap++){
					if(production[k][kay] == ter[ap]){
						vp = 1;
						break;
					}
				}
				if(vp == 0){
					ter[sid] = production[k][kay];
					sid ++;
				}
			}
		}
	}
	ter[sid] = '$';
	sid++;
	printf("\nThe LL(1) Parsing Table for the above grammer :-");
    printf("\n---------------------------------------------------------------------------------------------------------------------\n");
	printf("\t\t |\t");
	for(ap = 0;ap < sid; ap++){
		printf("%c\t\t",ter[ap]);
	}
    printf("\n---------------------------------------------------------------------------------------------------------------------\n");

	char first_prod[count][sid];
	for(ap=0;ap<count;ap++){
		int destiny = 0;
		k = 2;
		int ct = 0;
		char tem[100];
		while(production[ap][k] != '\0'){
			if(!isupper(production[ap][k])){
				tem[ct++] = production[ap][k];
				tem[ct++] = '_';
				tem[ct++] = '\0';
				k++;
				break;
			}
			else{
				int zap=0;
				int tuna = 0;
				for(zap=0;zap<count;zap++){
					if(calc_first[zap][0] == production[ap][k]){
						for(tuna=1;tuna<100;tuna++){
							if(calc_first[zap][tuna] != '!'){
								tem[ct++] = calc_first[zap][tuna];
							}
							else
								break;
						}
					break;
					}
				}
				tem[ct++] = '_';
			}
			k++;
		}
		int zap = 0,tuna;		
		for(tuna = 0;tuna<ct;tuna++){
			if(tem[tuna] == '#'){
				zap = 1;
			}
			else if(tem[tuna] == '_'){
				if(zap == 1){
					zap = 0;
				}
				else
					break;
			}
			else{
				first_prod[ap][destiny++] = tem[tuna];
			}
		}
	}
	char table[land][sid+1];
	doneFirstIndex = -1;
	for(ap = 0; ap < land ; ap++){
		for(kay = 0; kay < (sid + 1) ; kay++){
			table[ap][kay] = '!';
		}
	}
	for(ap = 0; ap < count ; ap++){
		ck = production[ap][0];
		isDone = 0;
		for(kay = 0; kay <= doneFirstIndex; kay++)
			if(ck == table[kay][0])
				isDone = 1;
		if (isDone == 1)
			continue;
		else{
			doneFirstIndex = doneFirstIndex + 1;
			table[doneFirstIndex][0] = ck;
		}
	}
	for(ap = 0; ap < count ; ap++){
		int tuna = 0;
		while(first_prod[ap][tuna] != '\0'){
			int to,ni=0;
			for(to=0;to<sid;to++){
				if(first_prod[ap][tuna] == ter[to]){
					ni = 1;
				}
			}
			if(ni == 1){
				char xz = production[ap][0];
				int cz=0;
				while(table[cz][0] != xz){
					cz = cz + 1;
				}
				int vz=0;
				while(ter[vz] != first_prod[ap][tuna]){
					vz = vz + 1;
				}
				table[cz][vz+1] = (char)(ap + 65);
			}
			tuna++;
		}
	}
	for(k=0;k<sid;k++){
		for(kay=0;kay<100;kay++){
			if(calc_first[k][kay] == '!'){
				break;
			}
			else if(calc_first[k][kay] == '#'){
				int fz = 1;
				while(calc_follow[k][fz] != '!'){
					char xz = production[k][0];
					int cz=0;
					while(table[cz][0] != xz){
						cz = cz + 1;
					}
					int vz=0;
					while(ter[vz] != calc_follow[k][fz]){
						vz = vz + 1;
					}
					table[k][vz+1] = '#';
					fz++;	
				}
				break;
			}
		}
	}
	for(ap = 0; ap < land ; ap++){
		printf("\t   %c\t|\t",table[ap][0]);
		for(kay = 1; kay < (sid + 1) ; kay++){
			if(table[ap][kay] == '!')
				printf("\t\t");
			else if(table[ap][kay] == '#')
				printf("%c=#\t\t",table[ap][0]);
			else{
				int mum = (int)(table[ap][kay]);
				mum -= 65;
				printf("%s\t\t",production[mum]);
			}
		}
		printf("\n");
		printf("---------------------------------------------------------------------------------------------------------------------");
		printf("\n");
	}
	int j;
	printf("\n\nPlease enter the desired INPUT STRING = ");
	char input[100];
	char ch;
	scanf("%s%c",input,&ch);
	printf("\n\t\t\t\t\t===========================================================================\n");
	printf("\t\t\t\t\t\tStack\t\t\tInput\t\t\tAction");
	printf("\n\t\t\t\t\t===========================================================================\n");
	int i_doneFirstIndex = 0,s_doneFirstIndex = 1;
	char stack[100];
	stack[0] = '$';
	stack[1] = table[0][0];
	while(s_doneFirstIndex != -1){
		printf("\t\t\t\t\t\t");
		int vamp = 0;
		for(vamp=0;vamp<=s_doneFirstIndex;vamp++){
			printf("%c",stack[vamp]);
		}
		printf("\t\t\t");
		vamp = i_doneFirstIndex;
		while(input[vamp] != '\0'){
			printf("%c",input[vamp]);
			vamp++;
		}
		printf("\t\t\t");
		char her = input[i_doneFirstIndex];
		char him = stack[s_doneFirstIndex];
		s_doneFirstIndex--;
		if(!isupper(him)){
			if(her == him){
				i_doneFirstIndex++;
				printf("POP ACTION\n");
			}
			else{
				printf("\nString Not Accepted by LL(1) Parser !!\n");
				exit(0);
			}
		}
		else{
			for(i=0;i<sid;i++){
				if(ter[i] == her)
					break;
			}
			char produ[100];
			for(j=0;j<land;j++){
				if(him == table[j][0]){
					if (table[j][i+1] == '#'){
						printf("%c=#\n",table[j][0]);
						produ[0] = '#';
						produ[1] = '\0';
					}
					else if(table[j][i+1] != '!'){
						int mum = (int)(table[j][i+1]);
						mum -= 65;
						strcpy(produ,production[mum]);
						printf("%s\n",produ);
					}
					else{
						printf("\nString Not Accepted by LL(1) Parser !!\n");
						exit(0);
					}
				}
			}
			int le = strlen(produ);
			le = le - 1;
			if(le == 0){
				continue;
			}
			for(j=le;j>=2;j--){
				s_doneFirstIndex++;
				stack[s_doneFirstIndex] = produ[j];
			}
		}
	}
	printf("\n\t\t\t=======================================================================================================================\n");
	if (input[i_doneFirstIndex] == '\0'){
		printf("\t\t\t\t\t\t\t\tYOUR STRING HAS BEEN ACCEPTED !!\n");
	}
	else
		printf("\n\t\t\t\t\t\t\t\tYOUR STRING HAS BEEN REJECTED !!\n");
	printf("\t\t\t=======================================================================================================================\n");
}

void follow(char c)
{
	int i ,j;
	if(production[0][0]==c){
 		f[m++]='$';
 	}
 	for(i=0;i<10;i++)
 	{
  		for(j=2;j<10;j++)
  		{
   			if(production[i][j]==c)
   			{
    			if(production[i][j+1]!='\0'){
					followfirst(production[i][j+1],i,(j+2));
 				}
    			if(production[i][j+1]=='\0' && c!=production[i][0]){
     				follow(production[i][0]);
				}
   			}   
  		}
 	}
}

void findfirst(char c ,int q1 , int q2)
{
	int j;
	if(!(isupper(c))){
		first[n++]=c;
	}
	for(j=0;j<count;j++)
	{
		if(production[j][0]==c)
		{
			if(production[j][2]=='#'){  // If RHS is epsilon
				if(production[q1][q2] == '\0')  //If it ends after epsilon, add # to first
					first[n++]='#';
				else if(production[q1][q2] != '\0' && (q1 != 0 || q2 != 0))
				{
					findfirst(production[q1][q2], q1, (q2+1));
				}
				else
					first[n++]='#';
			}
			else if(!isupper(production[j][2])){    // If first character in RHS is Terminal
				first[n++]=production[j][2];
			}
			else {  // If first character in RHS is Non terminal
				findfirst(production[j][2], j, 3);
			}
		}
	}	
}

void followfirst(char c, int c1 , int c2)
{
    int k;
    if(!(isupper(c)))
		f[m++]=c;
	else{
		int i=0,j=1;
		for(i=0;i<count;i++)
		{
			if(calc_first[i][0] == c)
				break;
		}
		while(calc_first[i][j] != '!')
		{
			if(calc_first[i][j] != '#'){
				f[m++] = calc_first[i][j];
			}
			else{
				if(production[c1][c2] == '\0'){
					follow(production[c1][0]);
				}
				else{
					followfirst(production[c1][c2],c1,c2+1);
				}
			}
			j++;
		}
	}
}