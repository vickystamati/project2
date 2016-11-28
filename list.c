#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"hash.h"
#include"list.h"
#include"initialise.h"
#include "assignment.h"
#include"update.h"
#include"clara.h"

void search(struct list *lista,long long find,long idfind,struct distlist * dilist,int length,double radius,int L)
{
	struct distnode * new;
	char * chbin,* incur;//binary ws simvoloseira
	chbin=malloc((length+1)*sizeof(char));//desmevei oso to legethos ton stoixeion tou arxeiou
	turnintobinary(find,length,chbin);//metatropi long se binary
	int i,loop=0;
	double d;
	struct node *current = lista->head;
	if(radius==0)//an i aktina einai 0
	{
		new = malloc(sizeof(struct distnode));
		new->distance=length+1;//paradoxi, max+1 diaforetika
		new->next=NULL;
		while (current!= NULL && loop!=3*L) //mexri 3L epanalipseis
		{
			if(current->id==idfind) //an einai o eaytos tou
			{
				current = current->next;
				continue;
			}
			incur=malloc(length+1);
			turnintobinary(current->key,length,incur);
			d=0;
			for(i=0;i<length;i++)//elegxei ta diaforetika stoixeia,diladi tin apostasi
			{
				if(incur[i]!=chbin[i])
				{
					d++;
				}
			}
			if(d<new->distance)//einai i mikroteri ekeini ti stigmi
			{
				new->nearid=current->id;
				new->nearkey=current->key;
				new->distance=d;
			}
			current = current->next;
			free(incur);
		}
		
		if(new->distance!=length+1)//an exei allaksei to new->distance to pernaei sti lista
			insertnear(dilist,new);
		else
			free(new);
	}
	else
	{
		while (current!= NULL && loop!=3*L)
		{
			if(current->id==idfind) 
			{
				current = current->next;
				continue;
			}
			incur=malloc(length+1);
			turnintobinary(current->key,length,incur);
			d=0;
			for(i=0;i<length;i++)
			{
				if(incur[i]!=chbin[i])
				{
					d++;
				}
			}
			if(d<=radius)//an anikei stin aktina kanto eisagwgi sti lista
			{
				new = malloc(sizeof(struct distnode));
				new->next=NULL;
				new->nearid=current->id;
				new->nearkey=current->key;
				new->distance=d;
				insertnear(dilist,new);
			}
			current = current->next;
			free(incur);
		}		
	}	
	free(chbin);
}

void findmin(struct distlist * dilist,int flag,FILE* fpw)
{
	struct distnode *new=malloc(sizeof(struct distnode));
	struct distnode *current = dilist->head;
	int min;
	if(current!=NULL)
	{
		min=current->distance+1;
		while (current!= NULL)
		{
			if(min>current->distance)//einai i mikroteri ekeini ti stigmi
			{
				new->nearid=current->nearid;
				new->nearkey=current->nearkey;
				new->distance=current->distance;
				min=current->distance;
			}
			current = current->next;
		}
		if(flag==0)//an flag einai 0 tote exoume lsh
		{
			fprintf(fpw, "Nearest neighbor: item%lu\n",new->nearid);
			fprintf(fpw, "distanceLSH: %f\n",new->distance);
		}
		else//alliws brute
			fprintf(fpw, "distanceTrue: %f\n",new->distance);
	}
	else
		fprintf(fpw, "Empty list at findmin\n");
	free(new);
}

void printdistancelist(struct distlist * lista,int length,FILE * fpw)
{
	struct distnode* temp;
	char *token;
	temp = lista->head;
	if(temp==NULL)
	{
		fprintf(fpw, "Empty List\n");
	}
	else
	{	
		fprintf(fpw, "R-near neighbors: \n");
		while(temp!=NULL)//trexei oli ti lista
		{
			
			token=malloc(length+1);
			turnintobinary(temp->nearkey,length,token);
			fprintf(fpw, "item%lu\n",temp->nearid);//emfanizei 
			free(token);
			temp = temp->next;
		}
	}
}



