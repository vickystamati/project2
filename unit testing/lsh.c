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
#include"execute.h"
#define bufSize 2048
#define bafSize 8192

void lshhaminit(struct hashtable ** hasht,struct clustlist * clist,struct list * inlist,int L,int k,int length,int readcount,int cent)
{
	struct node * temp;
	char * token,token2[k];
	int ** gfun;
	int random,i,j,z,loop,tloop;
	double ** pammatr,radius;
	struct distlist * dilist;
	long long binarynum,decimalnum;
	radius=firstrad(clist,length,cent,0,inlist);
	//printf("radius %f\n",radius);
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
				tloop=search(hasht[j][decimalnum].lista,temp->key,temp->id,&dilist[i],length,radius,L);//lsh search
				if(tloop!=0)
					loop++;
				j++;
			}
			free(token);	
		}
		radius*=2;
	}
	erasedouble(dilist,cent,0);
	insertfromhash(pammatr,clist,dilist,cent,length,0,inlist);
	createnewlist(inlist,clist,cent,length,readcount,0);
	cleancentroids(clist,cent,0);
	for(i=0;i<cent;i++)
		freedlist(&dilist[i]);
	free(dilist);
	free(pammatr[0]);
	free(pammatr[1]);
	free(pammatr);
	freegf(gfun,L);
	free(gfun);

}


void lshcosinit(struct hashtable ** hasht,struct clustlist * clist,struct list * inlist,int L,int k,int length,int readcount,int cent)
{
	int i,j,z,t,**gfun,random,sum,loop,tloop,x=4;
	double ** hfix,ran,y1,y2,***htable,radius,**pammatr;
	char token[k];
	long long decimalnum,binarynum;
	struct node* temp;
	struct distlist * dilist;
	radius=firstrad(clist,length,cent,1,inlist);
	pammatr=malloc(2*sizeof(double*));
	for(i=0;i<2;i++)
		pammatr[i]=malloc(2*sizeof(double));
	dilist=malloc(cent*sizeof(struct distlist));
	for(i=0;i<cent;i++)
		dilist[i].head=NULL;
	hfix=malloc((k*L)*sizeof(double*));//vazw ta k*L h
	for(i=0;i<k*L;i++)
		hfix[i]=malloc(length*sizeof(double));
	for(i=0;i<(k*L);i++)
	{
		for(j=0;j<length;j++)
		{
			ran=2;
			while(ran>=1)//ftiaxnw y1<1 kai y2<1, kai ftiaxnw to rand<1 (typos marsaglia)
			{
				y1=-1+(rand()/(RAND_MAX +1.0))*(1+1);	
				y2=-1+(rand()/(RAND_MAX +1.0))*(1+1);	
				ran=(y1*y1) + (y2*y2);	
			}
			hfix[i][j]=sqrt((-2*log10(ran))/(ran)) * y1;//ftiaxno ta h
		}
	}
	gfun=malloc(L*sizeof(int*));	
	for(i=0;i<L;i++)//kataskevazoyme ti sinartisi g pou periexei ton arithmo tou h poy tha mpei se kathe g(px h1 h2 h3 h5)
	{
		gfun[i]=malloc(k*sizeof(int));
		for(j=0;j<k;j++)
		{
			random=1+ (rand() /( RAND_MAX + 1.0))*(L*k-1);
			gfun[i][j]=random;
		}
	
	}
	htable=malloc(L *sizeof(double**));//triplo asteraki,ftiaxnei tin teliki morfi tis g(me ta h pou perixei gia kathe hashtable)
	for(i=0;i<L;i++)
	{
		htable[i]=malloc(k*sizeof(double*));
		for(j=0;j<k;j++)
		{
			htable[i][j]=malloc(length*sizeof(double));
		}
	}
	for(i=0;i<L;i++)
	{
		for(j=0;j<k;j++)
		{			
			htable[i][j]=hfix[gfun[i][j]];

		}
	}
	temp=inlist->head;
	while(temp!=NULL)
	{
		sum=0;
		for(i=0;i<L;i++)
		{
			for(j=0;j<k;j++)
			{
				for(z=0;z<length;z++)
				{
					sum+=htable[i][j][z] * temp->key1[z];
				}
				if(sum>=0)
					token[j]='1';
				else 
					token[j]='0';
				sum=0;
			}
			binarynum=atoll(token);
			decimalnum=turnintodecimal(binarynum);//thesi sto hastable pou tha mpei
			insertcosine(hasht[i][decimalnum].lista,temp->key1,temp->id,length);//insert stin cosine
		}
		temp=temp->next;
	}
	loop=0;
	while(loop!=x)
	{
		for(t=0;t<cent;t++)
		{
			sum=0;
			for(i=0;i<L;i++)
			{
				for(j=0;j<k;j++)
				{
					for(z=0;z<length;z++)
					{
						sum+=htable[i][j][z] * clist[t].centro->key1[z];
					}
					if(sum>=0)
						token[j]='1';
					else 
						token[j]='0';
					sum=0;
				}
				binarynum=atoll(token);
				decimalnum=turnintodecimal(binarynum);//thesi sto hastable pou tha mpei
				tloop=searchcosine(hasht[i][decimalnum].lista,clist[t].centro->key1,clist[t].centro->id,&dilist[t],length,radius,L);//search lsh
			}
		}
		loop++;	
		radius=2*radius;
	}
	erasedouble(dilist,cent,1);
	insertfromhash(pammatr,clist,dilist,cent,length,1,inlist);
	createnewlist(inlist,clist,cent,length,readcount,1);
	cleancentroids(clist,cent,1);
	for(i=0;i<cent;i++)
		freedlist(&dilist[i]);
	free(dilist);
	free(pammatr[0]);
	free(pammatr[1]);
	free(pammatr);
	freegf(gfun,L);
	free(gfun);
	freefix(hfix,k,L);
	free(hfix);
	freehtable(htable,L,k);
	free(htable);
}
				


