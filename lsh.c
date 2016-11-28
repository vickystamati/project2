/*void lshham(struct list * inlist,int L,int k,struct clustlist * clist,int length)
{
	struct hashtable ** hasht;
	int ** gfun;
	int random,i,j;
	long hashsize=2;
	for(i=1;i<k;i++)
	{
		hashsize*=2;
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
}
*/
