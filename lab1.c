/*******************************************************************************
  * File Name          : lab1.c
  * Description        : Prints the name of lab partners
  * Author:            : Jeevan Dsouza, Dushant Choudhary
  * Date:              : 11-09-2021				 
  ******************************************************************************
  */
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"


ParserReturnVal_t CmdTest2(int action)
{

  if(action==CMD_SHORT_HELP) return CmdReturnOk;
  if(action==CMD_LONG_HELP) {
    printf("testasm\n\n"
	   "This command tests the C to assembler interface\n"
	   );

    return CmdReturnOk;
  }

printf("Jeevan and Dushant lab 1 \n");

  return CmdReturnOk;
}

ADD_CMD("lab1",CmdTest2,"lab 1")



