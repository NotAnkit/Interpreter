#include <stdio.h>
#include <string.h>

#include "scanner.h"
#include "memory.h"
#include "core.h"
#include "nodes.h"
#include "parser.h"
#include "executer.h"
#include "free.h"
#include "printer.h"

int main(int argc, char *argv[]) {  
  
  scanner_open(argv[1]);

  struct nodeProcedure p = {};
  stack_init();

  //Parse the tokens
  parseProcedure(&p);

  //execute the tree
  executeProcedure(&p, argv[2]);

  //Print Tree Used to check the parse
  //printProcedure(&p);

  //free the tree
  freeProcedure(&p);

  scanner_close();

  return 0;
}