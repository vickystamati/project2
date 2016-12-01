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
#include"execute.h"
#define bufSize 2048
#define bafSize 8192

int commandread(int argc,char * argv[],int  *s,int* k,int* L,int *q,int* cent)
{
	FILE *fp;
	char buf[bufSize],buflen[bufSize];
	if(argc==7 || argc==8)
	{
		if(strcmp(argv[1],"-d")==0)
		{
			if ((fp = fopen((argv[2]), "r")) == NULL)
			{
 				printf("Error open data file \n");
				return 1;
			}			
			fclose(fp);
		}
		else
		{
			printf("Wrong argument 1!\n");
			return 1;
		}
		if(strcmp(argv[3],"-c")==0)
		{
			if ((fp = fopen((argv[4]), "r")) == NULL)
 			{	
				printf("Error open conf file\n");
				return 1;
			}
			else
			{
				while (fscanf(fp, "%s %s", buf,buflen) != EOF)
				{
					if(buf[0]=='n')
					{
						if(buf[15]=='e')
							*cent=atoi(buflen);
						if(buf[15]=='f')
							*k=atoi(buflen);
						if(buf[15]=='t')
							*L=atoi(buflen);
					}
					if(buf[0]=='c')
					{
						if(buf[8]=='s')
							*q=atoi(buflen);
						if(buf[8]=='i')
							*s=atoi(buflen);
					}
				}

			}				
			fclose(fp);
		}
		else
		{
			printf("Wrong argument 2!\n");
			return 1;
		}
		if(strcmp(argv[5],"-o")!=0)
		{
			printf("Wrong argument 3!\n");
				return 1;
		}
	}
	else
	{
		printf("Wrong arguments!\n");
		return 1;	
	}
	return 0;
}


void fileoutput(struct clustlist * clist,int cent,int all,FILE * fp)
{
	struct node * tempnode;	
	int i,size;
	for(i=0;i<cent;i++)
	{
		if(all==0)
		{
			if(clist[i].head==NULL)
				size=0;
			else
				size=clustindexsize(clist,i);
			fprintf(fp,"CLUSTER-%d {size: %d, medoid: %lu}\n",i+1,size,clist[i].centro->id);
		}
		else
		{
			fprintf(fp,"CLUSTER-%d {",i+1);
			if(clist[i].head==NULL)
				fprintf(fp,"-");
			else
				printclustindex(clist,i,fp);
			fprintf(fp,"}\n");

		}
	}
}


