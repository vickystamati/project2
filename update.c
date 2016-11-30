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
#include"clara.h"
#include"lsh.h"
#define bufSize 2048
#define bafSize 8192

void lloydsupdate(struct clustlist * clist,struct list * lista,int cent,int choice,int length,int counter,double calc)//list opoiu dilist
{
	printf("mpika\n");
	struct centlist * temp, *temp2;
	struct node * upd,*cur1,*cur2;
	struct clustlist * tempclist=malloc(cent*sizeof(struct clustlist));
	int i,z,j,flag,end=0;
	double distance,distancemin;
	double delj,calsum,change;
	cur1=malloc(sizeof (struct node));
	cur2=malloc(sizeof (struct node));
	upd=malloc(sizeof (struct node));
	calsum=calc;
	printf("mpika1\n");
	for(i=0;i<cent;i++)
	{
		tempclist[i].centro=malloc(sizeof(struct centlist));
		if(choice!=0)
		{
			tempclist[i].centro->key1=malloc(length*sizeof(double));
		}
	}
	while(end==0)
	{
		printf("mpika2\n");
		for(i=0;i<cent;i++)
		{
			flag=0;
			temp=clist[i].head;
			while(temp!=NULL)
			{
					printf("mpika3\n");
				cur1->id=temp->id;
				if(choice==0)
					cur1->key=temp->key;
				else
					cur1->key1=temp->key1;
				distance=0;
				temp2=clist[i].head;
				while(temp2!=NULL)
				{
			printf("mpika4\n");
					cur2->id=temp2->id;
					if(choice==0)
						cur2->key=temp2->key;
					else
						cur2->key1=temp2->key1;
					if(temp->id!=temp2->id)
					{
						if(choice==0)
							distance+=hamdistance(cur1,length,cur2);
						else if(choice==1)
							distance+=cosdistance(cur1,length,cur2);
						else if(choice==2)
							distance+=eucldistance(cur1,length,cur2);
						else if(choice==3)
							distance+=matrdistance(lista,cur1,length,cur2);
					}
					else//an vrei ton eayto tou prosthetei tin apostasi ap to kentroides
						distance+=temp->distance;
					temp2=temp2->next;
				}
				if(flag==0)
				{
					upd->id=temp->id;
					if(choice==0)
						upd->key=temp->key;
					else
					{
						//upd->key1=malloc(length*sizeof(double));
						//for(z=0;z<length;z++)
						//upd->key1[z]=temp->key1[z];
						upd->key1=temp->key1;
					}
					distancemin=distance;
				}
				else
				{
					if(distance<distancemin)
					{
						upd->id=temp->id;
						if(choice==0)
							upd->key=temp->key;
						else
						{
							//for(z=0;z<length;z++)
							//upd->key1[z]=temp->key1[z];
							upd->key1=temp->key1;
						}
						distancemin=distance;
					}
				}
				flag++;
				temp=temp->next;
			}//telos while,exei vrei to "jalitero metoid"
					printf("mpika kalo medoid\n");
			change=approve(clist,lista,upd,cent,i,choice,length,calc);
			if(change!=-1)
			{		printf("mpika einai\n");
				tempclist[i].centro->id=upd->id;
				if(choice==0)
					tempclist[i].centro->key=upd->key;
				else
				{
					for(z=0;z<length;z++)
						tempclist[i].centro->key1[z]=upd->key1[z];
				}
			}
			else
			{
						printf("mpika not\n");
				tempclist[i].centro->id=clist[i].centro->id;
				if(choice==0)
					tempclist[i].centro->key=clist[i].centro->key;
				else
				{
					for(z=0;z<length;z++)
						tempclist[i].centro->key1[z]=clist[i].centro->key1[z];
				}

			}
		}
		end=0;
		for(i=0;i<cent;i++)
		{
			if(clist[i].centro->id==tempclist[i].centro->id)
				end++;
		}
		if(end==cent)
		{
			break;
		}	
		printf("mpika paw gia pamham\n");
		for(i=0;i<cent;i++)
		{
			printf("to %d mallon %lu\n",i,tempclist[i].centro->id);
			if(tempclist[i].head==NULL)
				printf("adeua listaa reeeeee\n");
		}
		if(choice==0)
			medpamham(lista,tempclist,length,counter,cent,choice);
		else if(choice==1)
			medpamcos(lista,tempclist,length,counter,cent,choice);
		else if(choice==2)
			medpameucl(lista,tempclist,length,counter,cent,choice);
		else if(choice==3)
			medpammatr(lista,tempclist,length,counter,cent,choice);
		change=calcj(tempclist,cent);
				printf("mpik pamhamtelos\n");
		if(change<calc)
		{
			freeclustlist(clist,cent,choice);
			swapclist(clist,tempclist,cent,length,choice);
			freeclustlist(tempclist,cent,choice);
			calc=change;
		}
		freeclustlist(tempclist,cent,choice);
	}
	free(cur1);
	free(cur2);
	free(upd);
	for(i=0;i<cent;i++)
	{
		if(choice!=0)
			free(tempclist[i].centro->key1);
		free(tempclist[i].centro);
	}
	free(tempclist);
}

