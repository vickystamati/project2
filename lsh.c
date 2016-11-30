#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include <inttypes.h>
#include"hash.h"
#include"list.h"
#include"initialise.h"
#include"assignment.h"
#include"update.h"
#include"clara.h"
#include"lsh.h"
#define bufSize 2048
#define bafSize 8192

void lshhaminit(struct hashtable ** hasht,struct clustlist * clist,struct list * inlist,int L,int k,int length,int readcount,int cent)
{
	struct centlist * tata;
	struct node * temp,*insertnode;
	char * token,token2[k];
	int ** gfun;
	int random,i,j,z,loop,tloop;
	double ** pammatr,dist,distmin,radius;
	struct distlist * dilist;
	long long binarynum,decimalnum;
	radius=firstrad(clist,length,cent,0);
	printf("radius %f\n",radius);
	pammatr=malloc(2*sizeof(double*));
	for(i=0;i<2;i++){
		pammatr[i]=malloc(2*sizeof(double));
	}
	dilist=malloc(cent*sizeof(struct distlist));
	for(i=0;i<cent;i++)
		dilist[i].head=NULL;
	gfun=malloc(L*sizeof(int*));
	for(i=0;i<L;i++)//kataskevazoyme ti sinartisi g
	{
		gfun[i]=malloc(k*sizeof(int));
		for(j=0;j<k;j++)
		{
			random=1+ (rand() /( RAND_MAX + 1.0))*(length-1);
			gfun[i][j]=random;//epilogi,mesw kanonikis tuxaiotitas, arithmwn poy antistixoun sta ID twn h
		}
	}
	temp=inlist->head;
	while(temp!=NULL)
	{
		
		j=0;
		token=malloc((length+1)*sizeof(char));
		turnintobinary(temp->key,length,token);
		while(j<L)
		{
			for(i=0;i<k;i++)
			{
				token2[i]=token[gfun[j][i]];
			}
			binarynum=atoll(token2);//pairnei tin duadiki simvoloseira kai tin kanei typo long long int
			decimalnum=turnintodecimal(binarynum);//metatrepei to diadiko se dekadiko wste na paei sto swsto bucket
			insert(hasht[j][decimalnum].lista,temp->key,temp->id);
			j++;
		}
		free(token);
		temp=temp->next;
		loop++;
	}
	loop=1;//paradoxi gia na mpei
	while(loop!=0)
	{
		//printf("mpainwww %f\n",radius);
		loop=0;//to kanei kathe fora 0 wste na to athroisei stin search
		for(i=0;i<cent;i++)
		{
			temp=clist[i].centro;
			j=0;
			token=malloc((length+1)*sizeof(char));
			turnintobinary(temp->key,length,token);
			while(j<L)
			{
				for(z=0;z<k;z++)
				{
					token2[z]=token[gfun[j][z]];
				}
				binarynum=atoll(token2);//pairnei tin duadiki simvoloseira kai tin kanei typo long long int
				decimalnum=turnintodecimal(binarynum);//metatrepei to diadiko se dekadiko wste na paei sto swsto bucket
				//printf("sto  %d, tou %d\n",i,j);
				tloop=search(hasht[j][decimalnum].lista,temp->key,temp->id,&dilist[i],length,radius,L);//lsh search
				dilistcount(&dilist[i]);
				if(tloop!=0)
					loop++;
				j++;
			}
			free(token);	
		}
		radius*=2;
	}
	erasedouble(dilist,cent);
/*for(i=0;i<cent;i++)
	dilistcount(&dilist[i]);*/
	insertfromhash(pammatr,clist,dilist,cent,length,0);
	createnewlist(inlist,clist,cent,length,readcount);
	cleancentroids(clist,cent);
	for(i=0;i<cent;i++)
		freedlist(&dilist[i]);
	free(dilist);
	free(pammatr[0]);
	free(pammatr[1]);
	free(pammatr);
	freegf(gfun,L);
	free(gfun);

	/*for(i=0;i<cent;i++)
	{
		printf("kentrpo %lu",clist[i].centro->id);
		tata=clist[i].head;
		while(tata!=NULL)
		{
			printf("mesa %lu\n",tata->id);
			tata=tata->next;			
		}
	}*/
}



