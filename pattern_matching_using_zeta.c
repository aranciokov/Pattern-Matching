#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "zeta.h"

void main(int argc, char** argv) {
  char *pattern, *text, *string, *subst;
  int *result, c, plen, slen, tlen, nocc;
  if (argc != 3)
    printf("\nusage: pm_z.exe \"pattern\" \"text_in_which_you_want_to_find_occurrencies_of_pattern\"");
  else {
    nocc = 0;
    
    pattern = argv[1];
    plen = strlen(pattern);
    
    text = argv[2];
    tlen = strlen(text);
    
    string = (char*) malloc ((plen + 1 + tlen + 1)*sizeof(char));
    strcpy(string, strcat(strcat(pattern, "$"), text));
    slen = strlen(string); // plen + tlen + 2

    result = zeta(string);
    
    
    /*subst = (char*) malloc(slen+1);
    
    for (c = 0; c < slen; c++) {
      strncpy(subst, string+c, result[c]);
      subst[result[c]] = '\0';
      if (result[c] > 0)
        printf("The substring %s, starting at %d, is a prefix of S of length %d\n", subst, c, result[c]);
      else
        printf("The substring %c.., starting at %d, is not a prefix of S\n", string[c], c);
    }
    free(subst);*/
    
    for (c = 0; c < slen; c++)
      if (result[c] == plen) {
        nocc++;
        printf("Occurrency at position %d\n", c-plen-1);
      }
    
    printf("Total number of occurrencies of the given pattern in the given text is %d", nocc);
    
    //free(pattern);
    //free(text);
    //free(string);
    //free(result);
  }
}