void insertnear(struct distlist * dlist,struct distnode * new)
{
	int flag=0;
	if (dlist->head == NULL)//an head null kane eisagwgi
	{
		dlist->head=new;
		dlist->head->next=NULL;	

	}
	else
	{
		struct distnode *current = dlist->head;
		if(new->nearid==current->nearid)
		{
			flag=1;
		}
		while (current->next != NULL) 
		{
			if(new->nearid==current->next->nearid)//elegxei ti monadikotita
			{
				flag=1;
				break;
			}
			current = current->next;
		}
		if(flag==0)//an den exei allaksei to flag kano eisagwgi
		{
			current->next = new;
			current->next->next=NULL;

		}	
	}
}

void insert(struct list * lista,long long binary,long itemid)
{
	struct node * new;
	new = malloc(sizeof(struct node));
	if(new == NULL)
	{
		printf("Failed to allocate memory!\n");
	}
	new -> id=itemid;
	new -> key =binary;
	new -> next = NULL;
	if (lista->head == NULL)
	{
		lista->head=new;
		lista->head->next=NULL;	
	}
	else
	{
		struct node *current = lista->head;
		while (current->next != NULL) 
		{
			current = current->next;
		}
		current->next = new;
	}	
}

void turnintobinary(long long dn,int length,char* token)
{
	int i,k,count=0;
	char binarray[length+1];
	for (i = length-1; i >= 0; i--)
	{
		k = dn >> i;
 
		if (k & 1)
			binarray[count]='1';
		else
			binarray[count]='0';
		count++;
	}
	binarray[length]='\0';
	strcpy(token,binarray);
}

void insertcosine(struct list *lista,double * vector,long itemid,int counter)
{
	int i;
	struct node * new;
	new = malloc(sizeof(struct node));
	if(new == NULL)
	{
		printf("Failed to allocate memory!\n");
	}
	new -> id=itemid;
	new -> key1 =malloc(counter*sizeof(double));
	for(i=0;i<counter;i++)
	{	
		new->key1[i]=vector[i];//vazei sto key1 ta stoixeia tou vector
	}
	new -> next = NULL;
	if (lista->head == NULL)//an to head einai null kanei eisagwgi
	{
		lista->head=new;
		lista->head->next=NULL;	
	}
	else
	{
		struct node *current = lista->head;
		while (current->next != NULL) //allwis vriskei ton adeio komvo
		{
			current = current->next;
		}
		current->next = new;
	}	
}

void searchcosine(struct list *lista,double* find,long idfind,struct distlist * dilist,int counter,double radius,int L)
{
	struct distnode * new;
	int i;
	double d,sum,sum1,sum2;
	int loop=0;
	struct node *current = lista->head;
	if(radius==0)
	{
		new = malloc(sizeof(struct distnode));
		new->distance=counter+1;//paradoxi, max+1 diaforetika
		new->next=NULL;
		while (current!= NULL && loop!=3*L)
		{
			if(current->id==idfind) 
			{
				current = current->next;
				continue;
			}
			sum=0,sum1=0,sum2=0;
			for(i=0;i<counter;i++)//upologismos typou cosine
			{
				sum+=current->key1[i] * find[i];
				sum1+=current->key1[i] * current->key1[i];
				sum2+=find[i] * find[i];
			}
			loop++;
			if(sum1!=0 && sum2!=0)//epeidi einai riza, elegxos na einai diaforetiko ap to 0
			{
				d=1-(sum/sqrt(sum1 * sum2));
				if(d<new->distance)//einai i mikroteri ekeini ti stigmi
				{
					new->nearid=current->id;
					new->distance=d;
				}
			}
			current = current->next;
		}
		if(new->distance!=counter+1)//an exei allaksei to min kanei eisagwgi
			insertnear(dilist,new);
		else
			free(new);
	}
	else//omoia an radius!=0
	{
		while (current!= NULL && loop!=3*L) //isws thelei current->next
		{
			if(current->id==idfind) 
			{
				current = current->next;
				continue;
			}
			sum=0,sum1=0,sum2=0;
			for(i=0;i<counter;i++)
			{
				sum+=current->key1[i] * find[i];
				sum1+=current->key1[i] * current->key1[i];
				sum2+=find[i] * find[i];
			}
			loop++;
			if(sum1!=0 && sum2!=0)
			{
				d=1-(sum/(sqrt(sum1) * sqrt(sum2)));
				if(d<=radius)//einai i mikroteri ekeini ti stigmi
				{
					new = malloc(sizeof(struct distnode));
					new->next=NULL;
					new->nearid=current->id;
					new->distance=d;
					insertnear(dilist,new);
				}
			}
			current = current->next;
		}		
	}
}
void printdistancelistcosine(struct distlist * lista, FILE * fpw)
{
	struct distnode* temp;
	temp = lista->head;
	if(temp==NULL)
	{
		fprintf(fpw, "Empty List\n");
	}
	else
	{
		fprintf(fpw, "R-near neighbors: \n");
		while(temp!=NULL)
		{
			fprintf(fpw, "item%lu\n",temp->nearid);
			temp = temp->next;
		}
	}
}

