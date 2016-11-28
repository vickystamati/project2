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
#include<time.h>
#define bufSize 2048
#define bafSize 8192

void inserthamm(struct list * lista,char * dfile)
{
	struct node * new;
	FILE* fp;
	int counter=0,i,j,z;	
	char bufinteger[bufSize],buflen[bufSize],buf[bufSize];
	unsigned long long number;
	unsigned long itemid;
	char * pitemid,*token;
	if ((fp = fopen((dfile), "r")) == NULL)
	{
		printf("Empty file!\n");
	}
	while (fscanf(fp, "%s %s", buf,buflen) != EOF)
	{
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
			insert(lista,number,itemid);
		}
		counter++;
	}
	fclose(fp);
}


void insertcos(struct list * lista,char * dfile,int counter)
{
	struct node * new;
	FILE* fp;	
	int i,j,z;
	char bufint[bufSize],buflen[bufSize];
	unsigned long itemid;
	char * pitemid;
	double * vector;
	if ((fp = fopen((dfile), "r")) == NULL)
	{
		printf("Empty file!\n");
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
			itemid=strtol(bufint,&pitemid,10);//pairnei to ID
			for(z=0;z<counter;z++)
			{
				fscanf(fp, "%s", buflen);
				vector[z]=atof(buflen);
			}
			insertcosine(lista,vector,itemid,counter);//insert stin cosine
			free(vector);
		}
	}
	fclose(fp);
}

void insertmatr(struct list * lista,char * dfile,int counter)
{
	struct node * new;
	FILE* fp;	
	char s[2] = ",";
	char * tok;
	int i,j,z;
	char bufint[bufSize],buflen[bafSize],buf[bufSize];
	unsigned long itemid;
	char * pitemid;
	double * vector;
	if ((fp = fopen((dfile), "r")) == NULL)
	{
		printf("Empty file!\n");
	}
	fscanf(fp, "%s", buflen);
	fscanf(fp, "%s", buflen);
	fscanf(fp, "%s", buflen);
	fscanf(fp, "%s", buflen);
	tok = strtok (buflen,s);//diavazei to string me ola ta item
	while (tok != NULL)//ta ksexwrizei kai ta metraei me tin strtok
	{
		vector=malloc(counter*sizeof(double));//pinakas me ta stoixeia tou kathe item
		i=0;
		z=0;
		while(i<strlen(tok))
		{
			if(tok[i]>='0' && tok[i]<='9')
			{
				bufint[z]=tok[i];
				z++;
			}
			i++;
		}
		itemid=strtol(bufint,&pitemid,10);//pairnei to ID
		for(z=0;z<counter;z++)
		{
			fscanf(fp, "%s", buf);
			vector[z]=atof(buf);

		}		
		insertcosine(lista,vector,itemid,counter);//insert stin cosine
		tok = strtok (NULL, s);
		free(vector);
	}
	fclose(fp);
}

int getmatritems(char *dfile)
{
	FILE * fp;
	char s[2] = ",";
	char * tok;
	int i;
	char buflen[bafSize];
	int readcount=0;
	if ((fp = fopen((dfile), "r")) == NULL)
	{
		printf("Empty file!\n");
		return 0;
	}
	i=0;
	readcount=0;
	fscanf(fp, "%s", buflen);
	fscanf(fp, "%s", buflen);
	fscanf(fp, "%s", buflen);
	fscanf(fp, "%s", buflen);
	tok = strtok (buflen,s);//diavazei to string me ola ta item
	while (tok != NULL)//ta ksexwrizei kai ta metraei me tin strtok
	{
		tok = strtok (NULL, s);
		readcount++;
	}
	fclose(fp);
	return readcount;
}



int getnumitems(char *dfile)
{
	FILE * fp;
	int i;
	char buflen[bufSize];
	int readcount=0;
	if ((fp = fopen((dfile), "r")) == NULL)
	{
		printf("Empty file!\n");
		return 0;
	}
	i=0;
	readcount=0;
	while (fscanf(fp, "%s", buflen) != EOF)//vazei sto counter ta stoixeia tou kathe item
	{
		if(buflen[0]=='i' && buflen[1]=='t' && buflen[2]=='e' && buflen[3]=='m')
		{
			readcount++;
		}
	}
	fclose(fp);
	return readcount;
}

