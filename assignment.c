#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"hash.h"
#include"list.h"
#include"initialise.h"
#include"assignment.h"
#include"update.h"
#include"clara.h"
#include"lsh.h"
#include<time.h>
#define bufSize 2048
#define bafSize 8192

void insertassig(double ** pammatr,struct clustlist * clist,struct node * item,int choice, int length)
{
	int i,j,z;

	i=pammatr[0][0];
	if (clist[i].head == NULL)//an head null kane eisagwgi
	{
		clist[i].head=malloc(sizeof(struct centlist));
		clist[i].head->id=item->id;
		if(choice==0)
			clist[i].head->key=item->key;
		else
		{
			clist[i].head->key1=malloc(length * sizeof(double));
			for(z=0;z<length;z++)
				clist[i].head->key1[z]=item->key1[z];
			//clist[i].head->key1=item->key1;
		}
		clist[i].head->distance=pammatr[0][1];
		j=pammatr[1][0];
		clist[i].head->listnearid=j;
		clist[i].head->distancenear=pammatr[1][1];
		clist[i].head->next=NULL;
	}
	else
	{
		struct centlist *current = clist[i].head;
		while (current->next != NULL) 
			current = current->next;
		current->next= malloc(sizeof(struct centlist));
		current->next->id=item->id;
		if(choice==0)
			current->next->key=item->key; 
		else
		{
			current->next->key1=malloc(length * sizeof(double));
			for(z=0;z<length;z++)
				current->next->key1[z]=item->key1[z];
			//current->key1=item->key1;
		}
		current->next->distance=pammatr[0][1];
		j=pammatr[1][0];
		current->next->listnearid=j;
		current->next->distancenear=pammatr[1][1];
		current->next->next=NULL;
	}
}


void pamassig(struct list * lista,struct clustlist * clist,double ** indistmatr,int length,int counter,int cent,int choice)//concentrate
{
	int i,j,z;
	int flag;
	int centplace[cent];
	double **pammatr;
	struct node * temp;
	pammatr=malloc(cent*sizeof(double*));
	for(i=0;i<cent;i++)
		pammatr[i]=malloc(2*sizeof(double));
	z=0;
	for(i=0;i<cent;i++)//vriskei se poia thesi tou pinaka apostasewn einai to kathe kentroides
	{
		temp=lista->head;
		for(j=0;j<counter;j++)
		{
			if(temp->id==clist[i].centro->id)
			{
				centplace[z]=j;
				z++;
				break;
			}
			temp=temp->next;	
		}
	}
	temp=lista->head;
	for(i=0;i<counter;i++)
	{
		
		for(j=0;j<cent;j++)
		{
			flag=0;
			if(i!=centplace[j])
				flag=1;
		}
		if(flag==1)
		{
			for(j=0;j<cent;j++)
			{
				pammatr[j][0]=j;
				pammatr[j][1]=indistmatr[i][centplace[j]];
			}
			bubble_sort2d(pammatr, cent);
			if(choice==3)
				length=counter;
			insertassig(pammatr,clist,temp,choice,length);//isws eksw apo to flag==1
		}
		temp=temp->next;	
	}
	for(i=0;i<cent;i++)
		free(pammatr[i]);
	free(pammatr);
	
}

double calcj(struct clustlist * clist,int cent)
{
	int i;
	double sum=0;
	struct centlist * temp;
	for(i=0;i<cent;i++)
	{
		temp=clist[i].head;
		while(temp!=NULL)
		{
			sum+=temp->distance;
			temp=temp->next;		
		}
	}
	return sum;
}



void medpamham(struct list * lista,struct clustlist * clist,int length,int counter,int cent,int choice)//medoids
{
	int i,j,z;
	int flag;
	char *token,*token2;
	int centplace[cent];
	double **pammatr;
	double distance;
	struct node * temp;
	pammatr=malloc(cent*sizeof(double*));
	for(i=0;i<cent;i++){
		pammatr[i]=malloc(2*sizeof(double));
	}
	z=0;
	temp=lista->head;
	while(temp!=NULL)
	{
		flag=1;
		for(i=0;i<cent;i++)
		{	
			if(temp->id==clist[i].centro->id)
			{
				flag=0;
				break;
			}
		}
		if(flag==1)
		{
			token=malloc((length+1)*sizeof(char));
			turnintobinary(temp->key ,length ,token);
			for(j=0;j<cent;j++)
			{
				distance=0;
				pammatr[j][0]=j;
				token2=malloc((length+1)*sizeof(char));
				turnintobinary(clist[j].centro->key ,length ,token2);
				for(z=0;z<length;z++)
				{
					if(token[z]!=token2[z])
						distance++;
				}
				pammatr[j][1]=distance;
				free(token2);
			}
			bubble_sort2d(pammatr, cent);
			insertassig(pammatr,clist,temp,choice,length);//isws eksw apo to flag==1
			free(token);
		}
		z++;
		temp=temp->next;	
	}
	for(i=0;i<cent;i++)
		free(pammatr[i]);
	free(pammatr);
}




