void createlist(struct list * );
void inserthamm(struct list * ,char * );
void insertcos(struct list * ,char * ,int);
void insertmatr(struct list * ,char * ,int);
void concentrateham(struct list *,double **,int,int,struct node **,int);
void concentratecos(struct list *,double **,int,int,struct node **,int);
void concentrateeucl(struct list *,double **,int,int,struct node **,int);
void concentratematr(struct list *,double **,int ,struct node **,int);
void concentfinal(struct list * ,double ** ,int,struct node **,int);
void medoidsham(struct list * ,double ** ,int ,int ,struct node ** ,int );
int getnumitems(char *);
int gethamlength(char * );
int getcoslength(char * );
int getmatritems(char *);
