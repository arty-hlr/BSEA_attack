#include "backdoored.h"

// Generate a random 162-bits key and write it to keyFileName
void keyGen(char *keyFileName) {
  byte key[21] = {0};
  getrandom(&key, 21, 0);
  key[20] &= 0xFC;
  FILE *filePtr = fopen(keyFileName, "wb");
  fwrite(key, sizeof(byte), 21, filePtr);
  fclose(filePtr);
}

// Initialize the registers with the key stored in binary in keyFileName
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
      (*R3) <<= 1;
      j++;
    }
  }
  off = j;
  *R0 &= R0MASK;
  *R1 &= R1MASK;
  *R2 &= R2MASK;
  *R3 &= R3MASK;
}

// Get x1, x2, and x3, the left-most bits of R1, R2, and R3
byte getBits(qword R1, qword R2, qword R3) {
  byte out = 0;
  out = __builtin_parityll(R3 & R3OUT);
  out <<= 1;
  out |= __builtin_parityll(R2 & R2OUT);
  out <<= 1;
  out |= __builtin_parityll(R1 & R1OUT);
  return(out);
}

// Clock R1, R2, and R3 once each
void clockAll(qword *R1, qword *R2, qword *R3) {
  bit tappedR1 = __builtin_parityll((*R1) & R1TAPS);
  bit tappedR2 = __builtin_parityll((*R2) & R2TAPS);
  bit tappedR3 = __builtin_parityll((*R3) & R3TAPS);
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

// Compute p from b0 and b1 of R0
byte getpfromR0(dword R0) {
  byte p = 0;
  p += R0 >> 28;
  p += 2*((R0 >> 27) & ONE);
  p++;
  return(p);
}

// Clock the R0 register p times
void clockR0p(dword *R0, byte p) {
  bit tappedR0 = 0;
  int i = 0;
  do {
    tappedR0 = __builtin_parityll((*R0) & R0TAPS);
    (*R0) <<= 1;
    (*R0) &= R0MASK;
    (*R0) |= tappedR0;
    i++;
  } while (i < p);
}

// Return the 8 bit vector b21...b28
byte get8bitVector(dword R0) {
  return(R0 & R0VECTOR);
}

// XOR the 8 bit vector b21...b28 to the boolean function
void XORboolFunc(byte *boolFunc, byte toXOR) {
  (*boolFunc) ^= toXOR;
}

// Output the input-th bit of the boolean function
bit outputBoolFunc(byte boolFunc, byte input) {
  return(__builtin_parityll(boolFunc & (ONE << input)));
}