void lsheuclinit(struct hashtable ** hasht,struct clustlist * clist,struct list * inlist,int L,int k,int length,int readcount,int cent)
{
	struct node * temp;
	int ** gfun,*rfix;
	int random,i,j,t,z,sum,fsum,loop,tloop;
	double ** pammatr,**vfix,*tfix,ran,y1,y2,w=4,radius;
	struct distlist * dilist;
	long modnum=1,idfind,hashsize;
	radius=firstrad(clist,length,cent,2,inlist);
	hashsize=readcount/16;
	if(k>10 && L>30)//elegxos gia k kai L
	{
		printf("Wrong values for k,L! GOODBYE!\n");
		return;
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
	for(i=0;i<L;i++)//kataskevazoyme ti sinartisi g pou periexei ton arithmo tou h poy tha mpei se kathe g(px h1 h2 h3 h5)
	{
		gfun[i]=malloc(k*sizeof(int));
		for(j=0;j<k;j++)
		{
			random=(rand() /( RAND_MAX + 1.0))*(L*k);
			gfun[i][j]=random;
		}
	
	}
	vfix=malloc(k*L*sizeof(double*));
	tfix=malloc(k*L*sizeof(double));
	for(i=0;i<k*L;i++)
	{		
		vfix[i]=malloc(length*sizeof(double));
	}
	for(i=0;i<k*L;i++)
	{
		tfix[i]=(rand() /( RAND_MAX + 1.0))*w;	//Kataskevazoume to t toy kathe h
		for(j=0;j<length;j++)
		{
			ran=2;
			while(ran>=1)
			{
				y1=-1+(rand()/(RAND_MAX +1.0))*(1+1);	
				y2=-1+(rand()/(RAND_MAX +1.0))*(1+1);	
				ran=(y1*y1) + (y2*y2);	
			}
			vfix[i][j]=sqrt((-2*log10(ran))/(ran)) * y1;//to v tou kathe h
		}
	}
	rfix=malloc(k*sizeof(int));
	for(i=0;i<k;i++)
	{
		rfix[i]=1+(rand() /( RAND_MAX + 1.0))*128;//kataskevi r(i) gia ton upologismo tis Φ
	}
	temp=inlist->head;
	while(temp!=NULL)
	{
		for(i=0;i<L;i++)
		{
			fsum=0;
			for(j=0;j<k;j++)
			{
				sum=0;
				for(z=0;z<length;z++)
				{
			
					sum=temp->key1[z] * vfix[gfun[i][j]][z];
				}
				sum=(sum+tfix[gfun[i][j]])/w;
				sum=floor(sum);//ypologismos h
				fsum+=rfix[j]*sum;//athroisma twn h me ta katallila r(i) gia tin kataskevi tis Φ
			}
			idfind=mod(fsum,modnum);//to id poy prokiptei ap to mod()
			inserteuclidian(hasht[i][mod(idfind,hashsize)].lista,temp->key1,temp->id,idfind,length);
		}
		temp=temp->next;
	}
	loop=0;//paradoxi gia na mpei
	while(loop!=w)
	{
		for(t=0;t<cent;t++)
		{
			for(i=0;i<L;i++)
			{
				fsum=0;
				for(j=0;j<k;j++)
				{
					sum=0;
					for(z=0;z<length;z++)
					{

						sum=clist[t].centro->key1[z] * vfix[gfun[i][j]][z];
					}

					sum=(sum+tfix[gfun[i][j]])/w;
					sum=floor(sum);
					fsum+=rfix[j]*sum;
				}
				idfind=mod(fsum,modnum);
				tloop=searcheuclidian(hasht[i][mod(idfind,hashsize)].lista,clist[t].centro->key1,clist[t].centro->id,idfind,&dilist[t],length,radius,L);
			}
		}
		loop++;
		radius*=2;
	}
	erasedouble(dilist,cent,2);
	insertfromhash(pammatr,clist,dilist,cent,length,2,inlist);
	createnewlist(inlist,clist,cent,length,readcount,2);
	cleancentroids(clist,cent,2);
	for(i=0;i<cent;i++)
		freedlist(&dilist[i]);
	free(dilist);
	free(pammatr[0]);
	free(pammatr[1]);
	free(pammatr);
	freegf(gfun,L);
	free(gfun);
	freefix(vfix,k,L);
	free(vfix);
	free(tfix);
	free(rfix);

}



void lshmatrinit(struct hashtable ** hasht,struct clustlist * clist,struct list * inlist,int L,int k,int length,int readcount,int cent)
{
	struct node * temp;
	struct distlist * dilist;
	int i,j,z,**hmat,**gfun,sum=0,tloop;
	double *tmat,**summid,**matrix,radius,**pammatr;
	char token[k];
	long long decimalnum,binarynum;
	pammatr=malloc(2*sizeof(double*));
	for(i=0;i<2;i++)
		pammatr[i]=malloc(2*sizeof(double));
	radius=firstrad(clist,length,cent,3,inlist);
	dilist=malloc(cent*sizeof(struct distlist));
	for(i=0;i<cent;i++)
		dilist[i].head=NULL;
	matrix=malloc(length*sizeof(double*));
	for(i=0;i<length;i++)//desmevei pinaka typou double gia na perasei ta stoixeia tou arxeiou
	{
		matrix[i]=malloc(length*sizeof(double));		
	}
	hmat=malloc(k*L*sizeof(int*));
	tmat=malloc(k*L*sizeof(double));
	for(i=0;i<k*L;i++)
	{
		hmat[i]=malloc(2*sizeof(int));		
	}
	gfun=malloc(L*sizeof(int*));
	for(i=0;i<L;i++)
	{
		gfun[i]=malloc(k*sizeof(int));
		for(j=0;j<k;j++)
		{
			gfun[i][j]=(rand() /( RAND_MAX + 1.0))*(L*k);//tuxaia h(i)
		}
	}
	temp=inlist->head;
	for(i=0;i<length;i++)
		for(j=0;j<length;j++)
			matrix[i][j]=temp->key1[j];
	for(i=0;i<k*L;i++)
	{
			hmat[i][0]=1+(rand() /( RAND_MAX + 1.0))*(length);//tuxaia items
			hmat[i][1]=1+(rand() /( RAND_MAX + 1.0))*(length);//tuxaia items
			while(hmat[i][0]==hmat[i][1])//an einai idia,ksanakanei random se mia ap tis duo
			{
				hmat[i][1]=1+(rand() /( RAND_MAX + 1.0))*(length);
			}
	}
	summid=malloc(k*L*(sizeof(double*)));
	for(i=0;i<k*L;i++)
	{
		summid[i]=malloc(length*sizeof(double));
		for(j=0;j<length;j++)
		{
			summid[i][j]=0;
			summid[i][j]+=matrix[j][hmat[i][0]]*matrix[j][hmat[i][0]];
			summid[i][j]+=matrix[j][hmat[i][1]]*matrix[j][hmat[i][1]];
			summid[i][j]-=matrix[hmat[i][0]][hmat[i][1]]*matrix[hmat[i][0]][hmat[i][1]];
			summid[i][j]=summid[i][j]/(2*matrix[hmat[i][0]][hmat[i][1]]);//sum gia to kathe item
		}
	}
	for(i=0;i<k*L;i++)
	{
		bubble_sort(summid[i],length);//taksinomisi pinaka me bubble sort
		tmat[i]=summid[i][length/2];//pernaei to meson tou pinaka
	}
	for(z=0;z<L;z++)
	{
		temp=inlist->head;
		for(j=0;j<length;j++)
		{
			for(i=0;i<k;i++)
			{
				sum=0;
				sum+=matrix[j][hmat[gfun[z][i]][0]]*matrix[j][hmat[gfun[z][i]][0]];
				sum+=matrix[j][hmat[gfun[z][i]][1]]*matrix[j][hmat[gfun[z][i]][1]];
				sum-=matrix[hmat[gfun[z][i]][0]][hmat[gfun[z][i]][1]]*matrix[hmat[i][0]][hmat[gfun[z][i]][1]];
				sum=sum/(2*matrix[hmat[gfun[z][i]][0]][hmat[gfun[z][i]][1]]);
				if(sum>=tmat[gfun[z][i]])
					token[i]='1';
				else 
					token[i]='0';				
			}
			binarynum=atoll(token);//pairnei tin duadiki simvoloseira kai tin kanei typo long long int
			decimalnum=turnintodecimal(binarynum);//meta trepei to diadiko se dekadiko wste na paei sto swsto bucket
			insertcosine(hasht[z][decimalnum].lista,matrix[j],temp->id,length);
			temp=temp->next;
		}	
	}
	tloop=1;
	while(tloop!=0)
	{
		tloop=0;
		for(j=0;j<cent;j++)	
		{	
			for(z=0;z<L;z++)
			{
				for(i=0;i<k;i++)
				{
					sum=0;
					sum+=clist[j].centro->key1[hmat[gfun[z][i]][0]]*clist[j].centro->key1[hmat[gfun[z][i]][0]];
					sum+=clist[j].centro->key1[hmat[gfun[z][i]][1]]*clist[j].centro->key1[hmat[gfun[z][i]][1]];
					sum-=matrix[hmat[gfun[z][i]][0]][hmat[gfun[z][i]][1]]*matrix[hmat[i][0]][hmat[gfun[z][i]][1]];
					sum=sum/(2*matrix[hmat[gfun[z][i]][0]][hmat[gfun[z][i]][1]]);
					if(sum>=tmat[gfun[z][i]])
						token[i]='1';
					else 
						token[i]='0';				
				}
				binarynum=atoll(token);//pairnei tin duadiki simvoloseira kai tin kanei typo long long int
				decimalnum=turnintodecimal(binarynum);//meta trepei to diadiko se dekadiko wste na paei sto swsto bucket
				tloop+=searchmatrix(hasht[z][decimalnum].lista,clist[j].centro->key1,clist[j].centro->id,&dilist[j],length,radius,L);
				
			}	
		}
		//loop++;
		radius=2*radius;
	}

	erasedouble(dilist,cent,3);
	insertfromhash(pammatr,clist,dilist,cent,length,3,inlist);
	createnewlist(inlist,clist,cent,length,readcount,3);
	cleancentroids(clist,cent,3);
	for(i=0;i<cent;i++)
		freedlist(&dilist[i]);
	free(dilist);
	free(pammatr[0]);
	free(pammatr[1]);
	free(pammatr);
	freefix(summid,k*L,1);
	free(summid);
	freegf(gfun,L);
	free(gfun);
	free(tmat);
	freegf(hmat,k*L);
	free(hmat);
	freefix(matrix,length,1);
	free(matrix);
}






void cleancentroids(struct clustlist * clist,int cent,int choice)
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
					deleteclustnode(clist,i,temp[j],choice);
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
void deleteclustnode(struct clustlist * clist,int pos,long fid,int choice)
{
	struct centlist* temp,*temp2;
	if(clist[pos].head->id==fid)
	{
		temp = clist[pos].head;
		clist[pos].head = clist[pos].head->next;
		if(choice!=0)
			free(temp->key1);
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
		if(choice!=0)
			free(temp2->key1);
		free(temp2);
	}
}



void createnewlist(struct list * inlist,struct clustlist * clist,int cent,int length,int counter,int choice)
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
		{
			if(choice==0)
				insert(listleft,templist->key,templist->id);
			else if(choice==1)
				insertcosine(listleft,templist->key1,templist->id,length);
			else if(choice==2)
				inserteuclidian(listleft,templist->key1,templist->id,0,length);
		}
		templist=templist->next;
	}
	if(choice==0)
		medpamham(listleft,clist,length,counter,cent,0);
	else if(choice==2)
		medpameucl(listleft,clist,length,counter,cent,2);
	free(tempnode);
	//if(listleft->head!=NULL)
		//freehlist(listleft);
	free(listleft);
}





