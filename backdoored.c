#include "backdoored.h"

void keyGen(char *keyFileName) {
  byte key[21] = {0};
  getrandom(&key, 21, 0);
  key[20] &= 0xF0;
  FILE *filePtr = fopen(keyFileName, "wb");
  fwrite(key, sizeof(byte), 21, filePtr);
  fclose(filePtr);
}

void regSetup(char *keyFileName, dword *R0, qword *R1, qword *R2, qword *R3) {
  FILE *filePtr = fopen(keyFileName, "rb");

  byte l = 0;
  byte j = 0;
  byte off = 0;
  byte read = 0;

  while(l < 29) {
    if(off) {
      j = off;
      off = 0;
    }
    else {
      j = 0;
      fread(&read, sizeof(byte), 1, filePtr);
    }
    while(j < 8) {
      (*R0) |= (ONE & (read >> j));
      if(++l >= 29)
        break;
      // printf("[DEBUG] R0 is 0x%lx\n", *R0);
      (*R0) <<= 1;
      j++;
    }
  }
  off = j;
  l = 0;

  while(l < 41) {
    if(off) {
      j = off;
      off = 0;
    }
    else {
      j = 0;
      fread(&read, sizeof(byte), 1, filePtr);
    }
    while(j < 8) {
      (*R1) |= (ONE & (read >> j));
      if(++l >= 41)
        break;
      // printf("[DEBUG] R1 is 0x%lx\n", *R1);
      (*R1) <<= 1;
      j++;
    }
  }
  off = j;
  l = 0;

  while(l < 43) {
    if(off) {
      j = off;
      off = 0;
    }
    else {
      j = 0;
      fread(&read, sizeof(byte), 1, filePtr);
    }
    while(j < 8) {
      (*R2) |= (ONE & (read >> j));
      if(++l >= 43)
        break;
      // printf("[DEBUG] R2 is 0x%lx\n", *R2);
      (*R2) <<= 1;
      j++;
    }
  }
  off = j;
  l = 0;

  while(l < 49) {
    if(off) {
      j = off;
      off = 0;
    }
    else {
      j = 0;
      fread(&read, sizeof(byte), 1, filePtr);
    }
    while(j < 8) {
      (*R3) |= (ONE & (read >> j));
      if(++l >= 49)
        break;
      // printf("[DEBUG] R3 is 0x%lx\n", *R3);
      (*R3) <<= 1;
      j++;
    }
  }
  off = j;
  *R0 &= R0MASK;
  *R1 &= R1MASK;
  *R2 &= R2MASK;
  *R3 &= R3MASK;
  // printf("[DEBUG] State of the registers: 0x%lx, 0x%lx, 0x%lx, 0x%lx\n", *R0, *R1, *R2, *R3);
}

bit parity(qword R) {
	R ^= R>>32;
	R ^= R>>16;
	R ^= R>>8;
	R ^= R>>4;
	R ^= R>>2;
	R ^= R>>1;
	return(R&1);
}

byte getBits(qword R1, qword R2, qword R3) {
  byte out = 0;
  out = parity(R3 & R3OUT);
  out <<= 1;
  out |= parity(R2 & R2OUT);
  out <<= 1;
  out |= parity(R1 & R1OUT);
  return(out);
}

void clockAll(qword *R1, qword *R2, qword *R3) {
  bit tappedR1 = parity((*R1) & R1TAPS);
  bit tappedR2 = parity((*R2) & R2TAPS);
  bit tappedR3 = parity((*R3) & R3TAPS);
  (*R1) <<= 1;
  (*R1) &= R1MASK;
  (*R1) |= tappedR1;
  (*R2) <<= 1;
  (*R2) &= R2MASK;
  (*R2) |= tappedR2;
  (*R3) <<= 1;
  (*R3) &= R3MASK;
  (*R3) |= tappedR3;
}

byte getpfromR0(dword R0) {
  return((R0 >> 27) + 1);
}

void clockR0p(dword *R0, byte p) {
  bit tappedR0 = 0;
  int i = 0;
  do {
    tappedR0 = parity((*R0) & R0TAPS);
    (*R0) <<= 1;
    (*R0) &= R0MASK;
    (*R0) |= tappedR0;
    i++;
  } while (i < p);
}

byte get8bitVector(dword R0) {
  return(R0 & R0VECTOR);
}

void XORboolFunc(byte *boolFunc, byte toXOR) {
  (*boolFunc) ^= toXOR;
}

bit outputBoolFunc(byte boolFunc, byte input) {
  return((boolFunc >> input) & ONE);
}