#include "backdoored.h"

int main(int argc, char *argv[]) {
  char *mode = argv[1];
  char *keyFileName = argv[2];
  char *outputFileName = argv[3];
  int numberOfBytes = atoi(argv[4]);
  clock_t start, end;
  double cpu_time_used;

  if(argc <= 4) {
    printf("Usage: [mode -c to create or -u to use a keyfile] [keyFileName or -1] [outputFileName] [size]\n");
    exit(0);
  }

  dword R0 = 0;
  qword R1=0;
  qword R2=0;
  qword R3=0;
  if (atoi(argv[2]) == -1) {
    R0 = -1 & R0MASK;
    R1 = -1 & R1MASK;
    R2 = -1 & R2MASK;
    R3 = -1 & R3MASK;
  }
  else {
    if(argv[1][1] == 'c')
      keyGen(keyFileName);
    regSetup(keyFileName, &R0, &R1, &R2, &R3);
  }

  byte boolFunc = BOOLFUNC1;

  FILE *filePtr = fopen(outputFileName, "wb");
  int i = 0;
  int j = 0;
  byte p = 0;
  byte toXOR = 0;
  byte input = 0;
  bit outputBit = 0;
  byte output = 0;

  start = clock();
  while(i <= numberOfBytes) {
    p = getpfromR0(R0);
    // printf("[DEBUG] p is 0x%x\n", p);
    clockR0p(&R0, p);
    // printf("[DEBUG] R0 is 0x%x\n", R0);
    toXOR = get8bitVector(R0);
    XORboolFunc(&boolFunc, toXOR);
    input = getBits(R1, R2, R3);
    clockAll(&R1, &R2, &R3);
    outputBit = outputBoolFunc(boolFunc, input);

    if(j == 8) {
      j = 0;
      fwrite(&output, sizeof(byte), 1, filePtr);
      output = 0;
    }

    output <<= 1;
    output |= outputBit;
    j++;
    i++;
  }
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  // printf("Time taken by the CPU to write %d bytes is %f\n", numberOfBytes, cpu_time_used);
  FILE *timeFilePtr = fopen("./tests/times", "a");
  fprintf(timeFilePtr, "%f\n", cpu_time_used);
  fclose(timeFilePtr);
}
