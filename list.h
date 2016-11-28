struct distnode{
	 long long nearkey;
	 long nearid;
	double distance;
	struct distnode *next;
};

struct distlist{
	struct distnode * head;
};


struct node{
	long long key;//binary
	long  id;//itemid
	long findid;//ID P
	double dist;
	double * key1;
	struct node * next;
};

struct list{
	struct node * head;
};


void createdlist(struct distlist * );
void insert(struct list * ,long long ,long );
void insertnear(struct distlist * ,struct distnode * );
void findmin(struct distlist * ,int,FILE*);
void search(struct list * ,long long,long ,struct distlist * ,int,double,int);
void turnintobinary( long long ,int ,char* );
void printdistancelist(struct distlist * ,int,FILE*);
void insertcosine(struct list *,double * , long,int);
void searchcosine(struct list *,double* , long ,struct distlist * ,int,double,int);
void printdistancelistcosine(struct distlist *,FILE*);
void inserteuclidian(struct list *,double* , long ,long ,int);
void searcheuclidian(struct list *,double* ,long,long ,struct distlist * ,int,double,int);
void printdistancelisteuclidian(struct distlist *,FILE*);
void searchmatrix(struct list* ,double ** ,long ,struct  distlist * , int,double ,int);
void freehlist(struct list*);
void freedlist(struct distlist*);
long int mod (long int , long int );
void bubble_sort(double* , int );
long long turnintodecimal(long long);
void bubble_sort2d(double** matr, int counter);
