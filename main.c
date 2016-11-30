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
#include"update.h"
#include"clara.h"
#include"lsh.h"
#define bufSize 2048
#define bafSize 8192

int main(int argc, char *argv[])
{
	FILE* fp;
	FILE* fpw;
	char *dfile,*qfile,*ofile; 
	int i,j,check,choice,decide,readcount,length,k=4,L=5,s=2,cent=10;
	char buflen[bufSize];
	double time_spentbr,time_spentlsh,time_spent,jsum,jsum2;
	double ** indistmatr;
	int decides[3];
	struct list * inlist;
	struct node * centroids,*centroidsclarans;
	struct clustlist * clist,*clistclarans;
	clock_t beginlsh,endlsh,beginbr,endbr,end,begin;
	srand(time(NULL));
	if(argc==7)
	{
		if(strcmp(argv[1],"-d")==0)
		{
			dfile=(argv[2]);
			if ((fp = fopen((dfile), "r")) != NULL)
			{
				fscanf(fp, "%s", buflen);
				fscanf(fp, "%s", buflen);
				if((strcmp(buflen,"vector")==0) || strcmp(buflen,"euclidean")==0)
				{
					fscanf(fp, "%s", buflen);
					fscanf(fp, "%s", buflen);
					if((strcmp(buflen,"cosine")==0))
						check=1;
					else
						check=2;	
				}
				else if(strcmp(buflen,"hamming")==0)
					check=0;
				else if(strcmp(buflen,"matrix")==0)
					check=3;
				else
				{
					printf("Wrong file structure!\n");
					return 0;
				}						
			}
			else
				return 0;
		fclose(fp);
		}
		else
		{
			printf("Wrong argument1!\n");
			return 0;
		}
		if(strcmp(argv[3],"-c")==0)
		{
			//qfile=(argv[4]);
		}
		else
		{
			printf("Wrong argument2!\n");
			return 0;
		}
		if(strcmp(argv[5],"-o")==0)
		{
			ofile=argv[6];
			fpw=fopen(ofile, "w");
		}
		else
		{
			printf("Wrong argument3!\n");
			return 0;
		}
	}
	else
	{
		printf("Wrong arguments!\n");
		return 0;	
	}
	clist=malloc(cent*sizeof(struct clustlist));
	for(i=0;i<cent;i++)
	{
		clist[i].centro=malloc(sizeof(struct node));
		clist[i].head=NULL;
	}
	centroids=malloc(cent*sizeof(struct node));
	inlist=malloc(sizeof(struct list));
	inlist->head=NULL;
	printf("Press 0 to use CLARA or 1 for the other options!\n");
	scanf("%d",&decide);
	decides[0]=decide;
	if(decide==0)
	{
			if(dfile[4]=='H')
			{
				choice=0;
				readcount=getnumitems(dfile);
				length=gethamlength(dfile);
				inserthamm(inlist,dfile);
			}
			else if(dfile[4]=='C')
			{
				choice=1;
				readcount=getnumitems(dfile);
				length=getcoslength(dfile);
				insertcos(inlist,dfile,length);
			}
			else if(dfile[4]=='E')
			{
				choice=2;
				readcount=getnumitems(dfile);
				length=getcoslength(dfile);
				insertcos(inlist,dfile,length);
			}
			else if(dfile[4]=='D')
			{
				choice=3;
				readcount=getmatritems(dfile);
				length=readcount;
				insertmatr(inlist,dfile,readcount);
			}
			if(choice!=0)
				for(i=0;i<cent;i++)
					clist[i].centro->key1=malloc(length*sizeof(double));
			clara(clist,inlist,length,readcount,choice,cent);

		
	}
	else if(decide==1)
	{
		printf("Which initialisation method you want?\nPress 0 for K-metoids++ or 1 for Concentrate (Park-Jun)\n");
		scanf("%d",&decide);
		decides[0]=decide;
		if(decide==0)
		{
			printf("Medoids++\n");
			if(dfile[4]=='H')
			{
				choice=0;
				readcount=getnumitems(dfile);
				length=gethamlength(dfile);
				inserthamm(inlist,dfile);
				medoidsham(inlist,length,readcount,centroids,cent);
			}
			else if(dfile[4]=='C')
			{
				choice=1;
				readcount=getnumitems(dfile);
				length=getcoslength(dfile);
				insertcos(inlist,dfile,length);
				medoidscos(inlist,length,readcount,centroids,cent);
			}
			else if(dfile[4]=='E')
			{
				choice=2;
				readcount=getnumitems(dfile);
				length=getcoslength(dfile);
				insertcos(inlist,dfile,length);
				medoidseucl(inlist,length,readcount,centroids,cent);
			}
			else if(dfile[4]=='D')
			{
				choice=3;
				readcount=getmatritems(dfile);
				length=readcount;
				insertmatr(inlist,dfile,readcount);
				medoidsmatr(inlist,readcount,centroids,cent);
			}
		}
		else if(decide==1)
		{

			printf("Concentrate\n");
			if(dfile[4]=='H')
			{
				choice=0;
				readcount=getnumitems(dfile);
				length=gethamlength(dfile);
				indistmatr=malloc(readcount*sizeof(double));//orizoume pinaka apostasewn nxn
				for(i=0;i<readcount;i++)
				{
					indistmatr[i]=malloc(readcount*sizeof(double));
				}
				inserthamm(inlist,dfile);
				concentrateham(inlist,indistmatr,length,readcount,centroids,cent);
			}
			else if(dfile[4]=='C')
			{
				choice=1;
				readcount=getnumitems(dfile);
				length=getcoslength(dfile);
				indistmatr=malloc(readcount*sizeof(double));//orizoume pinaka apostasewn nxn
				for(i=0;i<readcount;i++)
				{
					indistmatr[i]=malloc(readcount*sizeof(double));
				}
				insertcos(inlist,dfile,length);
				concentratecos(inlist,indistmatr,length,readcount,centroids,cent);
			}
			else if(dfile[4]=='E')
			{
				choice=2;
				readcount=getnumitems(dfile);
				length=getcoslength(dfile);
				indistmatr=malloc(readcount*sizeof(double));//orizoume pinaka apostasewn nxn
				for(i=0;i<readcount;i++)
				{
					indistmatr[i]=malloc(readcount*sizeof(double));
				}
				insertcos(inlist,dfile,length);
				concentrateeucl(inlist,indistmatr,length,readcount,centroids,cent);
			}
			else if(dfile[4]=='D')
			{
				choice=3;
				readcount=getmatritems(dfile);
				length=readcount;
				indistmatr=malloc(readcount*sizeof(double));//orizoume pinaka apostasewn nxn
				for(i=0;i<readcount;i++)
				{
					indistmatr[i]=malloc(readcount*sizeof(double));
				}
				insertmatr(inlist,dfile,readcount);
				concentratematr(inlist,indistmatr,readcount,centroids,cent);
			}
			else	
				return 0;
		}
		else
		{
			printf("Wrong Choice GOODBYE!\n");
			return 0;
		}
		for(i=0;i<cent;i++)//initialise clist with cetroids
		{
			clist[i].centro->id=centroids[i].id;
			if(choice==0)
				clist[i].centro->key=centroids[i].key;
			else
			{
				clist[i].centro->key1=malloc(length*sizeof(double));
				for(j=0;j<length;j++)
					clist[i].centro->key1[j]=centroids[i].key1[j];
			}
			clist[i].head=NULL;	
		}
		free(centroids);
		printf("Which assignment method you want?\nPress 0 for PAM assignment (simplest approach)  or 1 for Assignment by LSH/DBH: Reverse Approach\n");
		if(decide==0)//apo medoids
		{
			scanf("%d",&decide);
			decides[1]=decide;
			if(decide==0)
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
			else if(decide==1)
				lshassign(inlist,clist,length,readcount,cent,choice,k,L);
			else
			{
				printf("Wrong Choice GOODBYE!\n");
				return 0;
			}
		}
		else if(decide==1)//apo concentrate
		{
			scanf("%d",&decide);
			decides[1]=decide;
			if(decide==0)
			{
				pamassig(inlist,clist,indistmatr,length,readcount,cent,choice);
			}
			else if(decide==1)
			{
				lshassign(inlist,clist,length,readcount,cent,choice,k,L);
			}
			else
			{
				printf("Wrong Choice GOODBYE!\n");
				return 0;
			}
		}
		jsum=0;
		jsum=calcj(clist,cent);
		printf("Which update method you want?\nPress 0 for update a la Lloyd’s or 1 for CLARANS Update \n");
		scanf("%d",&decide);
		decides[2]=decide;
		if(decide==0)
		{
			lloydsupdate(clist,inlist,cent,choice,length,readcount,jsum);
		}
		else if(decide==1)
		{
			clistclarans=malloc(cent*sizeof(struct clustlist));
			for(i=0;i<cent;i++)
			{
				clistclarans[i].centro=malloc(sizeof(struct node));
				clistclarans[i].head=NULL;
			}

			for(i=0;i<s;i++)
			{
				claransupdate(clist,inlist,cent,choice,length,readcount);
				if(i!=0)
					break;
				swapclist(clistclarans,clist,cent,length,choice);
				freeclustlist(clist,cent,choice);
				claransloop(clist,inlist,cent,choice,length,readcount,decides,indistmatr,k,L);	
			}
			jsum=calcj(clist,cent);
			jsum2=calcj(clistclarans,cent);
			if(jsum2<jsum)
			{
				freeclustlist(clist,cent,choice);
				swapclist(clist,clistclarans,cent,length,choice);
			}
			freeclustlist(clistclarans,cent,choice);
			if(choice!=0)
				for(i=0;i<cent;i++)
					free(clistclarans[i].centro->key1);
			for(i=0;i<cent;i++)
				free(clistclarans[i].centro);
			free(clistclarans);
		}
		
	}
	else
		return 0;
	silhouette(clist,inlist,cent,choice,length);
	freeclustlist(clist,cent,choice);
	if(choice!=0)
		for(i=0;i<cent;i++)
			free(clist[i].centro->key1);
	for(i=0;i<cent;i++)
		free(clist[i].centro);
	free(clist);
	if(decides[0]==1)
	{
		for(i=0;i<readcount;i++)
			free(indistmatr[i]);
		free(indistmatr);
	}
	freehlist(inlist);
	free(inlist);
}
