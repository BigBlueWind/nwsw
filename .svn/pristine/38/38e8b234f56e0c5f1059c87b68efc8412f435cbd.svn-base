/**
 * @file    imc_ut.c
 * @brief   This file holds the code for Unit Testing(UT) of the IMC module.
 * @brief   UT involves testing of functions in the IMC APIs.
 *
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 *
 */

#include <check.h>
#include "all_cmn.h"

#include "imc_buffer.h"
#include "imc_queue.h"
/* ##################################### GLOBALS ################################################################### */
SRunner *sr;

typedef struct s_TestCaseInfoUt
{
  void   (*testFunc)(int);
  String100 testFuncName;
  String256 title;
}TestCaseInfoUt;


/* ##################################### FUNCTIONS ################################################################### */

#ifndef srunner_log
 #define srunner_log(sr, ...) logInfo( __VA_ARGS__ )
#endif
typedef struct packed
{
  UINT32 intVal;
  UINT8 charVal[4];
  UINT16 shortVal;
}__attribute__((packed))Packed;

BufferTracker *initBuffTrackerForTcase()
{
  BufferTracker *bufferTracker = NULL;
  bufferTracker = imcInitBufferTracker();

  return bufferTracker;
}

void freeBuffTrackerForTcase(BufferTracker *bufferTracker)
{
  imcFreeBuffer(bufferTracker);
}

/**
 * TC_BS_IMC_UT_001 - Test Case Number
 * @details Verify the addition of Data and Retrival of the same using getFromTop and GetFromBottom
 **/

UINT16 getTotalNodes(BufferTracker *bufferTracker)
{
  Node *node = bufferTracker->head;
  UINT16 nodes = 0;

  while(node){nodes += 1; node = node->next;}
  return nodes;
}


START_TEST(TC_BS_IMC_UT_001)
{
  BufferTracker *bufferTracker = NULL;
  String50 topStr, bottomStr;
  UINT8 *data="This is message ";

  bufferTracker = initBuffTrackerForTcase();

  clearBytes( topStr, sizeof(topStr) );
  clearBytes( bottomStr, sizeof(bottomStr) );

  /* Add Data at the TOP
   * Get data from TOP
   * Get the same data from bottom, now compare the data received from TOP and Bottom
   */
  imcAddAtTop(data, strlen(data), bufferTracker);
  imcGetBytesFromTop( strlen(data), bufferTracker, topStr);
  imcGetBytesFromBottom( strlen(data), bufferTracker, bottomStr);

  if( 0 NOTEQ strcmp(topStr, data ) )
  {
      fail("Read from TOP failed");
  }
  if( 0 NOTEQ strcmp(topStr, bottomStr) )
  {
        fail(" Read from TOP, Read from BOTTOM Mismatch");
  }

  freeBuffTrackerForTcase(bufferTracker);
}
END_TEST


/**
 * TC_BS_IMC_UT_002 - Test Case Number
 * @details Verify adding bytes at top
 **/
START_TEST(TC_BS_IMC_UT_002)
{
  BufferTracker *bufferTracker = NULL;
  String50  bottomRead, headerRead, totMsg;
  UINT8 *data= "This is message ";
  UINT8 *headerToAdd="Adding Header to Msg";

  bufferTracker = initBuffTrackerForTcase();
  clearBytes( bottomRead, sizeof(bottomRead) );
  clearBytes( headerRead, sizeof(headerRead) );
  clearBytes( totMsg, sizeof(totMsg) );

  imcAddAtTop( data, strlen(data), bufferTracker);
  imcAddAtTop( headerToAdd, strlen(headerToAdd), bufferTracker);

  /*Compare Added, Received Headers */
  imcGetBytesFromTop( strlen(headerToAdd), bufferTracker, headerRead);
  if( 0 NOTEQ strcmp( headerToAdd, headerRead))
  {
      fail("Header Added and the header Read are Different");
  }

  /*Compare Data, DataRead */
  imcGetBytesFromBottom( strlen(data), bufferTracker, bottomRead);

  if( 0 NOTEQ strcmp( data, bottomRead))
  {
      fail("Data Read from BOTTOM is different from Data Added");
  }
  freeBuffTrackerForTcase(bufferTracker);
}
END_TEST


/**
 * TC_BS_IMC_UT_003 - Test Case Number
 * @details Verify adding bytes at Bottom
 **/
