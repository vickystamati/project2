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
#include"lsh.h"
#include"execute.h"

int main(int argc, char *argv[])
{
	int a;
	printf("Press 0 for auto use OR press 1 for manual use\n");
	scanf("%d",&a);
	if(a==0)
		autoexecute(argc,argv);//ektelesi olwn twn sindiasmwn
	else if(a==1)
		manualexecute(argc,argv);//menu me xeirokinites epiloges
}
