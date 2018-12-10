#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/random.h>
#include <time.h>

#define R0MASK 0x1FFFFFFF // 29 bits
#define R1MASK 0x000001FFFFFFFFFF // 41 bits
#define R2MASK 0x000007FFFFFFFFFF // 43 bits
#define R3MASK 0x0001FFFFFFFFFFFF // 49 bits

#define R0OUT  0x000000FF // bits 0 through 7 (the 8 lowest bits)
#define R1OUT  0x0000010000000000 // bit 40 (the high bit)
#define R2OUT  0x0000040000000000 // bit 42 (the high bit)
#define R3OUT  0x0001000000000000 // bit 48 (the high bit)
#define R0VECTOR 0xFF // bits 21 through 28

#define ONE 0x1

/*  Feedback taps to clock the shift registers.
 *  These correspond to the primitive polynomials:
 *  x^29 + x^22 + x^16 + x^15 + x^11 + x^3 + 1
 *  x^41 + x^40 + x^32 + x^20 + x^12 + x^11 + 1
 *  x^43 + x^35 + x^32 + x^30 + x^25  x^8 + 1
 *  x^49 + x^45 + x^42 + x^41 + x^39 + x^8 + 1
*/
#define R0TAPS 0x12023040 // bits 28, 25, 17, 13, 12, 6
#define R1TAPS 0x0000010030100101 // bits 40, 29, 28, 20, 8, 0
#define R2TAPS 0x0000040400021480 // bits 42, 34, 17, 12, 10, 7
#define R3TAPS 0x00010100000002C8 // bits 48, 40, 9, 7, 6, 3

#define BOOLFUNC1 0x9B
#define BOOLFUNC2 0xAE
#define BOOLFUNC3 0x2D

typedef unsigned char byte;
typedef uint32_t dword;
typedef uint64_t qword;
typedef byte bit;

void keyGen(char *keyFileName);
void regSetup(char *keyFileName, dword *R0, qword *R1, qword *R2, qword *R3);
byte getBits(qword R1, qword R2, qword R3);
void clockAll(qword *R1, qword *R2, qword *R3);

byte getpfromR0(dword R0);
void clockR0p(dword *R0, byte p);
byte get8bitVector(dword R0);

void XORboolFunc(byte *boolFunc, byte toXOR);
bit outputBoolFunc(byte boolFunc, byte input);