void lsheuclinit(struct hashtable ** hasht,struct clustlist * clist,struct list * inlist,int L,int k,int length,int readcount,int cent){}/*
{
	struct node * temp,*insertnode;
	char * token,token2[k];
	int ** gfun,radius;
	int random,i,j,z;
	double ** pammatr,dist,distmin;
	struct distlist * dilist;
	long long binarynum,decimalnum;
	long modnum=1;
	if(k>10 && L>30)//elegxos gia k kai L
	{
		printf("Wrong values for k,L! GOODBYE!\n");
		return 0;
	}
	for(i=0;i<32;i++)
		modnum*=2;
	modnum=modnum-5;//kataskeui M = (2^32)-5
	pammatr=malloc(2*sizeof(double*));
	for(i=0;i<2;i++)
		pammatr[i]=malloc(2*sizeof(double));
	dilist=malloc(cent*sizeof(struct distlist));
	for(i=0;i<cent;i++)
		dilist[i].head=NULL;
	gfun=malloc(L*sizeof(int*));
	for(i=0;i<L;i++)//kataskevazoyme ti sinartisi g
	{
		gfun[i]=malloc(k*sizeof(int));
		for(j=0;j<k;j++)
		{
			random=1+ (rand() /( RAND_MAX + 1.0))*(length-1);
			gfun[i][j]=random;//epilogi,mesw kanonikis tuxaiotitas, arithmwn poy antistixoun sta ID twn h
		}
	}
	temp=inlist->head;
	while(temp!=NULL)
	{
		j=0;
		token=malloc((length+1)*sizeof(char));
		turnintobinary(temp->key,length,token);
		while(j<L)
		{
			for(i=0;i<k;i++)
			{
				token2[i]=token[gfun[j][i]];
			}
			binarynum=atoll(token2);//pairnei tin duadiki simvoloseira kai tin kanei typo long long int
			decimalnum=turnintodecimal(binarynum);//metatrepei to diadiko se dekadiko wste na paei sto swsto bucket
			insert(hasht[j][decimalnum].lista,temp->key,temp->id);
			j++;
		}
		free(token);
		temp=temp->next;
	}
	for(i=0;i<cent;i++)
	{
		temp=clist[i].centro;
		j=0;
		token=malloc((length+1)*sizeof(char));
		turnintobinary(temp->key,length,token);
		while(j<L)
		{
			for(z=0;z<k;z++)
			{
				token2[z]=token[gfun[j][z]];
			}
			binarynum=atoll(token2);//pairnei tin duadiki simvoloseira kai tin kanei typo long long int
			decimalnum=turnintodecimal(binarynum);//metatrepei to diadiko se dekadiko wste na paei sto swsto bucket
			search(hasht[j][decimalnum].lista,temp->key,temp->id,&dilist[i],length,radius,L);//lsh search
			j++;
		}
		free(token);	
	}
	erasedouble(dilist,cent);
	insertfromhash(pammatr,clist,dilist,cent,length,0);
	for(i=0;i<cent;i++)
		freedlist(&dilist[i]);
	free(dilist);
	free(pammatr[0]);
	free(pammatr[1]);
	free(pammatr);
	freegf(gfun,L);
	free(gfun);
	createnewlist(inlist,clist,cent,length,readcount);
	cleancentroids(clist,cent);
}
*/




void cleancentroids(struct clustlist * clist,int cent)
{
	int i,j,flag;
	struct centlist * tempcl;
	long temp[cent];
	for(i=0;i<cent;i++)
		temp[i]=clist[i].centro->id;
	for(i=0;i<cent;i++)
	{
		flag=0;
		tempcl=clist[i].head;
		while(tempcl!=NULL)
		{
			for(j=0;j<cent;j++)
			{
				if(tempcl->id==temp[j])
				{
					deleteclustnode(clist,i,temp[j]);
					flag=1;
					break;
				}
			}	
			if(flag==1)
				break;
			tempcl=tempcl->next;
		}

	}
	
}
void deleteclustnode(struct clustlist * clist,int pos,long fid)
{
	struct centlist* temp,*temp2;
	if(clist[pos].head->id==fid)
	{
		temp = clist[pos].head;
		clist[pos].head = clist[pos].head->next;
		free(temp);
	}
	else
	{
		temp=clist[pos].head;
		while(temp->next->id!=fid)
		{
			temp=temp->next;
		}
		temp2=temp->next;
		temp->next=temp->next->next;
		free(temp2);
	}
}



