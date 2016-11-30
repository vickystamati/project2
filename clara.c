#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include <inttypes.h>
#include"hash.h"
#include"list.h"
#include "initialise.h"
#include "assignment.h"
#include "update.h"
#include "clara.h"
#include"lsh.h"
#define bufSize 2048
#define bafSize 8192


void claraf(struct clustlist * clist , struct list * inlist,int length,int readcount,int choice,int cent)
{
	int i,j,z,m,counter;
	int random,nnum,s,flag;
	int randmatr[cent];
	int *listmatr;
	double jval,jtemp;
	struct clustlist * clisttemp,*clistfinal;
	struct centlist * centtemp,*t2;
	struct node * temp;
	struct list * lista=malloc(sizeof(struct list));
	lista->head=NULL;
	nnum=40 + 2*cent;
	listmatr=malloc(nnum*sizeof(int));
	i=0;
	while(i<nnum)
	{
		flag=0;
		random=1+ (rand() /( RAND_MAX + 1.0))*(readcount-1);
		if(i==0)
		{
			listmatr[i]=random;
		}
		else
		{
			flag=0;
			for(j=0;j<i;j++)
				if(listmatr[j]==random)
					flag=1;
			if(flag==1)
				continue;
			else
				listmatr[j]=random;
		}
		temp=inlist->head;
		for(j=0;j<random;j++)
			temp=temp->next;
		if(choice==0)
			insert(lista,temp->key,temp->id);
		else
			insertcosine(lista,temp->key1,temp->id,length);
		i++;
	}
	i=0;
	while(i<cent)
	{
		random=1+ (rand() /( RAND_MAX + 1.0))*(nnum-1);
		if(i==0)
		{
			randmatr[i]=random;
		}
		else
		{
			flag=0;
			for(j=0;j<i;j++)
				if(randmatr[j]==random)
					flag=1;
			if(flag==1)
				continue;
			else
				randmatr[j]=random;
		}
		temp=lista->head;
		for(j=0;j<random;j++)
		{
			temp=temp->next;
		}
		clist[i].centro->id=temp->id;
		if(choice==0)
			clist[i].centro->key=temp->key;
		else
		{
			for(z=0;z<length;z++)
				clist[i].centro->key1[z]=temp->key1[z];
		}
		i++;
	}
	if(choice==0)
		medpamham(lista,clist,length,readcount,cent,choice);
	else if(choice==1)
		medpamcos(lista,clist,length,readcount,cent,choice);
	else if(choice==2)
		medpameucl(lista,clist,length,readcount,cent,choice);
	else if(choice==3)
		medpammatr(lista,clist,length,readcount,cent,choice);
	jval=calcj(clist ,cent);
	clistfinal=malloc(cent*sizeof(struct clustlist));
	clisttemp=malloc(cent*sizeof(struct clustlist));
	for(i=0;i<cent;i++)
	{
		clistfinal[i].centro=malloc(sizeof(struct centlist));
		clisttemp[i].centro=malloc(sizeof(struct centlist));
		if(choice!=0)
		{
			clisttemp[i].centro->key1=malloc(length*sizeof(double));
			clistfinal[i].centro->key1=malloc(length*sizeof(double));
		}
		clisttemp[i].head=NULL;
		clistfinal[i].head=NULL;
	}
	counter=0;
	for(i=0;i<cent;i++)
	{
		for(j=0;j<cent;j++)
		{
			centtemp=clist[j].head;
			while(centtemp!=NULL)
			{	
				for(m=0;m<cent;m++)//pername ta kentroidi sto temp struct
				{
				
					if(i==m)
					{
						clisttemp[m].centro->id=centtemp->id;
						if(choice==0)
							clisttemp[m].centro->key=centtemp->key;
						else
							for(z=0;z<length;z++)
								clisttemp[m].centro->key1[z]=centtemp->key1[z];	
					}
					else
					{
						clisttemp[m].centro->id=clist[m].centro->id;
						if(choice==0)
							clisttemp[m].centro->key=clist[m].centro->key;
						else
							for(z=0;z<length;z++)
								clisttemp[m].centro->key1[z]=clist[m].centro->key1[z];
					}
				}
				if(choice==0)
					medpamham(lista ,clisttemp,length ,readcount ,cent ,choice );
				else if(choice==1)
					medpamcos(lista ,clisttemp,length ,readcount ,cent ,choice );
				else if(choice==2)
					medpameucl(lista ,clisttemp,length ,readcount ,cent ,choice );
				else if(choice==3)
					medpammatr(lista ,clisttemp,length ,readcount ,cent ,choice );
				jtemp=calcj(clisttemp ,cent);

				if(jtemp<jval)
				{
					freeclustlist(clistfinal,cent,choice);
					swapclist(clistfinal,clisttemp,cent,length,choice);
					freeclustlist(clisttemp,cent,choice);
					jval=jtemp;

				}
				else
					freeclustlist(clisttemp,cent,choice);
				centtemp=centtemp->next;
				counter++;

			}
		}
	}
	if(clistfinal[0].head!=NULL)
	{
		freeclustlist(clist,cent,choice);
		swapclist(clist,clistfinal,cent,length,choice);
	}
	freeclustlist(clist,cent,choice);
	if(choice==0)
		medpamham(inlist ,clist,length ,readcount ,cent ,choice );
	else if(choice==1)
		medpamcos(inlist ,clist,length ,readcount ,cent ,choice );
	else if(choice==2)
		medpameucl(inlist ,clist,length ,readcount ,cent ,choice );
	else if(choice==3)
		medpammatr(inlist ,clist,length ,readcount ,cent ,choice );
	
	
}


void clara(struct clustlist * clist,struct list * inlist,int length,int readcount,int choice,int cent)
{
	struct clustlist*clistptr;
	int i,s=5;//apo theoria
	double jsum,jsumclara;
	clistptr=malloc(cent*sizeof(struct clustlist));
	for(i=0;i<cent;i++)
	{
		clistptr[i].centro=malloc(sizeof(struct node));
		clistptr[i].head=NULL;
	}
	if(choice!=0)
		for(i=0;i<cent;i++)
			clistptr[i].centro->key1=malloc(length*sizeof(double));
	for(i=0;i<s;i++)
	{
		claraf(clistptr,inlist,length,readcount,choice,cent);
		if(i==0)
		{
			jsumclara=calcj(clistptr ,cent );
			swapclist(clist,clistptr,cent,length,choice);
			freeclustlist(clistptr,cent,choice);
		}
		else
		{
			jsum=calcj(clistptr ,cent );
			if(jsum<jsumclara)
			{
				jsumclara=jsum;
				freeclustlist(clist,cent,choice);
				swapclist(clist,clistptr,cent,length,choice);
			}
			freeclustlist(clistptr,cent,choice);
		}
	}
}