void clarans(struct clustlist * clist,struct list * lista,int cent,int choice,int length,int counter,int * combmatr,int q)
{
	struct centlist * temp, *temp2;
	struct node * upd,*cur1,*cur2;
	struct clustlist * clistfinal, *clisttemp;
	long idtable[cent];
	int i,z,j,flag,pos1,pos2;
	double delj,calsum,change;
	cur1=malloc(sizeof (struct node));
	delj=calcj(clist,cent);
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
	i=0;
	while(i<q)
	{
		pos1=modint(combmatr[i],cent);
		pos2=combmatr[i]/cent;	
		cur1=lista->head;
		for(j=0;j<pos2;j++)
			cur1=cur1->next;
		for(j=0;j<cent;j++)
		{
			if(pos1!=j)
			{
				clisttemp[j].centro->id=clist[j].centro->id;
				if(choice==0)
					clisttemp[j].centro->key=clist[j].centro->key;
				else
					for(z=0;z<length;z++)
						clisttemp[j].centro->key1[z]=clist[j].centro->key1[z];
			}
			else
			{
				clisttemp[j].centro->id=cur1->id;
				if(choice==0)
					clisttemp[j].centro->key=cur1->key;
				else
					for(z=0;z<length;z++)
						clisttemp[j].centro->key1[z]=cur1->key1[z];
			}
		}
		if(choice==0)
			medpamham(lista,clisttemp,length,counter,cent,choice);
		else if(choice==1)
			medpamcos(lista,clisttemp,length,counter,cent,choice);
		else if(choice==2)
			medpameucl(lista,clisttemp,length,counter,cent,choice);
		else if(choice==3)
			medpammatr(lista,clisttemp,length,counter,cent,choice);
		calsum=calcj(clisttemp,cent);
		if(calsum<delj)
		{
			freeclustlist(clistfinal,cent,choice);
			swapclist(clistfinal,clisttemp,cent,length,choice);
			freeclustlist(clisttemp,cent,choice);
			delj=calsum;
		}
		else
			freeclustlist(clisttemp,cent,choice);
		i++;
	}
	if(clistfinal[0].head!=NULL)
	{
		freeclustlist(clist,cent,choice);
		swapclist(clist,clistfinal,cent,length,choice);

	}

}


void claransupdate(struct clustlist * clist,struct list * lista,int cent,int choice,int length,int counter)
{
	int i,j,q,try,x,flag,s;
	int *combmatr;
	s=2;
	for(i=0;i<2;i++)
	{
		try=0.12 * cent *(counter-cent);
		if(try>=250)
			q=try;
		else
			q=250;
		combmatr=malloc(q*sizeof(int));
		i=0;
		while(i<q)
		{
			flag=0;
			x=1+ (rand() /( RAND_MAX + 1.0))*((cent*counter-1)-1);
			if(i==0)
			{
				combmatr[i]=x;
				i++;
			}
			else
			{			
				for(j=0;j<i;j++)
					if(combmatr[j]==x)
						flag=1;
				if(flag==0)
				{
					combmatr[i]=x;
	 				i++;
				}
			}
		}
		clarans(clist,lista,cent,choice,length, counter,combmatr,q);	
	}
}