void medpamcos(struct list * lista,struct clustlist * clist,int length,int counter,int cent,int choice)//medoids
{
	int i,j,z;
	int flag;
	double sum,sum1,sum2;
	int centplace[cent];
	double **pammatr;
	double distance;
	struct node * temp;
	pammatr=malloc(cent*sizeof(double*));
	for(i=0;i<cent;i++){
		pammatr[i]=malloc(2*sizeof(double));
	}
	temp=lista->head;
	while(temp!=NULL)
	{
		flag=1;
		for(i=0;i<cent;i++)
		{	
			if(temp->id==clist[i].centro->id)
			{
				flag=0;
				break;
			}
		}
		if(flag==1)
		{

			for(j=0;j<cent;j++)
			{
				pammatr[j][0]=j;
				sum=0,sum1=0,sum2=0;
				for(z=0;z<length;z++)//upologismos typou cosine
				{
					sum+=temp->key1[z] * clist[j].centro->key1[z];
					sum1+=temp->key1[z] * temp->key1[z];
					sum2+=clist[j].centro->key1[z] * clist[j].centro->key1[z];
				}
				if(sum1>0 && sum2>0)//epeidi einai riza, elegxos na einai diaforetiko ap to 0
				{
					distance=1-(sum/(sqrt(sum1) * sqrt(sum2)));
				}
				else 
				{
					distance=0;
				}
				pammatr[j][1]=distance;
			}
			bubble_sort2d(pammatr, cent);
			insertassig(pammatr,clist,temp,choice,length);//isws eksw apo to flag==1
		}
		temp=temp->next;	
	}
	for(i=0;i<cent;i++)
		free(pammatr[i]);
	free(pammatr);	
}


void medpameucl(struct list * lista,struct clustlist * clist,int length,int counter,int cent,int choice)//medoids
{
	int i,j,z;
	int flag;
	double sum,sum1,sum2;
	int centplace[cent];
	double **pammatr;
	double distance;
	struct node * temp;
	pammatr=malloc(cent*sizeof(double*));
	for(i=0;i<cent;i++){
		pammatr[i]=malloc(2*sizeof(double));
	}
	temp=lista->head;
	while(temp!=NULL)
	{
		flag=1;
		for(i=0;i<cent;i++)
		{	
			if(temp->id==clist[i].centro->id)
			{
				flag=0;
				break;
			}
		}
		if(flag==1)
		{
			for(j=0;j<cent;j++)
			{
				pammatr[j][0]=j;
				sum=0,sum2=0;
				for(z=0;z<length;z++)//typos euclidean
				{
					sum=temp->key1[z] - clist[j].centro->key1[z];
					sum=sum*sum;
					sum2+=sum;
					sum=0;
				}
				distance=sqrt(sum2);//upologismos apostasis
				pammatr[j][1]=distance;
			}
			bubble_sort2d(pammatr, cent);
			insertassig(pammatr,clist,temp,choice,length);//isws eksw apo to flag==1
		}
		temp=temp->next;	
	}
	for(i=0;i<cent;i++)
		free(pammatr[i]);
	free(pammatr);
}

