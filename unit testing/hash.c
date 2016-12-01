#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"hash.h"
#include"list.h"
#include"initialise.h"


void createhash(struct hashtable**hasht, int L,int hashsize)
{
	int i,j;
	for(i=0;i<L;i++)
	{
		hasht[i] = malloc(hashsize*sizeof(struct hashtable));
		for(j=0;j<hashsize;j++)
		{
			hasht[i][j].lista=malloc(sizeof(struct list));
			hasht[i][j].lista->head=NULL;
		}
	}
}

void freegf(int ** gfun,int L)
{
	int i;
	for(i=0;i<L;i++)
		free(gfun[i]);
}

void freehasht(struct hashtable ** hasht,int L,int hashsize)
{
	int i,j;
		printf("DSADSA1");
	for(i=0;i<L;i++)
	{
		printf("DSADSA1");
		for(j=0;j<hashsize;j++)
		{
		printf("DSADSA2");
			freehlist(hasht[i][j].lista);
			free(hasht[i][j].lista);
		}
printf("DSADSA3");
		free(hasht[i]);
	}
	
}

void freefix(double ** hfix,int k, int L)
{
	int i;
	for(i=0;i<k*L;i++)
		free(hfix[i]);
}


void freehtable(double ***htable,int L,int k)
{
	int i;
	for(i=0;i<L;i++)
	{
		free(htable[i]);
	}
}