void erasedouble(struct distlist * dlist,int cent,int choice)
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
					{	if(temp1->distance<temp2->distance)
							deletedistnode(&dlist[j],temp2,choice);
						else
							deletedistnode(&dlist[i],temp1,choice);}
					temp2=temp2->next;
				}
				temp1=temp1->next;
			}
		}
	}
}


void deletedistnode(struct distlist * dlist,struct distnode * dnode,int choice)
{
	struct distnode* temp,*temp2;
	if(dnode->nearid==dlist->head->nearid)
	{
		temp = dlist->head;
		dlist->head = dlist->head->next;
		if(choice!=0)
			free(temp->nearkey1);
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
		if(choice!=0)
			free(temp2->nearkey1);
		free(temp2);
	}
	
}



void insertfromhash(double ** pammatr,struct clustlist * clist,struct distlist * dilist,int cent,int length,int choice,struct list * lista)
{
	struct node * insertnode;
	double distmin,dist;
	int i,j,z;
	struct distnode * temp2;
	insertnode=malloc(sizeof(struct node));
	if(choice!=0)
		insertnode->key1=malloc(length*sizeof(double));
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
					if(choice==0)
						insertnode->key=temp2->nearkey;
					else
					{
						for(z=0;z<length;z++)
						{
							insertnode->key1[z]=temp2->nearkey1[z];
						}
					}
					if(choice==0)
						dist=hamdistance(clist[j].centro,length,insertnode);
					else if(choice==1)
						dist=cosdistance(clist[j].centro,length,insertnode);	
					else if(choice==2)
						dist=eucldistance(clist[j].centro,length,insertnode);
					else if(choice==3)
						dist=matrdistance(lista,clist[j].centro,length,insertnode);
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
	if(choice!=0)
		free(insertnode->key1);
	free(insertnode);
}


double firstrad(struct clustlist * clist,int length,int cent,int choice,struct list * lista)
{
	int i,j;
	double distance,distancemin=1000000;
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
			else if(choice==3)
				distance=matrdistance(lista,clist[i].centro,length,clist[j].centro);
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
