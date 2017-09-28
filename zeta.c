#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zeta.h"

void example_zeta(int argc, char** argv) {
  int *z;
  char *input, *subst;
  int c;
  if (argc != 2)
    printf("usage: \nzeta.exe \"somethingsomething\"");
  else {
    input = argv[1];
    z = zeta (input);
    subst = (char*) malloc(strlen(input)+1);
    for (c = 0; c < strlen(input); c++) {
      strncpy(subst, input+c, z[c]);
      subst[z[c]] = '\0';
      if (z[c] > 0)
        printf("The substring %s, starting at %d, is a prefix of S of length %d\n", subst, c, z[c]);
      else
        printf("The substring %c.., starting at %d, is not a prefix of S\n", input[c], c);
    }
    free(subst);
    free(input);
    free(z);
  }
}

/**
 * @param s: a string on which the Z algorithm will be applied
 * @return : a vector of integers representing, for each i in 1..|s|, the maximum length of a prefix of S which can be read starting at position i
 */
int* zeta(char* s) {
  int slen, k, l, r, i, j;
  int *zeta;
  /* 
   * basis: zeta[1]=|s| because from the starting position the whole string is a prefix of itself
   */
  slen = strlen(s);
  zeta = malloc(slen*sizeof(int));
  zeta[0] = slen;
  
  /* l and r are the indexes used to manage the max z-block */
  l = -1;
  r = -1;
  
  /*
   * for each i in 1..|s|, 
   * - first, we need to understand where i is positioned w.r.t. to the z-block
   * - then, based on the previous answer, a case-by-case reasoning is applied
   */
  for (i = 1; i < slen; i++) {
    j = 0;
    /*
     * if i > r, it means that there is no prefix-substring of S which starts before i and ends at or after i
     * thus, we compute a new [l, r] z-block with the naive method (char-by-char checking between S[0..] and S[i..])
     * once we are done, we know that Z_i(S)=j (because that's how far we can go and still have S[0..j]==S[i..i+j]), from which we can also determine l and r
     */
    if (i > r) {
      while (j < slen && s[j] == s[i+j]) j++;
      zeta[i] = j;
      l = i;
      r = i + j;
    }
    /*
     * otherwise i <= r, thus we know that there is a prefix-substring of S which starts before i and ends at or after i
     * let k = i - l; we do need this because [l, r] interval is the max z-block found until i-1 and we thus know that S[l..r]==S[0..r-l] (by definition on what Z does) and thus S[l..i]==S[0..k], so to move r forward (and thus move the z-block) it must be that k+Z_k(S) >= r-l
     * if k + Z_k(S) < r - l, it means that there is not a longer prefix-substring of S starting at position i, so Z_i(S) is simply Z_k(S) (because of the previously made reasoning)
     * otherwise if k+Z_k(S) >= r-l, we can move the z-block using a similar reasoning to that applied to the "i>r" case 
     */
    else if (l <= i && i <= r) {
      k = i - l;
      if (k + zeta[k] < r - l)
        zeta[i] = zeta[k];
      else {
        while (r+j-1 < slen && s[r+j-1] == s[r-i+j-1]) j++;
        zeta[i] = j;
        l = i;
        r = i + j;
      }
    }
    else printf("This won't happen: to be here, both i>r and (l <= i && i <= r) must be false");
  }
  
  return zeta;
}
