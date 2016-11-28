#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include <inttypes.h>
#include"hash.h"
#include"list.h"
#include "initialise.h"
#define bufSize 2048
#define bafSize 8192

int main(int argc, char *argv[])
{
	FILE* fp;
	FILE* fpw;
	struct hashtable ** hasht;
	struct distlist * dilist;
	char * tok;
	int readcount=0,columns=0;
	int idcounter;
	long * matrid;
	double **summid;
	double ** matrix,** querymatr;
	int ** hmat,**gfun;
	double * tmat;
	const char s[2] = ",";
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
	double temp,ran,y1,y2,radius;
	double time_spentbr,time_spentlsh,time_spent;
	int decide;
	clock_t beginlsh,endlsh,beginbr,endbr,end,begin;
	srand(time(NULL));
	if(argc==7)
	{
		printf("%s\n",argv[1]); 
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
	printf("Which initialisation method you want?\nPress 0 for K-metoids++ or 1 for Concentrate (Park-Jun)\n");
	scanf("%d",&decide);
	double ** indistmatr;
	int cent=4;//dinetai apo arxeio
	struct list * inlist;
	struct node ** centroids;
	centroids=malloc(cent*sizeof(struct node*));
	inlist=malloc(sizeof(struct list));
	inlist->head=NULL;
	if(decide==0)
	{
		printf("Medoids++\n");
		if(dfile[4]=='H')
		{
			readcount=getnumitems(dfile);
			length=gethamlength(dfile);
			inserthamm(inlist,dfile);
			medoidsham(inlist,indistmatr,length,readcount,centroids,cent);
			for(i=0;i<cent;i++)
				printf("%lu\n",centroids[i]->id);
		}
	}
	else if(decide==1)
	{

		printf("Concentrate\n");
		if(dfile[4]=='H')
		{
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
			readcount=getmatritems(dfile);
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
	printf("DSADASDSASADASDASDASDSADS\n");
	return 0;


































	//hamming
	if(check==0)
	{
		fprintf(fpw,"Hamming\n");
		begin=clock();
		if ((fp = fopen((dfile), "r")) == NULL)
		{
			printf("Empty file!\n");
			return 0;
		}
		while (fscanf(fp, "%s", buflen) != EOF)//metraei ta psifia tou kathe item
		{
			token=buflen; 
			if(token[0]=='0' || token[0]=='1')
			{
				length=strlen(token);//pernaei sto length ton arithmo ton psifion 
				break;
			}
		}
		fclose(fp);
		if ((fp = fopen((dfile), "r")) == NULL)
		{
			printf("Empty file!\n");
			return 0;
		}
		z=strlen(dfile);
		j=0;
		c=0;
		for(i=0;i<z;i++)//metraei ton arithmo ton items
		{
			if(dfile[i]=='x')
				break;	
			if(j==2)
			{
				token4[c]=dfile[i];
				c++;
			}
			if(dfile[i]=='_')
				j++;
				
		}
		readcount=atoi(token4);
		fclose(fp);
		if(k>log(readcount))//elegxos gia to k
		{
			printf("Wrong k value! GOODBYE!\n");
			return 0;
		}
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
		hasht = malloc(L * sizeof(struct hashtable*));
		createhash(hasht,L,hashsize);//dimiourgia hashtable
		if ((fp = fopen((dfile), "r")) == NULL)
		{
			printf("Empty file!\n");
			return 0;
		}
		while (fscanf(fp, "%s %s", buf,buflen) != EOF)
		{
			//printf("mpika while %s %s\n",buf,buflen);
			if(counter>0)
			{
				i=0;
				z=0;
				while(i<strlen(buf))
				{
					if(buf[i]>='0' && buf[i]<='9')//elegxei na einai arithmos
					{
						bufinteger[z]=buf[i];//to dinei ws id
						z++;
					}
					i++;
				}
				itemid=strtol(bufinteger,&pitemid,10);//kratame mono tous arithmous ap to string
				number=strtoull(buflen,&token,2);//vazoume sto number to long long tis akolouthias 0,1
				j=0;
				while(j<L)
				{
					for(i=0;i<k;i++)
					{
						token3[i]=buflen[gfun[j][i]];
					}
					binarynum=atoll(token3);//pairnei tin duadiki simvoloseira kai tin kanei typo long long int
					decimalnum=turnintodecimal(binarynum);//metatrepei to diadiko se dekadiko wste na paei sto swsto bucket
					insert(hasht[j][decimalnum].lista,number,itemid);
					j++;
				}
			}
			counter++;
		}
		fclose(fp);
		end=clock();
		time_spent=(double)(end - begin) / CLOCKS_PER_SEC;
		freegf(gfun,L);
		free(gfun);
		freehasht(hasht,L,hashsize);
		free(hasht);
		//adeiazei ti mnimi pou desmeutike dinamika
	}
//Cosine
	else if(check==1)
	{	
		fprintf(fpw,"Cosine\n");
		begin=clock();
		hasht = malloc(L * sizeof(struct hashtable*));
		createhash(hasht,L,hashsize);
		if ((fp = fopen((dfile), "r")) == NULL)
		{
			printf("Empty file!\n");
			return 0;
		}
		i=0;
		counter=0;
		while (fscanf(fp, "%s", buflen) != EOF)//vazei sto counter ta stoixeia tou kathe item
		{
			if(buflen[0]=='i' && buflen[1]=='t' && buflen[2]=='e' && buflen[3]=='m')
			{
				if(i==1)
				{
					counter--;
					break;
				}
				i++;
			}
			if(i!=0)
				counter++;
		}
		fclose(fp);
		hfix=malloc((k*L)*sizeof(double*));//vazw ta k*L h
		for(i=0;i<k*L;i++)
			hfix[i]=malloc(counter*sizeof(double));
		for(i=0;i<(k*L);i++)
		{
			for(j=0;j<counter;j++)
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
				htable[i][j]=malloc(counter*sizeof(double));
			}
		}
		for(i=0;i<L;i++)
		{
			for(j=0;j<k;j++)
			{			
				htable[i][j]=hfix[gfun[i][j]];

			}
		}
		if ((fp = fopen((dfile), "r")) == NULL)
		{
			printf("Empty file!\n");
			return 0;
		}
		while (fscanf(fp, "%s", buflen) != EOF)//diavazei to arxeio
		{
			if(buflen[0]=='i' && buflen[1]=='t' && buflen[2]=='e' && buflen[3]=='m')//diavazei to kathe item
			{
				vector=malloc(counter*sizeof(double));//pinakas me ta stoixeia tou kathe item
				i=0;
				z=0;
				while(i<strlen(buflen))
				{
					if(buflen[i]>='0' && buflen[i]<='9')
					{
						bufint[z]=buflen[i];
						z++;
					}
					i++;
				}
				itemid=strtol(bufint,&pitemid,10);
				for(z=0;z<counter;z++)
				{
					fscanf(fp, "%s", buflen);
					temp=atof(buflen);
					vector[z]=temp;
				}
				for(i=0;i<L;i++)
				{
					for(j=0;j<k;j++)
					{
						for(z=0;z<counter;z++)
						{
							sum+=htable[i][j][z] * vector[z];
						}
						if(sum>=0)
							token3[j]='1';
						else 
							token3[j]='0';
						sum=0;
					}
					binarynum=atoll(token3);
					decimalnum=turnintodecimal(binarynum);//thesi sto hastable pou tha mpei
					insertcosine(hasht[i][decimalnum].lista,vector,itemid,counter);//insert stin cosine
				}
				free(vector);
			}
		}
		fclose(fp);
		end=clock();
		time_spent=(double)(end - begin) / CLOCKS_PER_SEC;
		freegf(gfun,L);
		free(gfun);
		freefix(hfix,k,L);
		free(hfix);
		freehtable(htable,L,k);
		free(htable);
		freehasht(hasht,L,hashsize);
		free(hasht);//adeiazei ti mnimi pou desmeytike me malloc
	
	}
	//EUCLIDEAN
	else if(check==2)
	{	
		fprintf(fpw,"Euclidean\n");
		begin=clock();
		if(k>10 && L>30)//elegxos gia k kai L
		{
			printf("Wrong values for k,L! GOODBYE!\n");
			return 0;
		}
		for(i=0;i<32;i++)
			modnum*=2;
		modnum=modnum-5;//kataskeui M = (2^32)-5
		if ((fp = fopen((dfile), "r")) == NULL)
		{
			printf("Empty file!\n");
			return 0;
		}
		i=0;
		counter=0;
		while (fscanf(fp, "%s", buflen) != EOF)		{
			if(buflen[0]=='i' && buflen[1]=='t' && buflen[2]=='e' && buflen[3]=='m')
			{
				if(i==1)
				{
					counter--;
					break;
				}
				i++;
			}
			if(i!=0)
				counter++;
		}
		fclose(fp);
		if ((fp = fopen((dfile), "r")) == NULL)
		{
			printf("Empty file!\n");
			return 0;
		}
		while (fgets(buflen, sizeof(buflen), fp) != NULL)
		{
			if(buflen[0]=='i' && buflen[1]=='t' && buflen[2]=='e' && buflen[3]=='m')
				itemcounter++;
				
		}
		fclose(fp);
		hashsize=itemcounter/16;
		hasht = malloc(L * sizeof(struct hashtable*));
		createhash(hasht,L,hashsize);
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
			vfix[i]=malloc(counter*sizeof(double));
		}
		for(i=0;i<k*L;i++)
		{
			tfix[i]=(rand() /( RAND_MAX + 1.0))*w;	//Kataskevazoume to t toy kathe h
			for(j=0;j<counter;j++)
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
		if ((fp = fopen((dfile), "r")) == NULL)
		{
			printf("Empty file!\n");
			return 0;
		}
		while (fscanf(fp, "%s", buflen) != EOF)
		{
			if(buflen[0]=='i' && buflen[1]=='t' && buflen[2]=='e' && buflen[3]=='m')
			{
				vector=malloc(counter*sizeof(double));
				i=0;
				z=0;
				while(i<strlen(buflen))
				{
					if(buflen[i]>='0' && buflen[i]<='9')
						{
							bufint[z]=buflen[i];//pairnei to itemid ws char
							z++;
						}
					i++;
				}
				itemid=strtol(bufint,&pitemid,10);
				for(z=0;z<counter;z++)
				{
					fscanf(fp, "%s", buflen);
					temp=atof(buflen);
					vector[z]=temp;//vazei ola ta stoixeia tou item
				}
				for(i=0;i<L;i++)
				{
					fsum=0;
					for(j=0;j<k;j++)
					{
						sum=0;
						for(z=0;z<counter;z++)
						{
						
							sum=vector[z] * vfix[gfun[i][j]][z];
						}
					
						sum=(sum+tfix[gfun[i][j]])/w;
						sum=floor(sum);//ypologismos h
						fsum+=rfix[j]*sum;//athroisma twn h me ta katallila r(i) gia tin kataskevi tis Φ
					}
					idfind=mod(fsum,modnum);//to id poy prokiptei ap to mod()
					inserteuclidian(hasht[i][mod(idfind,hashsize)].lista,vector,itemid,idfind,counter);
				}
				free(vector);
			}
		}
		fclose(fp);
		end=clock();
		time_spent=(double)(end - begin) / CLOCKS_PER_SEC;
		freegf(gfun,L);
		free(gfun);
		freefix(vfix,k,L);
		free(vfix);
		free(tfix);
		free(rfix);
		freehasht(hasht,L,hashsize);
		free(hasht);//apodeusmeusei mnimis

	}
	else if(check==3)
	{
		fprintf(fpw,"Matrix\n");
		begin=clock();
		hasht = malloc(L * sizeof(struct hashtable*));
		createhash(hasht,L,hashsize);
		begin=clock();
		if ((fp = fopen((dfile), "r")) == NULL)
		{
			printf("Empty file!\n");
			return 0;
		}
		fscanf(fp, "%s", baflen);
		fscanf(fp, "%s", baflen);
		fscanf(fp, "%s", baflen);
		fscanf(fp, "%s", baflen);
		tok = strtok (baflen,s);//diavazei to string me ola ta item
		while (tok != NULL)//ta ksexwrizei kai ta metraei me tin strtok
		{
			tok = strtok (NULL, s);
			counter++;
  		}
		fclose(fp);	
		matrix=malloc(counter*sizeof(double*));
		for(i=0;i<counter;i++)//desmevei pinaka typou double gia na perasei ta stoixeia tou arxeiou
		{
			matrix[i]=malloc(counter*sizeof(double));		
		}
		if ((fp = fopen((dfile), "r")) == NULL)
		{
			printf("Empty file!\n");
			return 0;
		}
		fscanf(fp, "%s", baflen);
		fscanf(fp, "%s", baflen);
		fscanf(fp, "%s", baflen);
		fscanf(fp, "%s", baflen);
		readcount=0;
		i=0;
		j=0;
		while(fscanf(fp, "%s", baflen)!=EOF)
		{
			matrix[i][j]=atof(baflen);//diavazei ta stoixeia tou kathe item
			readcount++;
			j++;
			if(readcount==counter)//epeidi einai ola se mia seira, diavazei mexri ton arithmo twn stoixeion
			{
				readcount=0;
				i++;
				j=0;
			}
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
		for(i=0;i<k*L;i++)
		{
				hmat[i][0]=1+(rand() /( RAND_MAX + 1.0))*(counter);//tuxaia items
				hmat[i][1]=1+(rand() /( RAND_MAX + 1.0))*(counter);//tuxaia items
				while(hmat[i][0]==hmat[i][1])//an einai idia,ksanakanei random se mia ap tis duo
				{
					hmat[i][1]=1+(rand() /( RAND_MAX + 1.0))*(counter);
				}
		}
		
		summid=malloc(k*L*(sizeof(double*)));
		for(i=0;i<k*L;i++)
		{
			summid[i]=malloc(counter*sizeof(double));
			for(j=0;j<counter;j++)
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
			bubble_sort(summid[i],counter);//taksinomisi pinaka me bubble sort
			tmat[i]=summid[i][counter/2];//pernaei to meson tou pinaka
		}

		for(z=0;z<L;z++)
		{
			for(j=0;j<counter;j++)
			{
				for(i=0;i<k;i++)
				{
					sum=0;
					sum+=matrix[j][hmat[gfun[z][i]][0]]*matrix[j][hmat[gfun[z][i]][0]];
					sum+=matrix[j][hmat[gfun[z][i]][1]]*matrix[j][hmat[gfun[z][i]][1]];
					sum-=matrix[hmat[gfun[z][i]][0]][hmat[gfun[z][i]][1]]*matrix[hmat[i][0]][hmat[gfun[z][i]][1]];
					sum=sum/(2*matrix[hmat[gfun[z][i]][0]][hmat[gfun[z][i]][1]]);
					if(sum>=tmat[gfun[z][i]])
						token3[i]='1';
					else 
						token3[i]='0';				
				}
				binarynum=atoll(token3);//pairnei tin duadiki simvoloseira kai tin kanei typo long long int
				decimalnum=turnintodecimal(binarynum);//meta trepei to diadiko se dekadiko wste na paei sto swsto bucket
				itemid=j+1;
				insertcosine(hasht[z][decimalnum].lista,matrix[j],itemid,counter);
			}	
		}
		fclose(fp);
		end=clock();
		time_spent=(double)(end - begin) / CLOCKS_PER_SEC;
		freefix(summid,k*L,1);
		free(summid);
		freegf(gfun,L);
		free(gfun);
		free(tmat);
		freegf(hmat,k*L);
		free(hmat);
		freefix(matrix,counter,1);
		free(matrix);
		freehasht(hasht,L,hashsize);
		free(hasht);
	}
	fclose(fpw);//kleinei to output arxeio,termatismos programmatos
}