void medpammatr(struct list * lista,struct clustlist * clist,int length,int counter,int cent,int choice)//medoids
{
	int i,j,z;
	int flag;
	double sum,sum1,sum2;
	int centplace[cent];
	double **pammatr;
	double distance;
	struct node * temp;
	pammatr=malloc(cent*sizeof(double*));
	for(i=0;i<cent;i++){
		pammatr[i]=malloc(2*sizeof(double));
	}
	z=0;
	for(i=0;i<cent;i++)//vriskei se poia thesi tou pinaka einai to kathe kentroides
	{
		temp=lista->head;
		for(j=0;j<length;j++)
		{
			if(temp->id==clist[i].centro->id)
			{
				centplace[z]=j;
				z++;
				break;
			}
			temp=temp->next;	
		}
	}
	temp=lista->head;
	while(temp!=NULL)
	{
		flag=1;
		for(i=0;i<cent;i++)
		{	
			if(temp->id==clist[i].centro->id)
			{
				flag=0;
				break;
			}
		}
		if(flag==1)
		{
			for(j=0;j<cent;j++)
			{
				pammatr[j][0]=j;
				distance=temp->key1[centplace[j]];//upologismos apostasis
				pammatr[j][1]=distance;
			}
			bubble_sort2d(pammatr, cent);
			insertassig(pammatr,clist,temp,choice,length);//isws eksw apo to flag==1
		}
		temp=temp->next;	
	}
	for(i=0;i<cent;i++)
		free(pammatr[i]);
	free(pammatr);
}

double hamdistance(struct node *temp,int length,struct node *temp2)
{
	char token[65],token2[65];
	double distance;
	int j,z;
	//token=malloc((length+1)*sizeof(char));
	turnintobinary(temp->key ,length ,token);
	distance=0;
	//token2=malloc((length+1)*sizeof(char));
	turnintobinary(temp2->key ,length ,token2);
	for(z=0;z<length;z++)
	{
		if(token[z]!=token2[z])
			distance++;
	}
	//free(token2);
	//free(token);
	return distance;
}


double cosdistance(struct node *temp,int length,struct node *temp2)
{
	double distance,sum,sum1,sum2;
	int j,z;
	distance=0;
	for(z=0;z<length;z++)
	{
		sum=0,sum1=0,sum2=0;
		for(z=0;z<length;z++)//upologismos typou cosine
		{
			sum+=temp->key1[z] * temp2->key1[z];
			sum1+=temp->key1[z] * temp->key1[z];
			sum2+=temp2->key1[z] * temp2->key1[z];
		}
		if(sum1>0 && sum2>0)//epeidi einai riza, elegxos na einai diaforetiko ap to 0
		{
			distance=1-(sum/(sqrt(sum1) * sqrt(sum2)));
		}
		else 
		{
			distance=0;
		}
	}
	return distance;
}


double eucldistance(struct node *temp,int length,struct node *temp2)
{
	double distance,sum,sum1,sum2;
	int j,z;
	distance=0;
	sum=0;
	sum2=0;
	for(z=0;z<length;z++)//typos euclidean
	{
		sum=temp->key1[z] - temp2->key1[z];
		sum=sum*sum;
		sum2+=sum;
		sum=0;
	}
	distance=sqrt(sum2);//upologismos apostasis
	return distance;
}

double matrdistance(struct list * lista,struct node *temp,int length,struct node *temp2)
{
	double distance;
	int pos=0;
	struct node * templist;
	templist=lista->head;
	while(templist->id!=temp2->id)
	{
		templist=templist->next;
		pos++;
	}
	distance=temp->key1[pos];
	return distance;
}
void lshassign(struct list * inlist,struct clustlist * clist,int length,int readcount,int cent,int choice,int k,int L)
{
	struct hashtable ** hasht;
	int i;
	long hashsize=2;
	int temphash=readcount/8;
	double rad=100000;//paradoxi
	if(choice==0)
	{
		for(i=1;i<k;i++)
		{
			hashsize*=2;
		}
		if(hashsize>temphash)
			hashsize=temphash;
		hasht = malloc(L * sizeof(struct hashtable));
		createhash(hasht,L,hashsize);//dimiourgia hashtable
		//printf("pawwwwww\n");
		lshhaminit(hasht,clist,inlist,L,k,length,readcount,cent);
		//freehasht(hasht,L,hashsize);
		//free(hasht);
	
	}
	else if(choice==2)
	{
		hashsize=readcount/16;
		hasht = malloc(L * sizeof(struct hashtable));
		createhash(hasht,L,hashsize);//dimiourgia hashtable
		lsheuclinit(hasht,clist,inlist,L,k,length,readcount,cent);
		//freehasht(hasht,L,hashsize);
		//free(hasht);
	
	}
	


}




