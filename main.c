#include "backdoored.h"

int main(int argc, char *argv[]) {
  if(argc <= 4) {
    printf("Usage: [mode -c to initialize the registers to -1 (0b111...111) or -u to use a keyfile] [-1 or keyFileName] [outputFileName] [size]\n");
    exit(0);
  }

  char *mode = argv[1];
  char *keyFileName = argv[2];
  char *outputFileName = argv[3];
  int numberOfBytes = atoi(argv[4]);
  clock_t start, end;
  double cpu_time_used;

  dword R0 = 0;
  qword R1 = 0;
  qword R2 = 0;
  qword R3 = 0;
  if (atoi(argv[2]) == -1) {
	// Initialize all the reisters to -1 (0b111...111)
    R0 = -1 & R0MASK;
    R1 = -1 & R1MASK;
    R2 = -1 & R2MASK;
    R3 = -1 & R3MASK;
  }
  else {
	// Generate a random keyfile and use it to initialize the registers
    if(argv[1][1] == 'c')
      keyGen(keyFileName);
    regSetup(keyFileName, &R0, &R1, &R2, &R3);
  }

  // There are 3 boolean functions to choose from, see the .h
  byte boolFunc = BOOLFUNC1;

  FILE *filePtr = fopen(outputFileName, "wb");
  byte p = 0;
  byte toXOR = 0;
  byte input = 0;
  bit outputBit = 0;
  byte output = 0;

  start = clock();
  for(int i = 0; i <= numberOfBytes; i++) {
    if(((i%8) == 0) && (i != 0)) {
      fwrite(&output, sizeof(byte), 1, filePtr);
      output = 0;
    }

    p = getpfromR0(R0);
    clockR0p(&R0, p);
    toXOR = get8bitVector(R0);
    XORboolFunc(&boolFunc, toXOR);
    input = getBits(R1, R2, R3);
    clockAll(&R1, &R2, &R3);
    outputBit = outputBoolFunc(boolFunc, input);
    output |= (outputBit << (7-(i%8)));
  }

  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  FILE *timeFilePtr = fopen("./tests/times", "a");
  fprintf(timeFilePtr, "%f\n", cpu_time_used);
  fclose(timeFilePtr);
}
