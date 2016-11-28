void lloydsupdate(struct clustlist * ,struct list *,int ,int ,int,int ,double);
void freeclustlist(struct clustlist * ,int ,int );
void swapclist(struct clustlist * ,struct clustlist * ,int ,int,int);
double approve(struct clustlist *,struct list *,struct node *,int,int,int,int ,double);
void silhouette(struct clustlist *,struct list * ,int ,int,int );
void claransupdate(struct clustlist * ,struct list * ,int ,int ,int ,int );
void clarans(struct clustlist * ,struct list * ,int ,int ,int ,int ,int*,int );
void claransloop(struct clustlist * ,struct list * ,int ,int ,int ,int,int *,double ** );
int modint (int , int );