int manualexecute(int argc, char *argv[])
{
	FILE* fp ,*fpw;
	char *dfile,*cfile,*ofile; 
	int i,j,z,check,choice,decide,readcount,length,k=4,L=5,s=2,cent=10,res,all,decides[4],q=0;
	char buflen[bufSize];
	double time_spentbr,time_spentlsh,time_spent,jsum,jsum2,** indistmatr,timespent;
	struct list * inlist;
	struct node * centroids;
	struct clustlist * clist,*clistclarans;
	clock_t begin,end;
	srand(time(NULL));
	res=commandread(argc,argv,&s,&k,&L,&q,&cent);
	if(argc==7)
		all=0;
	else
		all=1;
	if(res==1)
		return 0;
	dfile=argv[2];
	cfile=argv[4];
	ofile=argv[6];
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
	begin=clock();
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
		if(choice!=0)
			for(i=0;i<cent;i++)
				free(clist[i].centro->key1);
	}
	else if(decide==1)
	{
		printf("Which initialisation method you want?\nPress 1 for K-metoids++ or 2 for Concentrate (Park-Jun)\n");
		scanf("%d",&decide);
		decides[1]=decide;
		if(decide==1)
		{
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
		else if(decide==2)
		{

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
		printf("Which assignment method you want?\nPress 1 for PAM assignment (simplest approach)  or 2 for Assignment by LSH/DBH: Reverse Approach\n");
		if(decide==1)//apo medoids
		{
			scanf("%d",&decide);
			decides[2]=decide;
			if(decide==1)
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
			else if(decide==2)
				lshassign(inlist,clist,length,readcount,cent,choice,k,L);
			else
			{
				printf("Wrong Choice GOODBYE!\n");
				return 0;
			}
		}
		else if(decide==2)//apo concentrate
		{
			scanf("%d",&decide);
			decides[2]=decide;
			if(decide==1)
			{
				pamassig(inlist,clist,indistmatr,length,readcount,cent,choice);
			}
			else if(decide==2)
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
		printf("Which update method you want?\nPress 1 for update a la Lloyd’s or 2 for CLARANS Update \n");
		scanf("%d",&decide);
		decides[3]=decide;
		if(decide==1)
			lloydsupdate(clist,inlist,cent,choice,length,readcount,jsum);
		else if(decide==2)
			claransstart(clist,inlist,cent,choice,jsum,s,length,readcount,k,L,decides,indistmatr,q);		
	}
	else
		return 0;
	end=clock();
	timespent=(double)(end-begin)/CLOCKS_PER_SEC;
	fpw=fopen(ofile,"w");
	if(decides[0]==0)
		fprintf(fpw,"CLARA\n");
	else
		fprintf(fpw,"I%dA%dU%d\n",decides[1],decides[2],decides[3]);
	fileoutput(clist,cent,all,fpw);
	fprintf(fpw,"clustering_time: %f\n",timespent);
	silhouette(clist,inlist,cent,choice,length,fpw);
	fprintf(fpw,"\n");
	fclose(fpw);
	freeclustlist(clist,cent,choice);
	if(choice!=0)
		for(i=0;i<cent;i++)
			free(clist[i].centro->key1);
	for(i=0;i<cent;i++)
		free(clist[i].centro);
	free(clist);
	if(decides[1]==2)
	{
		for(i=0;i<readcount;i++)
			free(indistmatr[i]);
		free(indistmatr);
	}
	freehlist(inlist);
	free(inlist);
}

int autoexecute(int argc, char *argv[])
{
	FILE* fp ,*fpw;
	char *dfile,*cfile,*ofile; 
	int i,j,z,check,choice,decide,readcount,length,k=4,L=5,s=2,cent=10,res,all,decides[4],q=0,c1,c2,c3;
	char buflen[bufSize];
	double time_spentbr,time_spentlsh,time_spent,jsum,jsum2,** indistmatr,timespent;
	struct list * inlist;
	struct node * centroids;
	struct clustlist * clist,*clistclarans;
	clock_t begin,end;
	srand(time(NULL));
	res=commandread(argc,argv,&s,&k,&L,&q,&cent);
	if(argc==7)
		all=0;
	else
		all=1;
	if(res==1)
		return 0;
	dfile=argv[2];
	cfile=argv[4];
	ofile=argv[6];
	fpw=fopen(ofile,"w");
	decides[0]=0;
	for(c1=1;c1<=2;c1++)
	{
		for(c2=1;c2<=2;c2++)
		{
			for(c3=1;c3<=2;c3++)
			{
				decides[1]=c1;
				decides[2]=c2;
				decides[3]=c3;
				clist=malloc(cent*sizeof(struct clustlist));
				for(i=0;i<cent;i++)
				{
					clist[i].centro=malloc(sizeof(struct node));
					clist[i].head=NULL;
				}
				centroids=malloc(cent*sizeof(struct node));
				inlist=malloc(sizeof(struct list));
				inlist->head=NULL;
				begin=clock();
				if(decides[0]==0)
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
					if(choice!=0)
						for(i=0;i<cent;i++)
							free(clist[i].centro->key1);
				}
				else if(decides[0]==1)
				{
					if(decides[1]==1)
					{
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
					else if(decides[1]==2)
					{
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
						{
							printf("Wrong Choice GOODBYE!\n");
							return 0;
						}
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
					if(decides[1]==1)//apo medoids
					{
						if(decides[2]==1)
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
						else if(decides[2]==2)
							lshassign(inlist,clist,length,readcount,cent,choice,k,L);
						else
						{
							printf("Wrong Choice GOODBYE!\n");
							return 0;
						}
					}
					else if(decides[1]==2)//apo concentrate
					{
						if(decides[2]==1)
						{
							pamassig(inlist,clist,indistmatr,length,readcount,cent,choice);
						}
						else if(decides[2]==2)
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
					if(decides[3]==1)
						lloydsupdate(clist,inlist,cent,choice,length,readcount,jsum);
					else if(decides[3]==2)
						claransstart(clist,inlist,cent,choice,jsum,s,length,readcount,k,L,decides,indistmatr,q);		
				}
				else
					return 0;
				end=clock();
				timespent=(double)(end-begin)/CLOCKS_PER_SEC;
				if(decides[0]==0)
					fprintf(fpw,"CLARA\n");
				else
					fprintf(fpw,"I%dA%dU%d\n",decides[1],decides[2],decides[3]);
				fileoutput(clist,cent,all,fpw);
				fprintf(fpw,"clustering_time: %f\n",timespent);
				silhouette(clist,inlist,cent,choice,length,fpw);
				fprintf(fpw,"\n");
				freeclustlist(clist,cent,choice);
				if(choice!=0 && decides[0]!=0)
				{	for(i=0;i<cent;i++)
						free(clist[i].centro->key1);}
				for(i=0;i<cent;i++)
				{
					free(clist[i].centro);
				}
				free(clist);
				if(decides[1]==2)
				{
					for(i=0;i<readcount;i++)
						free(indistmatr[i]);
					free(indistmatr);
				}
				if(choice!=0)
					freehlist(inlist);
				free(inlist);
				if(decides[0]==0)
				{
					decides[0]=1;
					c3--;
				}
			}
		}
	}
	fclose(fpw);
}