int gethamlength(char * dfile)
{
	int i;
	int length;
	char buf[bufSize];
	FILE * fp;
	if ((fp = fopen((dfile), "r")) == NULL)
	{
		printf("Empty file!\n");
		return 0;
	}
	i=0;
	length=0;
	while (fscanf(fp, "%s", buf) != EOF)//vazei sto counter ta stoixeia tou kathe item
	{
		if(buf[0]=='0' || buf[0]=='1')
		{
			length=strlen(buf);
			break;
		}
	}
	fclose(fp);
	return length;
}


int getcoslength(char * dfile)
{
	int i=0;
	char buflen[bufSize];
	int counter=0;
	FILE * fp;
	if ((fp = fopen((dfile), "r")) == NULL)
	{
		printf("Empty file!\n");
		return 0;
	}
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
	return counter;
}


void concentrateham(struct list * lista,double ** matrix,int length,int counter,struct node * centroids,int cent)
{
	struct node * temp;
	struct node * temp2;
	int i,j,z;
	char * token;
	char * token2;
	double distance;
	temp=lista->head;
	i=0;
	while(temp!=NULL)
	{
		j=0;
		token=malloc(length*sizeof(char));
		turnintobinary(temp->key ,length ,token);
		temp2=lista->head;
		while(temp2!=NULL && i>j)
		{
			distance=0;
			token2=malloc(length*sizeof(char));
			turnintobinary(temp2->key ,length ,token2);
			for(z=0;z<length;z++)
			{
				if(token[z]!=token2[z])
					distance++;
			}
			matrix[i][j]=distance;
			free(token2);
			temp2=temp2->next;
			j++;
		}
		free(token);
		temp=temp->next;
		i++;
	}
		for(i=0;i<counter;i++)
		{
			for(j=i;j<counter;j++)
			{
				if(i==j)
					matrix[i][j]=0;
				else
					matrix[i][j]=matrix[j][i];
			}
		}
	concentfinal(lista,matrix,counter,centroids,cent);
}


void concentratecos(struct list * lista,double ** matrix,int length,int counter,struct node * centroids,int cent)
{
	struct node * temp;
	struct node * temp2;
	int sum,sum1,sum2;
	int i,j,z;
	double distance;
	temp=lista->head;
	i=0;
	while(temp!=NULL)
	{
		j=0;
		temp2=lista->head;
		while(temp2!=NULL && i>j)
		{
			sum=0,sum1=0,sum2=0;
			for(z=0;z<counter;z++)//upologismos typou cosine
			{
				sum+=temp->key1[z] * temp2->key1[z];
				sum1+=temp->key1[z] * temp->key1[z];
				sum2+=temp2->key1[z] * temp2->key1[z];
			}
			if((sum1>0 && sum2>0) || (sum1<0 && sum2<0))//epeidi einai riza, elegxos na einai diaforetiko ap to 0
			{
				matrix[i][j]=1-(sum/(sqrt(sum1) * sqrt(sum2)));
			}
			else 
				matrix[i][j]=0;
			j++;
			temp2=temp2->next;

		}
		i++;
		temp=temp->next;
	}
	for(i=0;i<counter;i++)
	{
		for(j=i;j<counter;j++)
		{
			if(i==j)
				matrix[i][j]=0;
			else
				matrix[i][j]=matrix[j][i];
		}
	}
	concentfinal(lista,matrix,counter,centroids,cent);
}

void concentrateeucl(struct list * lista,double ** matrix,int length,int counter,struct node * centroids,int cent)
{
	struct node * temp;
	struct node * temp2;
	int sum,sum2;
	int i,j,z;
	temp=lista->head;
	i=0;
	while(temp!=NULL)
	{
		j=0;
		temp2=lista->head;
		while(temp2!=NULL && i>j)
		{
			sum=0;
			sum2=0;
			for(z=0;z<counter;z++)//typos euclidean
			{
				sum=temp->key1[z] - temp2->key1[z];
				sum=sum*sum;
				sum2+=sum;
				sum=0;
			}
			matrix[i][j]=sqrt(sum2);//upologismos apostasis
			j++;
			temp2=temp2->next;
		}
		i++;
		temp=temp->next;
	}
	for(i=0;i<counter;i++)
	{
		for(j=i;j<counter;j++)
		{
			if(i==j)
				matrix[i][j]=0;
			else
				matrix[i][j]=matrix[j][i];
		}
	}
	concentfinal(lista,matrix,counter,centroids,cent);
}

