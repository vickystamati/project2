struct centlist
{
	long id;
	long long key;
	double * key1;

	double distance;

	int listnearid;
	double distancenear;
	double sav;
	//double near2dist[2][2];//pinakas me id
	struct centlist * next;
};

struct clustlist
{
	struct node * centro;
	struct centlist * head;
};

void pamassig(struct list *,struct clustlist * ,double ** ,int,int ,int,int);
void insertassig(double **,struct clustlist * ,struct node *,int,int);
double calcj(struct clustlist * ,int );
void medpamham(struct list * ,struct clustlist *,int ,int ,int ,int );
void medpamcos(struct list * ,struct clustlist * ,int ,int ,int ,int );
void medpameucl(struct list * ,struct clustlist * ,int ,int ,int ,int );
void medpammatr(struct list * ,struct clustlist * ,int ,int ,int ,int );
double hamdistance(struct node *,int ,struct node *);
double cosdistance(struct node *,int ,struct node *);
double eucldistance(struct node *,int ,struct node *);
double matrdistance(struct list *,struct node *,int ,struct node *);
void lshassign(struct list * inlist,struct clustlist * clist,int length,int readcount,int cent,int choice,int k,int L);
void printclustindex(struct clustlist * clist,int pos,FILE *);
int clustindexsize(struct clustlist * clist,int pos);

