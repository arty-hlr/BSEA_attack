#include "backdoored.h"

int main(int argc, char *argv[]) {
  char *keyFileName = argv[1];
  char *outputFileName = argv[2];
  int numberOfBytes = atoi(argv[3]);
  clock_t start, end;
  double cpu_time_used;


  dword R0 = 0;
  qword R1=0, R2=0, R3=0;
  keyGen(keyFileName);
  regSetup(keyFileName, &R0, &R1, &R2, &R3);
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