void claransloop(struct clustlist * clist,struct list * inlist,int cent,int choice,int length,int readcount,int *dec,double ** indistmatr,int k,int L)
{
	int i,j;
	struct node * centroids=malloc(cent*sizeof(struct node));
	if(dec[0]==0)
	{	
		if(choice==0)
			medoidsham(inlist,length,readcount,centroids,cent);
		else if(choice==1)
			medoidscos(inlist,length,readcount,centroids,cent);
		else if(choice==2)
			medoidseucl(inlist,length,readcount,centroids,cent);
		else if(choice==3)
			medoidsmatr(inlist,readcount,centroids,cent);
	}
	else if(dec[0]==1)
	{
		if(choice==0)
			concentrateham(inlist,indistmatr,length,readcount,centroids,cent);
		else if(choice==1)
			concentratecos(inlist,indistmatr,length,readcount,centroids,cent);
		else if(choice==2)
			concentrateeucl(inlist,indistmatr,length,readcount,centroids,cent);
		else if(choice==3)
			concentratematr(inlist,indistmatr,readcount,centroids,cent);
	}
	for(i=0;i<cent;i++)//initialise clist with cetroids
	{
		clist[i].centro->id=centroids[i].id;
		if(choice==0)
			clist[i].centro->key=centroids[i].key;
		else
		{
			for(j=0;j<length;j++)
				clist[i].centro->key1[j]=centroids[i].key1[j];
		}
		clist[i].head=NULL;
	}
	free(centroids);
	if(dec[1]==0)
	{
		if(dec[0]==1)
			pamassig(inlist,clist,indistmatr,length,readcount,cent,choice);
		else if(dec[0]==0)
		{
			if(choice==0)
				medpamham(inlist,clist,length,readcount,cent,choice);
			else if(choice==1)
				medpamcos(inlist,clist,length,readcount,cent,choice);
			else if(choice==2)
				medpameucl(inlist,clist,length,readcount,cent,choice);
			else if(choice==3)
				medpammatr(inlist,clist,length,readcount,cent,choice);
		}
	}
	else if(dec[1]==1)
	{
		lshassign(inlist,clist,length,readcount,cent,choice,k,L);
	}
}



void swapclist(struct clustlist * clist,struct clustlist * tempclist,int cent,int length,int choice)
{
	int i,j;
	struct centlist * temp, *temp2;
	for(i=0;i<cent;i++)
	{
		clist[i].centro->id=tempclist[i].centro->id;
		if(choice==0)
			clist[i].centro->key=tempclist[i].centro->key;
		else
		{
			if(clist[i].centro->key1==NULL)
				clist[i].centro->key1=malloc(length*sizeof(double));
			for(j=0;j<length;j++)
				clist[i].centro->key1[j]=tempclist[i].centro->key1[j];
		}
		temp=tempclist[i].head;
		while(temp!=NULL)
		{
			if(clist[i].head==NULL)
			{
				clist[i].head=malloc(sizeof(struct centlist));
				clist[i].head->id=temp->id;
				if(choice==0)
					clist[i].head->key=temp->key;
				else
				{
					clist[i].head->key1=malloc(length*sizeof(double));
					for(j=0;j<length;j++)
						clist[i].head->key1[j]=temp->key1[j];
				}
				clist[i].head->distance=temp->distance;
				clist[i].head->listnearid=temp->listnearid;
				clist[i].head->distancenear=temp->distancenear;
				clist[i].head->next=NULL;
			}
			else
			{
				temp2=clist[i].head;
				while(temp2->next!=NULL)
				{
					temp2=temp2->next;
				}
				temp2->next=malloc(sizeof(struct centlist));
				temp2->next->id=temp->id;
				if(choice==0)
					temp2->next->key=temp->key;
				else
				{
					temp2->next->key1=malloc(length*sizeof(double));
					for(j=0;j<length;j++)
						temp2->next->key1[j]=temp->key1[j];
				}
				temp2->next->distance=temp->distance;
				temp2->next->listnearid=temp->listnearid;
				temp2->next->distancenear=temp->distancenear;
				temp2->next->next=NULL;
			}			
			temp=temp->next;
		}
	}
}




void freeclustlist(struct clustlist * clist,int cent,int choice)
{
	struct centlist * temp;
	int i;
	for(i=0;i<cent;i++)
	{
		temp=clist[i].head;	
		while (clist[i].head != NULL)
		{
			temp=clist[i].head;	
			clist[i].head=clist[i].head->next;
			if(choice!=0)			
				free(temp->key1);
			free(temp);
		}
		clist[i].head=NULL;
	}
}

