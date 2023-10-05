#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spmat.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("No argument passed through command line!\n");
        return 1;
    }
    char *spfmt = argv[1];
    char *temp = argv[2];
    int nmults = atoi(temp);
    char *mmfile = argv[3];
    char *vecfilein = argv[4];
    char *vecfileout = argv[5];

    if (!strcmp(spfmt,"DEN")){
    	den(nmults,mmfile,vecfilein,vecfileout);
    }
    else if(!strcmp(spfmt,"COO")){
        coo(nmults,mmfile,vecfilein,vecfileout);
    }
    else if (!strcmp(spfmt,"CSR")){
        csr(nmults,mmfile,vecfilein,vecfileout);
    }
    else if (!strcmp(spfmt,"ELL")){
        ell(nmults,mmfile,vecfilein,vecfileout);
    }
    else if (!strcmp(spfmt,"JDS")){
        jds(nmults,mmfile,vecfilein,vecfileout);
    }
    else{
        printf("%s\n","Enter a valid Sparse Matrix Format"); 
    }
  
    return 0;
}