void createnewlist(struct list * inlist,struct clustlist * clist,int cent,int length,int counter)
{
	int i,flag;
	struct centlist * tempcl;
	struct list * listleft;
	struct node * tempnode,*templist;
	listleft=malloc(sizeof(struct list));
	listleft->head=NULL;
	tempnode=malloc(sizeof(struct node));
	templist=inlist->head;
	while(templist!=NULL)
	{
		flag=0;
		for(i=0;i<cent;i++)
		{
			tempcl=clist[i].head;
			while(tempcl!=NULL)	
			{
				tempnode->id=tempcl->id;
				if(tempnode->id==templist->id)
				{
					flag=1;
					break;
				}
				tempcl=tempcl->next;
			}
		}
		if(flag==0)
			insert(listleft,templist->key,templist->id);
		templist=templist->next;
	}
	medpamham(listleft,clist,length,counter,cent,0);
	free(tempnode);
	freehlist(listleft);
	free(listleft);
}





void erasedouble(struct distlist * dlist,int cent)
{
	int i,j;
	struct distnode * temp1,*temp2;
	for(i=0;i<cent;i++)
	{
		for(j=i+1;j<cent;j++)
		{		
			temp1=dlist[i].head;
			while(temp1!=NULL)
			{
				temp2=dlist[j].head;
				while(temp2!=NULL)
				{
					if(temp1->nearid==temp2->nearid)
						if(temp1->distance<temp2->distance)
							deletedistnode(&dlist[j],temp2);
						else
							deletedistnode(&dlist[i],temp1);
					temp2=temp2->next;
				}
				temp1=temp1->next;
			}
		}
	}
}


void deletedistnode(struct distlist * dlist,struct distnode * dnode)
{
	struct distnode* temp,*temp2;
	if(dnode->nearid==dlist->head->nearid)
	{
		temp = dlist->head;
		dlist->head = dlist->head->next;
		free(temp);
	}
	else
	{
		temp=dlist->head;
		while(temp->next->nearid!=dnode->nearid)
		{
			temp=temp->next;
		}
		temp2=temp->next;
		temp->next=temp->next->next;
		free(temp2);
	}
	
}



void insertfromhash(double ** pammatr,struct clustlist * clist,struct distlist * dilist,int cent,int length,int choice)
{
	struct node * insertnode;
	double distmin,dist;
	int i,j;
	struct distnode * temp2;
	insertnode=malloc(sizeof(struct node));
	for(i=0;i<cent;i++)
	{
		pammatr[0][0]=i;
		temp2=dilist[i].head;
		while(temp2!=NULL)
		{
			distmin=-1;//paradoxi
			pammatr[0][1]=temp2->distance;
			for(j=0;j<cent;j++)
			{
				if(i!=j)
				{
					insertnode->id=temp2->nearid;
					insertnode->key=temp2->nearkey;
					dist=hamdistance(clist[j].centro,length,insertnode);
					if(distmin==-1)
					{
						distmin=dist;
						pammatr[1][0]=j;
						pammatr[1][1]=distmin;
					}
					else
					{
						if(dist<distmin)
						{	
							distmin=dist;
							pammatr[1][0]=j;
							pammatr[1][1]=distmin;
						}
					}
				}
			}
			insertassig(pammatr,clist,insertnode,choice,length);
			temp2=temp2->next;
		}
	}
	free(insertnode);
}


double firstrad(struct clustlist * clist,int length,int cent,int choice)
{
	int i,j;
	double distance,distancemin=1000000;
	struct node * temp, * temp2;
	for(i=0;i<cent;i++)
	{
		for(j=i+1;j<cent;j++)
		{
			if(choice==0)
				distance=hamdistance(clist[i].centro,length,clist[j].centro);
			else if(choice==1)
				distance=cosdistance(clist[i].centro,length,clist[j].centro);
			else if(choice==2)
				distance=eucldistance(clist[i].centro,length,clist[j].centro);
			//else if(choice==3)
				//distance=matrdistance(temp,length,temp2);
			if(distance<distancemin)
				distancemin=distance;
		}
	}
	return distancemin;
}


void dilistcount(struct distlist * dlist)
{
	struct distnode * dn;
	int a=0;
	dn=dlist->head;
	while(dn!=NULL)	
	{
		a++;
		dn=dn->next;	
	}
	//printf("i lsita exei mesa %d\n",a);

}