double approve(struct clustlist * clist,struct list * lista,struct node * upd,int cent,int place ,int choice,int length ,double calc)
{
	double tempj=0,distance;
	int i,z;
	struct centlist * temp;
	struct node * tempnode;
	tempnode=malloc(sizeof(struct node));

	for(i=0;i<cent;i++)
	{
		temp=clist[i].head;
		while(temp!=NULL)
		{
			if(temp->id==upd->id)//an vrei to idio idio id ksaanevainei stin while
			{
				temp=temp->next;
				continue;
			}
			tempnode->id=temp->id;
			if(choice==0)
			{
				tempnode->key=temp->key;
				distance=hamdistance(tempnode,length,upd);
			}
			else if(choice==1)
			{
				tempnode->key1=temp->key1;		
				distance=cosdistance(tempnode,length,upd);
			}
			else if(choice==2)
			{
				tempnode->key1=temp->key1;		
				distance=eucldistance(tempnode,length,upd);
			}
			else if(choice==3)
			{
				tempnode->key1=temp->key1;		
				distance=matrdistance(lista,tempnode,length,upd);
			}
			if(i==place)//ama einai sti lista tin opoia anikei
			{
				if(distance<=temp->distancenear)
					tempj+=distance - temp->distance;
				else
					tempj+=temp->distancenear - temp->distance;
			}	
			else
			{
				if(distance>=temp->distance)
					tempj+=0;
				else
					tempj+=distance - temp->distance;
					
			}
			temp=temp->next;
		}

	}
	free(tempnode);
	tempj+=calc;
	if(tempj>0 && tempj<calc)
		return tempj;
	else
		return -1;
}

void silhouette(struct clustlist * clist,struct list * lista,int cent,int choice,int length)
{
	int i;
	double suma,sumb,sumaver,sumall=0,counta,countb,countaver;
	struct centlist * temp;
	struct centlist *temp2;
	struct node * com=malloc(sizeof(struct node));
	struct node *com2=malloc(sizeof(struct node));
	int flag;
	printf("Silhuette per cluster {");
	for(i=0;i<cent;i++)
	{
		temp=clist[i].head;
		sumaver=0;
		countaver=0;
		while(temp!=NULL)
		{
			flag=0;
			counta=0;
			suma=0;
			com->id=temp->id;
			if(choice==0)
				com->key=temp->key;
			else
				com->key1=temp->key1;
			temp2=clist[i].head;
			while(temp2!=NULL)
			{
				if(temp2->id!=temp->id)
				{
					com2->id=temp2->id;	
					if(choice==0)
					{
						com2->key=temp2->key;
						suma+=hamdistance(com,length,com2);
					}
					else if(choice==1)
					{
						com2->key1=temp2->key1;
						suma+=cosdistance(com,length,com2);
					}
					else if(choice==2)
					{
						com2->key1=temp2->key1;
						suma+=eucldistance(com,length,com2);
					}
					else if(choice==3)
					{
						com2->key1=temp2->key1;
						suma+=matrdistance(lista,com,length,com2);
					}
					counta++;
				}
				temp2=temp2->next;
			}
			countb=0;
			sumb=0;
			temp2=clist[temp->listnearid].head;
			while(temp2!=NULL)
			{
				com2->id=temp2->id;	
				if(choice==0)
				{
					com2->key=temp2->key;
					sumb+=hamdistance(com,length,com2);
				}
				else if(choice==1)
				{
					com2->key1=temp2->key1;
					sumb+=cosdistance(com,length,com2);
				}
				else if(choice==2)
				{
					com2->key1=temp2->key1;
					sumb+=eucldistance(com,length,com2);
				}
				else if(choice==3)
				{
					com2->key1=temp2->key1;
					sumb+=matrdistance(lista,com,length,com2);
				}
				countb++;
				temp2=temp2->next;
			}
			if(counta!=0 && countb!=0)
			{
				suma=suma/counta;
				sumb=sumb/countb;
			}
			else
				flag=1;
			if(suma>sumb && suma!=0)
				sumaver+=(sumb/suma)-1;
			else if(suma<sumb && sumb!=0)
				sumaver+=1-(suma/sumb);
			else if(suma==sumb)
				sumaver+=0;
			else
				flag=1;
			temp=temp->next;
			if(flag==0)
				countaver++;
		}
		if(countaver!=0)
			sumaver=sumaver/countaver;
		else
			sumaver=0;
		if(i==cent-1)
			printf("%f}\n",sumaver);
		else
			printf("%f, ",sumaver);
		sumall+=sumaver;
	}
	printf("Sum : %f\n",sumall);
	free(com);
	free(com2);

}

int modint (int a, int b)
{
	int ret;
	if(b < 0) //you can check for b == 0 separately and do what you want
		return mod(a, -b);   
	ret = a % b;
	if(ret < 0)
		ret+=b;
	return ret;
}








