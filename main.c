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
#define bufSize 2048
#define bafSize 8192

int main(int argc, char *argv[])
{
	FILE* fp;
	FILE* fpw;
	struct hashtable ** hasht;
	struct distlist * dilist;
	char * tok;
	int choice;
	int readcount=0,columns=0;
	int idcounter;
	long * matrid;
	double **summid;
	double ** matrix,** querymatr;
	int ** hmat,**gfun;
	double * tmat;
	//const char s[2] = ",";
	double sum=0;
	double *** htable;
	double ** hfix;
	double w=4.0;
	double *tfix,**vfix,*vector;
	int *rfix;
	int fsum;
	long modnum=1,itemcounter=0,idfind;
	int length,counter=0;
	char bufinteger[bufSize],buflen[bufSize],buf[bufSize],bufint[bufSize],baflen[bafSize], newfilename[30];;
	char *token;	
	char *dfile,*qfile,*ofile; 
	int k=3;//megethos hastable 2^k 
	int L=5;//arithmos hashtable
	long hashsize=2;
	int i,j,z,c,check,random;
	char token3[k],token4[k];
	unsigned long long binarynum,decimalnum,number;
	unsigned long itemid;
	char * pitemid;
	double temp,ran,y1,y2;
	double time_spentbr,time_spentlsh,time_spent;
	int decide;
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
	for(i=1;i<k;i++)
	{
		hashsize*=2;
	}
	double ** indistmatr;
	double jsum,jsum2;
	int decides[3];
	int s=2;
	int cent=15;//dinetai apo arxeio
	struct list * inlist;
	struct node * centroids,*centroidsclarans;
	struct clustlist * clist,*clistclarans;
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
				//printf("cent %d  ss %lu\n",cent,clist[i].centro->id);
				//free(centroids[i]);	
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
			{
		
			}
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
		printf("mpwiwedwww\n");
			clistclarans=malloc(cent*sizeof(struct clustlist));
			centroids=malloc(cent*sizeof(struct node));
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
				claransloop(clist,inlist,cent,choice,length,readcount,decides,indistmatr);	
			}
			for(i=0;i<cent;i++)
				printf("clist %lu  kai claralist %lu\n",clist[i].centro->id,clistclarans[i].centro->id);

			jsum=calcj(clist,cent);
			jsum2=calcj(clistclarans,cent);
			printf("%f %f\n",jsum,jsum2);
			if(jsum2<jsum)
			{
				printf("mpika\n");
				freeclustlist(clist,cent,choice);
				swapclist(clist,clistclarans,cent,length,choice);
			}
		}
		
	}
	else
		return 0;


	//printf("DSADASD\n");
	silhouette(clist,inlist,cent,choice,length);

	return 0;
}
