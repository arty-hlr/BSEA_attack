#include "gdef.h"
#include "swrite.h"
#include "bbattery.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
  // swrite_Basic = FALSE;
  if(argc <= 2) {
    printf("Usage: ./FIPS [fileName] [size]\n");
    exit(0);
  }

  char *fileName = argv[1];
  int size = atoi(argv[2]);
  bbattery_RabbitFile (fileName, size);
  return 0;
}