void concentratematr(struct list * lista,double ** matrix,int counter,struct node * centroids,int cent)
{
	int i,j;
	struct node * temp;
	temp=lista->head;
	i=0;
	while(temp!=NULL)
	{
		for(j=0;j<counter;j++)
		{
			matrix[i][j]=temp->key1[j];
		}
		i++;
		temp=temp->next;
	}
	concentfinal(lista,matrix,counter,centroids,cent);
}




void concentfinal(struct list * lista,double ** matrix,int counter,struct node * centroids,int cent)
{
	struct node * temp;
	int i,j,t;
	double sum, **summax;
	summax=malloc(counter*sizeof(double*));
	for(i=0;i<counter;i++)
		summax[i]=malloc(2*sizeof(double));
	temp=lista->head;
	for(i=0;i<counter;i++)
	{
		summax[i][0]=temp->id;
		summax[i][1]=0;
		for(j=0;j<counter;j++)
		{
			sum=0;
			for(t=0;t<counter;t++)
			{	
				sum+=matrix[j][t];
			}
			summax[i][1]+=matrix[i][j]/sum;
		}
		temp=temp->next;
	}
	bubble_sort2d(summax, counter);
	/*for(i=0;i<counter;i++)
	{

			printf("buble %f\n",summax[i][1]);
		
	}*/
	for(i=0;i<cent;i++)
	{
		temp=lista->head;
		while(temp->id!=summax[i][0])
		{
			temp=temp->next;
		}
		centroids[i]=*temp;	
	}
}

void medoidsham(struct list * lista,int length,int counter,struct node * centroids,int cent)
{
	int random;
	struct node * temp;
	struct node * temp2;
	double ** disttable;
	double ** ptable;
	int i,j,z;
	double distance,distancemin;
	char * token, * token2;
	double x,r;
	int flag;
	int ccount=0;//metritis kentroidon
	srand(time(NULL));	
	disttable=malloc(2*sizeof(double*));
	ptable=malloc(2*sizeof(double*));	
	while(ccount<cent)
	{	
		for(i=0;i<2;i++)
		{
			disttable[i]=malloc((counter-ccount-1)*sizeof(double));
			ptable[i]=malloc((counter-ccount)*sizeof(double));
		}	
		temp=lista->head;
		if(ccount==0)
		{
			random=1+ (rand() /( RAND_MAX + 1.0))*(counter-1);
			for(i=0;i<random;i++)
			{
				temp=temp->next;
			}
		}
		else
		{
			while(temp->id!=r)
				temp=temp->next;		
		}
		centroids[ccount]=*temp;
		if(ccount==(cent-1))
			break;
		temp2=lista->head;
		j=0;
		if(ccount==0)
		{
			token=malloc(length*sizeof(char));
			turnintobinary(temp->key ,length ,token);
			while(temp2!=NULL)
			{
				distance=0;
				if(temp2->id!=centroids[ccount].id)
				{
					token2=malloc(length*sizeof(char));
					turnintobinary(temp2->key ,length ,token2);
					for(z=0;z<length;z++)
					{
						if(token[z]!=token2[z])
							distance++;
					}
					free(token2);
					disttable[0][j]=temp2->id;
					disttable[1][j]=distance;
					j++;
				}	
				temp2=temp2->next;
			}
			free(token);
		}
		else
		{
			while(temp2!=NULL)
			{
				flag=0;
				distancemin=100000;//paradoxi
				for(i=0;i<ccount;i++)
				{
					if(temp2->id==centroids[i].id)
					{
						flag=1;
						break;
					}
				}
				if(flag==0)
				{
					token2=malloc(length*sizeof(char));
					turnintobinary(temp2->key ,length ,token2);
					for(i=0;i<ccount;i++)
					{
						token=malloc(length*sizeof(char));
						turnintobinary(centroids[i].key ,length ,token);
						distance=0;
						for(z=0;z<length;z++)
						{
							if(token[z]!=token2[z])
								distance++;
						}
						free(token);
						if(distance<distancemin)
							distancemin=distance;
					}
					free(token2);
					disttable[0][j]=temp2->id;
					disttable[1][j]=distancemin;
					j++;
					
				}	
				temp2=temp2->next;
			}
		}
		ccount++;
		ptable[1][0]=0;
		for(i=1;i<(counter-ccount);i++)
		{
			ptable[0][i]=disttable[0][i-1];
			ptable[1][i]+=ptable[1][i-1]+(disttable[1][i-1] *disttable[1][i-1]);
		}
		x=1+ (rand() /( RAND_MAX + 1.0))*(ptable[1][counter-ccount-1]-1);
		for(i=0;i<(counter-ccount);i++)
		{
			if(x<=ptable[1][i])
			{
				r=ptable[0][i];
				break;
			}
			
		}	
		//printf("%f\n",r);
		for(i=0;i<2;i++)
		{
			//free(disttable[i]);
			//free(ptable[i]);
		}
		/*free(ptable);
		free(disttable);*/
	}

}