START_TEST(TC_BS_IMC_UT_003)
{
  BufferTracker *bufferTracker = NULL;
  String50  bottomRead, headerRead, totMsg;
  UINT8 *data= "This is message ";
  UINT8 *tailToAdd="Adding Header to Msg";

  bufferTracker = initBuffTrackerForTcase();
  clearBytes( bottomRead, sizeof(bottomRead) );
  clearBytes( headerRead, sizeof(headerRead) );
  clearBytes( totMsg, sizeof(totMsg) );

  imcAddAtBottom( data, strlen(data), bufferTracker);
  imcAddAtBottom( tailToAdd, strlen(tailToAdd), bufferTracker);

  /*Compare Added, Received Headers */
  imcGetBytesFromBottom( strlen(tailToAdd), bufferTracker, bottomRead);
  if( 0 NOTEQ strcmp( tailToAdd, bottomRead))
  {
      fail("Header Added and the header Read are Different");
  }

  freeBuffTrackerForTcase(bufferTracker);
}
END_TEST
/**
 * TC_BS_IMC_UT_004 - Test Case Number
 * @details Verify length of total data in buffer and length value stored in list must be same
 *
 */
START_TEST(TC_BS_IMC_UT_004)
{
  BufferTracker *bufferTracker = NULL;
  String100  totMsg;
  UINT8 *data= "This is message ";
  UINT8 *headerToAdd="Adding Header to Msg";
  UINT8 *tailToAdd="Adding tail to Msg";

  bufferTracker = initBuffTrackerForTcase();
  clearBytes( totMsg, sizeof(totMsg) );

  imcAddAtTop( data, strlen(data), bufferTracker);
  imcAddAtTop( headerToAdd, strlen(headerToAdd), bufferTracker);
  imcAddAtBottom( tailToAdd, strlen(tailToAdd), bufferTracker);

  /*Compare Added, Received Headers */
  imcGetBytesFromTop( strlen(tailToAdd) + strlen(data) + strlen(headerToAdd), bufferTracker, totMsg);

  if(strlen(totMsg)/*(strlen(tailToAdd) + strlen(data) + strlen(headerToAdd))*/ NOTEQ imcGetTotalLen(bufferTracker))
  {
      fail("Total length and actual length are different");
  }

  freeBuffTrackerForTcase(bufferTracker);
}
END_TEST

/**
 * TC_BS_IMC_UT_005 - Test Case Number
 * @details Verify to write and read structure values containing values of mixed types
 *
 */
START_TEST(TC_BS_IMC_UT_005)
{
  BufferTracker *bufferTracker = NULL;
  Packed packedToWrite, packedReadTop, packedReadBottom;
  bufferTracker = initBuffTrackerForTcase();

  clearBytes(&packedToWrite, sizeof(Packed));
  clearBytes(&packedReadTop, sizeof(Packed));
  clearBytes(&packedReadBottom, sizeof(Packed));

  packedToWrite.intVal = 0x12345678;
  memcpy(&(packedToWrite.charVal), "sea", 3);
  packedToWrite.shortVal = 0x1234;

  imcAddAtTop( (UINT8 *)&packedToWrite, sizeof(Packed), bufferTracker);
  imcAddAtTop( (UINT8 *)&packedToWrite, sizeof(Packed), bufferTracker);

  /*Compare Added, Received Headers */
  imcGetBytesFromTop( sizeof(Packed), bufferTracker, (UINT8 *)&packedReadTop);
  imcGetBytesFromBottom( sizeof(Packed), bufferTracker, (UINT8 *)&packedReadBottom);

  if(0 NOTEQ memcmp(&packedReadTop, &packedReadBottom, sizeof(Packed)))
  {
      fail(" packedReadTop and packedReadBottom values are  mismatch");
  }

  freeBuffTrackerForTcase(bufferTracker);
}
END_TEST

/**
 * TC_BS_IMC_UT_006 - Test Case Number
 * @details Verify the creation multiple nodes , when the input data is greater than node buffer limit .
 *
 */