void inserteuclidian(struct list * lista,double *vector, long itemid,long id,int counter)
{
	int i;
	struct node * new;
	new = malloc(sizeof(struct node));
	if(new == NULL)
	{
		printf("Failed to allocate memory!\n");
	}
	new -> id=itemid;
	new -> key1 =malloc(counter*sizeof(double));
	new -> findid=id;
	for(i=0;i<counter;i++)
	{	
		new->key1[i]=vector[i];
	}
	new -> next = NULL;
	if (lista->head == NULL)
	{
		lista->head=new;
		lista->head->next=NULL;	
	}
	else
	{
		struct node *current = lista->head;
		while (current->next != NULL) 
		{
			current = current->next;
		}
		current->next = new;
	}	
}

void searcheuclidian(struct list *lista,double* find,long id,long idfind,struct distlist * dilist,int counter,double radius,int L)
{
	struct distnode * new;
	int i;
	double d,sum,sum2;
	int loop=0;
	struct node *current = lista->head;
	if(current->findid==idfind || L<0)
	{
		if(radius==0)
		{	
			new = malloc(sizeof(struct distnode));
			new->distance=10000000000;//paradoxi
			new->next=NULL;
			while (current!= NULL && loop!=3*L)
			{
				if(current->id==id) 
				{
					current = current->next;
					continue;
				}
				sum=0;
				sum2=0;
				for(i=0;i<counter;i++)//typos euclidean
				{
					sum=current->key1[i] - find[i];
					sum=sum*sum;
					sum2+=sum;
					sum=0;
				}
				d=sqrt(sum2);//upologismos apostasis
				loop++;
				if(d<new->distance)//einai i mikroteri ekeini ti stigmi
				{
					new->nearid=current->id;
					new->distance=d;
				}
				current = current->next;
			}
			if(new->distance!=10000000000)
				insertnear(dilist,new);
			else
				free(new);
		}
		else//omoia an yparxei radius
		{
			while (current!= NULL && loop!=3*L)
			{
				if(current->id==id) 
				{
					current = current->next;
					continue;
				}
				sum=0;
				sum2=0;
				for(i=0;i<counter;i++)
				{
					sum+=current->key1[i] - find[i];
					sum=sum*sum;
					sum2+=sum;
					sum=0;
				}
				d=sqrt(sum2);
				loop++;
				if(d<=radius)//einai i mikroteri ekeini ti stigmi
				{
					new = malloc(sizeof(struct distnode));
					new->next=NULL;
					new->nearid=current->id;
					new->distance=d;
					insertnear(dilist,new);
				}
				current = current->next;
			}		
		}
	}
}
void printdistancelisteuclidian(struct distlist * lista,FILE * fpw)
{
	struct distnode* temp;
	temp = lista->head;
	if(temp==NULL)
	{
		fprintf(fpw, "Empty List\n");
	}
	else
	{
		fprintf(fpw, "R-near neighbors: \n");
		while(temp!=NULL)
		{
			fprintf(fpw, "item%lu\n",temp->nearid);
			temp = temp->next;
		}
	}
}
void searchmatrix(struct list*lista,double ** find,long itemid,struct  distlist * dilist, int counter,double radius,int L)
{
	struct distnode * new;
	double d;
	int loop=0;
	struct node *current = lista->head;
	if(radius==0)
	{
		new = malloc(sizeof(struct distnode));
		new->distance=counter+1;//paradoxi, max+1 diaforetika
		new->next=NULL;
		while (current!= NULL && loop!=3*L)
		{
			if(current->id==itemid) 
			{
				current = current->next;
				continue;
			}
			loop++;
			d=find[itemid-1][current->id-1];//paei sthn katallhlh thesi tou matrix kai ypologizei tin apostasi
			if(d<new->distance && d!=0)//einai i mikroteri ekeini ti stigmi
			{
				new->nearid=current->id;
				new->distance=d;
			}
			current = current->next;
		}
		if(new->distance!=counter+1)
			insertnear(dilist,new);
		else 
			free(new);
	}
	else
	{
		while (current!= NULL && loop!=3*L)
		{
			if(current->id==itemid) 
			{
				current = current->next;
				continue;
			}
			loop++;
			d=find[itemid-1][current->id-1];
			if(d<=radius && d!=0)//einai i mikroteri ekeini ti stigmi
			{
				new = malloc(sizeof(struct distnode));
				new->next=NULL;
				new->nearid=current->id;
				new->distance=d;
				insertnear(dilist,new);
			}
			current = current->next;
		}		
	}
}
void freehlist(struct list* lista)//kanei free ti lista twn backet apo to hashtable
{
	struct node* temp;
	while (lista->head != NULL)
	{
		temp = lista->head;
		lista->head = lista->head->next;
		if(temp->key1!=NULL)
			free(temp->key1);
		free(temp);
	}
}
void freedlist(struct distlist* lista)//kanei free ti lista ton apostasewn
{
	struct distnode* temp;
	while (lista->head != NULL)
	{
		temp = lista->head;
		lista->head = lista->head->next;
		free(temp);
	}
}