void medoidscos(struct list * lista,int length,int counter,struct node * centroids,int cent)
{
	int random;
	struct node * temp;
	struct node * temp2;
	double ** disttable;
	double ** ptable;
	int i,j,z,flag;
	double sum,sum1,sum2;
	double distance,distancemin;
	double x,r;
	int ccount=0;//metritis kentroidon
	disttable=malloc(2*sizeof(double*));
	ptable=malloc(2*sizeof(double*));
	srand(time(NULL));		
	while(ccount<cent)
	{
		for(i=0;i<2;i++)
		{
			disttable[i]=malloc((counter-ccount-1)*sizeof(double));
			ptable[i]=malloc((counter-ccount)*sizeof(double));
		}
		temp=lista->head;
		if(ccount==0)
		{
			random=1+ (rand() /( RAND_MAX + 1.0))*(counter-1);
			for(i=0;i<random;i++)
			{
				temp=temp->next;
			}
		}
		else
		{
			while(temp->id!=r)
				temp=temp->next;	
		}
		centroids[ccount]=*temp;
		if(ccount==(cent-1))
			break;
		j=0;
		temp2=lista->head;
		if(ccount==0)
		{
			while(temp2!=NULL)
			{
				if(temp2->id!=centroids[ccount].id)
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
						disttable[0][j]=temp2->id;
						disttable[1][j]=1-(sum/(sqrt(sum1) * sqrt(sum2)));
					}
					else 
					{
						disttable[0][j]=temp2->id;
						disttable[1][j]=0;
					}
					j++;
				}
				temp2=temp2->next;
			}
		}
		else
		{
			while(temp2!=NULL)
			{
				flag=0;
				distancemin=100000;//paradoxi
				for(i=0;i<ccount;i++)
				{
					if(temp2->id==centroids[i].id)
					{
						flag=1;
						break;
					}
				}
				if(flag==0)
				{
					for(i=0;i<ccount;i++)
					{
						sum=0,sum1=0,sum2=0;
						for(z=0;z<length;z++)//upologismos typou cosine
						{
							sum+=centroids[i].key1[z] * temp2->key1[z];
							sum1+=centroids[i].key1[z] * centroids[i].key1[z];
							sum2+=temp2->key1[z] * temp2->key1[z];
						}
						if(sum1>0 && sum2>0)//epeidi einai riza, elegxos na einai diaforetiko ap to 0
						{
							distance=1-(sum/(sqrt(sum1) * sqrt(sum2)));
							if(distance<distancemin)
								distancemin=distance;
						}
					}
					disttable[0][j]=temp2->id;
					disttable[1][j]=distance;
					j++;
				}
				temp2=temp2->next;
			}
		}
		ccount++;
		ptable[1][0]=0;
		for(i=1;i<(counter-ccount);i++)
		{
			ptable[0][i]=disttable[0][i-1];
			ptable[1][i]+=ptable[1][i-1]+(disttable[1][i-1] *disttable[1][i-1]);
		}
		x=1+ (rand() /( RAND_MAX + 1.0))*(ptable[1][counter-ccount-1]-1);
		for(i=0;i<(counter-ccount);i++)
		{
			if(x<=ptable[1][i])
			{
				r=ptable[0][i];
				break;
			}
			
		}	
		//printf("%f\n",r);
		/*for(i=0;i<2;i++)
		{
			free(disttable[i]);
			free(ptable[i]);
		}
		free(ptable);
		free(disttable);*/
	}

}