START_TEST(TC_BS_IMC_UT_006)
{
  BufferTracker *bufferTracker = NULL;
  UINT8 *data = "sdfghjkloasdfghjkloasdfghjkloasdfghjkloasdfghjkloasdfghjkloasdfghjkloasdfghjkloasdfghjkloasdfghjkloasdfghjkloasdfghjklo";
  UINT8 *header1 = "asdfg";
  UINT8 *header2 = "qwert";

  bufferTracker = initBuffTrackerForTcase();

  imcAddAtTop(data, strlen(data), bufferTracker);
  imcAddAtTop(header1, strlen(header1), bufferTracker);
  imcAddAtTop(header2, strlen(header2), bufferTracker);

   if((strlen(header2) + strlen(data) + strlen(header1)) NOTEQ imcGetTotalLen(bufferTracker))
  {
      fail("Lenghts are different");
  }

  freeBuffTrackerForTcase(bufferTracker);
}
END_TEST



/**
 * TC_BS_IMC_UT_007 - Test Case Number
 * @details Verify the Addtop and AddBottom functions one after another .
 *
 */
START_TEST(TC_BS_IMC_UT_007)
{
  BufferTracker *bufferTracker = NULL;

  UINT8 *data = "00000";
  UINT8 *header1 = "1111";
  UINT8 *header2 = "2222";
  UINT8 *tail1 = "3333";
  UINT8 *tail2 = "66666";
  UINT8 str[665];

  bufferTracker = initBuffTrackerForTcase();


  clearBytes(str, sizeof(str));

  imcAddAtTop(data, strlen(data), bufferTracker);
  imcAddAtBottom(header1, strlen(header1), bufferTracker);
  imcAddAtTop(header2, strlen(header2), bufferTracker);
  imcAddAtBottom(tail1, strlen(tail1), bufferTracker);
  imcAddAtBottom(tail2, strlen(tail2), bufferTracker);

  imcGetBytesFromTop(imcGetTotalLen(bufferTracker), bufferTracker, str);

  if((strlen(tail2)+ strlen(header2) + strlen(data) + strlen(header1)+strlen(tail1)) NOTEQ imcGetTotalLen(bufferTracker))
  {
    fail("Lengths are different");
  }

  freeBuffTrackerForTcase(bufferTracker);
}
END_TEST

/**
 * TC_BS_IMC_UT_008 - Test Case Number
 * @details Verify addModulo operation
 *
 */
START_TEST(TC_BS_IMC_UT_008)
{
  UINT16 result = 0;

  UINT32 x, y, n=4;

  printf("\n");
  for( x=0; x< n; x++)
  {
    for( y=0; y< n ;y++)
    {
      addModulo( x,y,n,&result);
       if( result > n )
       {
         fail("Modulo addition failed, for values X:%d, Y:%d", x,y);
       }
       printf("%d  ", result);
    }
    printf("\n");
  }

}
END_TEST

/**
 * TC_BS_IMC_UT_009 - Test Case Number
 * @details Verify subtractModulo operation
 *
 */
START_TEST(TC_BS_IMC_UT_009)
{

  UINT16 result = 0;

  UINT32 x, y, n=4;

   printf("\n");
   for( x=0; x< n; x++)
   {
     for( y=0; y<n ;y++)
     {
       subtractModulo( x,y,n,&result);
        if( result > n )
        {
          fail("Modulo Subtraction failed, for values X:%d, Y:%d", x,y);
        }
        printf("%d  ", result);
     }
     printf("\n");
   }

}
END_TEST

/**
 * TC_BS_IMC_UT_010 - Test Case Number
 * @details Verify subtractModulo operation when input values are greater than the modulo number .
 *
 */
START_TEST(TC_BS_IMC_UT_010)
{

  UINT16 result = 0;
  TBOOL retVal = TRUE;

  UINT32 x=5, y=7, n=4;

  retVal = subtractModulo( x,y,n,&result);
  if(retVal EQ FALSE)
  {
    printf("Test case success\n");
  }
  else
    fail("Test case failed");
}
END_TEST

/**
 * TC_BS_IMC_UT_011 - Test Case Number
 * @details Verify additionModulo operation when input values are greater than the modulo number .
 *
 */
START_TEST(TC_BS_IMC_UT_011)
{

  UINT16 result = 0;
  TBOOL retVal = TRUE;

  UINT32 x=5, y=7, n=4;

  retVal = addModulo( x,y,n,&result);
  if(retVal EQ FALSE)
  {
    printf("Test case success\n");
  }
  else
    fail("Test case failed");
}
END_TEST

