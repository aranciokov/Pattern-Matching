#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zeta.h"
#include "sp.h"

int max(int, int);

void example_sp(int argc, char** argv) {
  char *str;
  int slen, *spp, *sp_, j;
  str = argv[1];
  slen = strlen(str);
  spp = sp_prime(str);
  printf("sp' = \n");
  for (j = 0; j < slen; j++)
    printf("%d => %d\n", j, spp[j]);
  
  sp_ = sp(str);
  printf("\nsp = \n");
  for (j = 0; j < slen; j++)
    printf("%d => %d\n", j, sp_[j]);
  
  free(spp);
  free(sp_);
}

/**
 * This function is used to get the sp' vector of the pattern P. In it, each cell sp'[i] represents the length of the longest suffix ending at position i which is also a prefix of P (of length sp'[i]). Moreover, it is known that the next character (at position i+1) differs from the next character of the pattern (at position sp'[i]+1) (i.e. let sp'[i]=k; P[1..k]==P[i-k..i] but P[k+1] != P[i+1]). This optimization allows faster shifting during the KMP algorithm.
 * @param pattern: the pattern string P on which I am computing the sp vector
 * @return spp: an integers vector, spp[i]=k is the length of the longest prefix of P which can be read from position i-k to position i (i.e. P[i-k..i] is the longest suffix of P[1..i] which is also a prefix of P). Moreover, spp[i]=k implies that P[i+1] != P[k+1].
 */
int* sp_prime(char* pattern) {
  int *spp, *z, i, j, slen;
  z = zeta(pattern);
  slen = strlen(pattern);
  
  spp = malloc(slen * sizeof(int));
  for (j = 0; j < slen; j++) spp[j] = 0;
  
  for (j = slen -1; j >= 1; j--) {
    /* 
     * Z[j] =k is the length of the longest prefix of the pattern P which can be read starting at position j (i.e. P[j..j+k], thus a suffix of P[1..j+k] of length k)
     * thus (i=) j+z[j] is the index of the last character of such prefix/suffix
     * sp[i]=? is the length of the longest suffix of P[1..i] which also matches a prefix of P
     * such ? is clearly Z[j]
     */
    i = j + z[j] -1;
    spp[i] = z[j];
    //printf("i=%d j=%d z[j]=%d spp[i]=%d \n",i,j,z[j],spp[i]);
  }
  
  /* avoiding some memory leak */
  free(z);
  
  return spp;
}

/**
 * This function is used to get the sp vector of the pattern P. In it, each cell sp[i] represents the length of the longest suffix ending at position i which is also a prefix of P (of length sp[i]). In this case, it is not guaranteed that P[sp[i]+1] != P[i+1], which is what happens in the optimized sp' version.
 * @param pattern: the pattern string P on which I am computing the sp vector
 * @return spp: an integers vector, spp[i]=k is the length of the longest prefix of P which can be read from position i-k to position i (i.e. P[i-k..i] is the longest suffix of P[1..i] which is also a prefix of P).
 */
int* sp(char* pattern) {
  int *spp, *sp, *z, i, j, slen;
  z = zeta(pattern);
  slen = strlen(pattern);
  
  spp = sp_prime(pattern);
  sp = malloc(slen * sizeof(int));
  
  for (j = 0; j < slen; j++)
    sp[j] = 0;
  
  sp[slen-1] = spp[slen-1];
  for (i = slen -2; i >= 1; i--) 
    sp[i] = max(sp[i+1] -1, spp[i]);
  
  /* avoiding some memory leak */
  free(z);
  free(spp);
  
  return sp;
}

int max(int a, int b) {
  return (a >= b ? a : b);
}