#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <CUnit/CUnit.h>
#include "CUnit/Basic.h"
#include<math.h>
#include"hash.h"
#include"list.h"
#include"initialise.h"
#include "assignment.h"
#include"update.h"
#include"clara.h"
#include"lsh.h"
#include"execute.h"

int init_suite1(int argc,char *argv[])
{
   if (NULL == argv[0] && argc==0) {
      return -1;
   }
   else {
      return 0;
   }
}

int clean_suite1(int argc,char *argv[])
{
   if (0 == argc) {
      return -1;
   }
   else {
      return 0;
   }
}



void testMANUAL(int argc,char *argv[])
{

	CU_ASSERT(manualexecute(argc,argv) == 1);

}
void testAUTO(int argc,char *argv[])
{

	CU_ASSERT(autoexecute(argc,argv) == 1);
}


int main()
{
   CU_pSuite pSuite = NULL;

  if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   if ((NULL == CU_add_test(pSuite, "test of testmanual", testMANUAL)) ||
       (NULL == CU_add_test(pSuite, "test of testauto", testAUTO)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }


}