TestCaseInfoUt utTestCaseTable [] = {

 { TC_BS_IMC_UT_001, "TC_BS_IMC_UT_001", "[Verify the addition of Data and Retrieval of the same using getFromTop and GetFromBottom]    " },
 { TC_BS_IMC_UT_002, "TC_BS_IMC_UT_002", "[Verify adding bytes at top]                                                                  " },
 { TC_BS_IMC_UT_003, "TC_BS_IMC_UT_003", "[Verify adding bytes at bottom]                                                               " },
 { TC_BS_IMC_UT_004, "TC_BS_IMC_UT_004", "[Verify length of total data in buffer and length value stored in list must be same]          " },
 { TC_BS_IMC_UT_005, "TC_BS_IMC_UT_005", "[Verify to write and read structure values containing values of mixed types]                  " },
 { TC_BS_IMC_UT_006, "TC_BS_IMC_UT_006", "[Verify the creation multiple nodes , when the input data is greater than node buffer limit ] " },
 { TC_BS_IMC_UT_007, "TC_BS_IMC_UT_007", "[Verify the Addtop and AddBottom functions one after another .]                               " },
 { TC_BS_IMC_UT_008, "TC_BS_IMC_UT_008", "[Verify addModulo operation]                               " },
 { TC_BS_IMC_UT_009, "TC_BS_IMC_UT_009", "[Verify subtractModulo operation]                               " },
 { TC_BS_IMC_UT_010, "TC_BS_IMC_UT_010", "[Verify subtractModulo operation when input values are greater than the modulo number .]      " },
 { TC_BS_IMC_UT_011, "TC_BS_IMC_UT_011", "[Verify addModulo operation when input values are greater than the modulo number .]           " },
 { NULL,   "NULL",   "NULL                                                                                                              " }

};


/* ************************************************************************** */
inline void addTestToUtTestCase(TCase *tc, TestCaseInfoUt *tcInfo)
  {
    _tcase_add_test (tc, tcInfo->testFunc, tcInfo->testFuncName, 0, 0, 0, 1);
  }

#define CK_DEFAULT_TIMEOUT  30

/* ************************************************************************** */
/* Test Suite */
Suite *imcTestSuite(TestCaseInfoUt *tcInfo)
{
  Suite *s;
  TCase *tc;

  s = suite_create ("** IMC Module Test Suite **");
  if (!tcInfo) /* Run all test cases in the mtTestCaseTable */
  {
    for (tcInfo = utTestCaseTable; tcInfo->testFunc NOTEQ NULL; tcInfo++)
    {
      tc = tcase_create (tcInfo->title);
      tcase_set_timeout (tc, CK_DEFAULT_TIMEOUT);
      addTestToUtTestCase (tc, tcInfo);
      suite_add_tcase (s, tc);
    }
  }
  else /* run the test case passed */
  {
      tc = tcase_create (tcInfo->title);
      tcase_set_timeout (tc, CK_DEFAULT_TIMEOUT);
      addTestToUtTestCase (tc, tcInfo);
      suite_add_tcase (s, tc);
  }
  return s;
}

int imc_ut_main(Suite *s)
{
  int num_failed;
  sr = srunner_create(s);
  srunner_set_xml (sr, "imc_ut_log.xml");

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all (sr, CK_VERBOSE);
  num_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (num_failed EQ 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
/* ************************************************************************** */
/** This function runs BSM MT  test cases  */
int main(int argc, char *argv[])
{
  UINT8 *tcName;
  TestCaseInfoUt *tcInfo;
  TBOOL found;

  if (argc EQ 1)
  {
    logInfo("%s", "Running all tests");
    imc_ut_main(imcTestSuite(NULL));
  }
  else
  {
    /* Run the test cases given in the command line */
    UINT8 i;
    for(i=1; i < argc; i++)
    {
      tcName = argv[i];
      found  = FALSE;
      for (tcInfo = utTestCaseTable; tcInfo->testFunc NOTEQ NULL; tcInfo++)
      {
        if(strcmp(&tcInfo->testFuncName[0], tcName) EQ 0)
        {
          found = TRUE;
          logInfo("Running test=%s", tcName);
          imc_ut_main(imcTestSuite(tcInfo));
          break;
        }
      }
      if (FALSE EQ found)
      {
        BS_ERROR_FUN_NOT_FOUND( tcName );
      }
    } /* for */
  } /* else */
  printAndEmptyErrorChain(&(getModuleContext()->errChain));
  return EXIT_SUCCESS;
}

/* ************************************************************************** */

