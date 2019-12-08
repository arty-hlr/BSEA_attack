# BSEA attack

From research done with Eric Filiol at ESIEA on stream ciphers, see paper in repository

# TO DO

+ Update code and presentation with latest algorithm
+ Implement attack from paper

# Presentation of the old algorithm

Here is a short presentation of the algorithm as well as its implementation.

There are 4 LFSR with different lengths: R0 is 29 bits long, R1 is 41 bits long, R2 is 43 bits long, and R3 is 49 bits long. There is also a boolean function that gets modified by the output of R0 as we will describe later and takes the output of R1, R2, and R3 as parameters to give the output bit of the algorithm at each tick of the clock. This function is described by its truth table represented as a byte in hex, like so: 9B, AE, or 2D.

As R0 is used to control the other registers and to modify the boolean function, let us call its bits 'b0 b1 ... b27 b28' from left to right, assuming that the registers shift to the left.

At each tick of the clock, several steps are executed, which decide how the registers advance:
1) Compute p = 1 + b0 + 2*b1
2) Advance R0 for p steps
3) Get the 8 right-most bits of R0, so (MSB) b21 ... b28 (LSB) to form a byte and XOR it with the truth table of the boolean function
4) Get x1, x2, x3, the left-most bits of R1, R2, R3, and advance R1, R2, R3 for 1 step.
5) Output f_t(x3, x2, x1), f_t being the boolean function at time t.

Below are the polynomials for each register, which are primitive:
R0: x^29 + x^22 + x^16 + x^15 + x^11 + x^3 + 1
R1: x^41 + x^40 + x^32 + x^20 + x^12 + x^11 + 1
R2: x^43 + x^35 + x^32 + x^30 + x^25  x^8 + 1
R3: x^49 + x^45 + x^42 + x^41 + x^39 + x^8 + 1

In order to understand how the algorithm works, you will find an example of a full tick's actions with random registers here:

The initial boolean function will be 9B, and the registers will be initialized as follows:
    b0                                                      b28
R0: |1|1|1|0|1|0|1|1|0|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|
R1: |0|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|
R2: |0|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|
R3: |1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|

The boolean function is better represented vertically, with the LSB at the top and the MSB at the bottom:

9B
--
 1  (LSB)
 1
 0
 1
 1
 0
 0
 1  (MSB)

And we can begin our steps:

1) From the right-most bits of R0, we get p = 1 + b0 + 2*b1 = 1 + 1 + 2*1 = 4

2) We then advance R0 for 4 steps, giving this updated register with the help of the polynomial:
x^29 + x^22 + x^16 + x^15 + x^11 + x^3 + 1 means we will XOR (counting from the left) the 22nd, the 16th, the 15th, the 11th, the 3rd, and the 0th bit of R0, marked by an asterisk, to get the new bit, then shift left, and add that new bit at the right-most place, marked by a plus sign.
                    b0                                                      b28
                     *     *               *       * *           *            
R0        (before): |1|1|1|0|1|0|1|1|0|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|
R0  (after 1 step): |1|1|0|1|0|1|1|0|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|
                                                                             +
After 3 more steps, we end up with the following R0:
                    b0                                                      b28
R0 (after 4 steps): |1|0|1|1|0|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|0|0|1|

3) We then take the bits b21 through b28 of R0, which will give the byte 0xF9, and XOR it with the boolean function as so ('+' representing addition in the finite field F_2):

9B + F9 = 62
------------
 1 +  1 =  0  (LSB, b28)
 1 +  0 =  1
 0 +  0 =  0
 1 +  1 =  0
 1 +  1 =  0
 0 +  1 =  1
 0 +  1 =  1
 1 +  1 =  0  (MSB, b21)

4) We then get the left-most bits of R1, R2, and R3: x1 = 0, x2 = 0, x3 = 1 and we advance R1, R2, R3 for one step, giving these updated registers using the polynomials to get the new bits of each register, shifting them to the left, and adding the new bits on the right:

R1 (after step): |1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|
R2 (after step): |1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|
R3 (after step): |1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|0|

5) The last step is to output the actual output bit, given by the boolean function following its truth table given here:

x3 x2 x1 | 62
-------------
 0  0  0 |  0
 0  0  1 |  1
 0  1  0 |  0
 0  1  1 |  0
 1  0  0 |  0 *
 1  0  1 |  1
 1  1  0 |  1
 1  1  1 |  0

Checking the values of x3, x2, and x1 we have, we should look at the line marked by an asterisk and output 0.



Well, I hope that was clear enough, if not, I'd advice having a look at the chapter about stream cipher from a good crypto book which will help you understand what happens with the LFSRs.


The implementation is pretty straightforward and quite readable, apart maybe from the load function which loads a key from a file and is quite a mess, but the rest is understandable I hope.