void medoidseucl(struct list * lista,int length,int counter,struct node * centroids,int cent)
{
	int random;
	struct node * temp;
	struct node * temp2;
	double ** disttable;
	double ** ptable;
	int i,j,z,flag;
	double sum,sum2;
	double distance,distancemin;
	double x,r;
	int ccount=0;//metritis kentroidon
	disttable=malloc(2*sizeof(double*));
	ptable=malloc(2*sizeof(double*));
	srand(time(NULL));		
	while(ccount<cent)
	{
		for(i=0;i<2;i++)
		{
			disttable[i]=malloc((counter-ccount-1)*sizeof(double));
			ptable[i]=malloc((counter-ccount)*sizeof(double));
		}
		temp=lista->head;
		if(ccount==0)
		{
			random=1+ (rand() /( RAND_MAX + 1.0))*(counter-1);
			for(i=0;i<random;i++)
			{
				temp=temp->next;
			}
		}
		else
		{
			while(temp->id!=r)
				temp=temp->next;	
		}
		centroids[ccount]=*temp;
		if(ccount==(cent-1))
			break;
		j=0;
		temp2=lista->head;
		if(ccount==0)
		{
			while(temp2!=NULL)
			{
				if(temp2->id!=centroids[ccount].id)
				{
					sum=0;
					sum2=0;
					for(z=0;z<length;z++)//typos euclidean
					{
						sum=temp2->key1[z] - centroids[ccount].key1[z];
						sum=sum*sum;
						sum2+=sum;
						sum=0;
					}
					disttable[0][j]=temp2->id;
					disttable[1][j]=sqrt(sum2);//upologismos apostasis
					j++;
				}
				temp2=temp2->next;
			}
		}
		else
		{
			while(temp2!=NULL)
			{
				flag=0;
				distancemin=100000;//paradoxi
				for(i=0;i<ccount;i++)
				{
					if(temp2->id==centroids[i].id)
					{
						flag=1;
						break;
					}
				}
				if(flag==0)
				{
					for(i=0;i<ccount;i++)
					{
						sum=0,sum2=0;
						for(z=0;z<length;z++)//typos euclidean
						{
							sum=temp2->key1[z] - centroids[ccount].key1[z];
							sum=sum*sum;
							sum2+=sum;
							sum=0;
						}
						distance=sqrt(sum2);
						if(distance<distancemin)
							distancemin=distance;
					}
					disttable[0][j]=temp2->id;
					disttable[1][j]=distance;
					j++;
				}
				temp2=temp2->next;
			}
		}
		ccount++;
		ptable[1][0]=0;
		for(i=1;i<(counter-ccount);i++)
		{
			ptable[0][i]=disttable[0][i-1];
			ptable[1][i]+=ptable[1][i-1]+(disttable[1][i-1] *disttable[1][i-1]);
		}
		x=1+ (rand() /( RAND_MAX + 1.0))*(ptable[1][counter-ccount-1]-1);
		for(i=0;i<(counter-ccount);i++)
		{
			if(x<=ptable[1][i])
			{
				r=ptable[0][i];
				break;
			}
			
		}	
		//printf("%f\n",r);
		/*for(i=0;i<2;i++)
		{
			free(disttable[i]);
			free(ptable[i]);
		}
		free(ptable);
		free(disttable);*/
	}

}





