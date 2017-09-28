#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sp.h"

void kmp (char*, char*);

void main (int argc, char** argv) {
  char *pat, *text;
  if (argc != 3) {
    printf ("usage: kmp \"pattern\" \"text_in_which_to_find_occurrencies_of_such_pattern\"\n");
  }
  else {
    pat = argv[1];
    text = argv[2];
    kmp(pat, text);
  }
}

/**
 * Knuth-Morris-Pratt algorithm using the sp vector. Given a pattern P and a text T, the aim is to find all occurrencies of P in T.
 * To do so, it uses the sp vector, which is an integers vector where sp[i]=k is the length of the longest suffix of P[1..i] (i.e. ending at position i) which also appears as a prefix of P.
 * Using the information given by such vector, KMP improves the naive algorithm.
 * In the naive version, when a mismatch is found (situation: P[i]=x != y=T[j+i]), it simply shifts the pattern by 1 position and starts checking the pattern and the new substring (i.e. it increments j and starts comparing P[1..] and T[j+1..]), not using the information given by the fact that P[1..i-1] and T[j..j+i-1] were the same (since there were no mismatches upto the position i).
 * KMP uses such information: when a mismatch is found (situation: P[i]=x != y=T[j+i]), it acknowledges the fact that P[1..i-1]==T[j..j+i-1] and it queries the sp vector at position i-1, getting the length of the longest suffix of P[1..i-1] which is also a prefix of P. In this way, it knows that P[1..sp[i-1]-1] will match with T[j+i-sp[i-1]..j+i-1], thus it can skip those checks (they are guaranteed to match thanks to sp) and it can focus on checking P[sp[i-1]] and T[j+i]
 *
 * Some ascii art below
 *  T: ...-------++---++*---....
 *  P:           ++---++#
 *                      ^mismatch
 *                     ^sp[i-1]=k  , and 
 *                ^k
 * Thus the following is the situation after the shift related to sp 
 *  T: ...-------++---++*---....
 *  P:                ++---++#
 *
 * @param pattern: pattern of which occurrencies are to be found inside the text
 * @param text: text in which occurrencies of pattern are to be found
 */
void kmp (char* pattern, char* text) {
  int plen, tlen, p, t, f, *sp_;
  plen = strlen(pattern);
  tlen = strlen(text);
  p = 0;
  f = 0;
  sp_ = sp(pattern);
  
  while(f + plen - p <= tlen) {
    /* mismatch case
     *            p:  v
     * P:        ABCABD
     * T: AAA...AABCABE...
     *          f^    ^f+p
     * In this case I know I can move the f position by p-sp[p]
     * The sp vector for ABCABD is [0 0 0 1 2 0] which says that P[1..5] has a suffix of length 2 which is also a prefix
     * Thus, first I shift right (+p) the pattern by p positions, getting
     *                 p:  v 
     * P:             ABCABD
     * T: AAA...AABCABE...
     *               f^    ^f+p
     * And then shifting left (-sp[p]) to avoid losing the "AB" occurrency in the text, getting 
     *               p:  v 
     * P:           ABCABD
     * T: AAA...AABCABE...
     *             f^    ^f+p
     * Finally, to be able to compare text and pattern, I would set p again to 0
     *          p:  v 
     * P:           ABCABD
     * T: AAA...AABCABE...
     *             f^
     * But now I already know that a prefix of length 2 (sp[p], with the previous p) will match, thus setting to 0 would be useless,
     * So I set p to sp[p], avoiding doing the extra computation
     *            p:  v 
     * P:           ABCABD
     * T: AAA...AABCABE...
     *             f^ ^f+p
     */
    if (text[f+p] != pattern[p]) {
      if (p > 0) {
        f = f + p - sp_[p-1];
        if (p > 0) p = sp_[p-1];
      }
      /* 
       * Special case to avoid loops when the mismatch is at the beginning of P
       * In such a case, sp[p] is 0 because sp is computed w.r.t. to *proper* suffixes
       * Thus, both p and sp[p] would be 0, and we'd enter an endless loop
       */
      else f++;
    }
    /* non-mismatch case 
     *            p: v
     * P:        ABCABD
     * T: AAA...AABCABE...
     *          f^   ^f+p
     * That's neat, we simply move forward (p++)
     */
    else {
      p++;
      /* match case (after the p++)
       *            p:   v
       * P:        ABCABD
       * T: AAA...AABCABD...
       *          f^     ^f+p
       * We found that T[f..f+p]==P[1..p]==P (and p==plen)
       * After printing the occurrency, what to do?
       * We set p=0 and we increment f, in order to start the search of the next occurrency
       *       p:   v
       * P:         ABCABD
       * T: AAA...AABCABD...
       *           f^
       */
      if (p == plen) {
        printf ("Occurrency found at %d\n", f);
        p = 0;
        f++;
      }
    }
  }
}