long long turnintodecimal(long long bn)
{
	long long binaryNumber,decimalNumber=0,j=1,remainder;
	binaryNumber=bn;
	while(binaryNumber!=0)
	{
		remainder=binaryNumber%10;
		decimalNumber=decimalNumber+remainder*j;
		j=j*2;
		binaryNumber=binaryNumber/10;
    }
	return decimalNumber;
}

void bubble_sort(double* matr, int counter)
{
	double t;
	int i,j;
	for (i=0;i<(counter-1);i++)
	{
		for (j=0;j<counter-i-1;j++)
		{
			if (matr[j] > matr[j+1])
			{
				t = matr[j];
				matr[j] = matr[j+1];
				matr[j+1] = t;
			}
		}
	}
}

void bubble_sort2d(double** matr, int counter)
{
	double t1,t2;
	int i,j;
	for (i=0;i<(counter-1);i++)
	{
		for (j=0;j<counter-i-1;j++)
		{
			if (matr[j][1] > matr[j+1][1])
			{
				t1 = matr[j][1];
				t2= matr[j][0];
				matr[j][1] = matr[j+1][1];
				matr[j][0] = matr[j+1][0];
				matr[j+1][0] = t2;
				matr[j+1][1] = t1;
			}
		}
	}
}

long int mod (long int a, long int b)
{
	if(b < 0) //you can check for b == 0 separately and do what you want
		return mod(a, -b);   
	long int ret = a % b;
	if(ret < 0)
		ret+=b;
	return ret;
}