/*void medoidseucl(struct list * lista,double ** matrix,int length,int counter,struct node * centroids,int cent)
{
	int random;
	struct node * temp;
	struct node * temp2;
	double ** disttable;
	double ** ptable;
	int i,j,z;
	double sum,sum2;
	double distance;
	double x,r;
	int ccount=0;//metritis kentroidon
	srand(time(NULL));		
	while(ccount<cent)
	{

		disttable=malloc(2*sizeof(double*));
		ptable=malloc(2*sizeof(double*));
		for(i=0;i<2;i++)
		{
			disttable[i]=malloc((counter-ccount-1)*sizeof(double));
			ptable[i]=malloc((counter-ccount)*sizeof(double));
		}	
		temp=lista->head;
		if(ccount==0)
		{		
			random=1+ (rand() /( RAND_MAX + 1.0))*(counter-1);
			for(i=0;i<random;i++)
			{
				temp=temp->next;
			}
		}
		else
		{
			while(temp->id!=r)
			{
				temp=temp->next;	
			}
		}
		centroids[ccount]=*temp;
		if(ccount==(cent-1))
			break;
		j=0;
		temp2=lista->head;
		while(temp2!=NULL)
		{
			if(temp2->id!=centroids[ccount].id)
			{
				sum=0;
				sum2=0;
				for(z=0;z<length;z++)//typos euclidean
				{
					sum=temp->key1[z] - temp2->key1[z];
					sum=sum*sum;
					sum2+=sum;
					sum=0;
				}
				disttable[0][j]=temp2->id;
				disttable[1][j]=sqrt(sum2);//upologismos apostasis
				j++;
			}
			temp2=temp2->next;
		}
		ccount++;
		ptable[1][0]=0;
		for(i=1;i<(counter-ccount);i++)
		{
			ptable[0][i]=disttable[0][i-1];
			ptable[1][i]+=ptable[1][i-1]+(disttable[1][i-1] *disttable[1][i-1]);
		}
		x=1+ (rand() /( RAND_MAX + 1.0))*(ptable[1][counter-ccount-1]-1);
		for(i=0;i<(counter-ccount);i++)
		{
			if(x<=ptable[1][i])
			{
				r=ptable[0][i];
				break;
			}
			
		}	
		//printf("%f\n",r);
		for(i=0;i<2;i++)
		{
			free(disttable[i]);
			free(ptable[i]);
		}
		free(ptable);
		free(disttable);
	}

}*/

void medoidsmatr(struct list * lista,int counter,struct node * centroids,int cent)
{
	int random,flag;
	double distance,distancemin;
	struct node * temp;
	struct node * temp2;
	double ** disttable;
	double ** ptable;
	int i,j,z;
	double x,r;
	int ccount=0;//metritis kentroidon
	srand(time(NULL));	
	int pos[cent];	
	while(ccount<cent)
	{

		disttable=malloc(2*sizeof(double*));
		ptable=malloc(2*sizeof(double*));
		for(i=0;i<2;i++)
		{
			disttable[i]=malloc((counter-ccount-1)*sizeof(double));
			ptable[i]=malloc((counter-ccount)*sizeof(double));
		}	
		temp=lista->head;
		if(ccount==0)
		{		
			random=1+ (rand() /( RAND_MAX + 1.0))*(counter-1);
			for(i=0;i<random;i++)
			{
				temp=temp->next;

			}
			pos[ccount]=random;
		}
		else
		{
			pos[ccount]=0;
			while(temp->id!=r)
			{
				pos[ccount]++;
				temp=temp->next;	
			}
		}
		centroids[ccount]=*temp;
		if(ccount==(cent-1))
			break;
		j=0;
		temp2=lista->head;
		if(ccount==0)
		{
			while(temp2!=NULL)
			{
				if(temp2->id!=centroids[ccount].id)
				{
					disttable[0][j]=temp2->id;
					disttable[1][j]=temp2->key1[pos[ccount]];//upologismos apostasis
					j++;
				}
				temp2=temp2->next;
			}
		}
		else
		{
			while(temp2!=NULL)
			{
				flag=0;
				distancemin=100000;//paradoxi
				for(i=0;i<ccount;i++)
				{
					if(temp2->id==centroids[i].id)
					{
						flag=1;
						break;
					}
				}
				if(flag==0)
				{
					for(z=0;z<ccount;z++)
					{
						distance=temp2->key1[pos[z]];
						if(distance<distancemin)
							distancemin=distance;
					}


				disttable[0][j]=temp2->id;
				disttable[1][j]=distancemin;//upologismos apostasis
				j++;
				}
				temp2=temp2->next;
			}
		}
		ccount++;	
		ptable[1][0]=0;
		for(i=1;i<(counter-ccount);i++)
		{
			ptable[0][i]=disttable[0][i-1];
			ptable[1][i]+=ptable[1][i-1]+(disttable[1][i-1] *disttable[1][i-1]);
		}
		x=1+ (rand() /( RAND_MAX + 1.0))*(ptable[1][counter-ccount-1]-1);
		for(i=0;i<(counter-ccount);i++)
		{
			if(x<=ptable[1][i])
			{
				r=ptable[0][i];
				break;
			}	
		}	
	}
}







