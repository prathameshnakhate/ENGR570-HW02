#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "spmat.h"

void coo(int nmults, char *mmfile, char *vecfilein, char *vecfileout) {
  
    int nz,n;

    mtx2txt(mmfile,&n,&nz);

    FILE *file;
    char *filename = "output.txt"; 
    double mat[nz];
    int row[nz], col[nz];
    int rtemp, ctemp, cnt=0;
    double value;

    file = fopen(filename, "r");
    while (fscanf(file, "%d %d %lf", &rtemp, &ctemp, &value) == 3) {
        if (rtemp < n+1 && ctemp < n+1) {
            mat[cnt] = value;
	    row[cnt] = rtemp-1;
	    col[cnt] = ctemp-1;
	    cnt++;
        }
    }
    fclose(file);
    remove(filename);

    double vecin[n];
    cnt = 0;
    file = fopen(vecfilein, "r");
    while (fscanf(file, "%lf", &value) == 1) {
        if (cnt < n) {
            vecin[cnt] = value;
        }
	cnt++;
    }
    fclose(file);

    double vecout[n];

    clock_t tic = clock();

    for (int m=0; m<nmults; m++){
	memset(vecout, 0, n*sizeof(double));
        for (int i=0; i<nz; i++){
             vecout[row[i]] +=  mat[i]*vecin[col[i]];
        }
	for (int i=0; i<n; i++)
	    vecin[i]=vecout[i];
    }

    clock_t toc = clock();
    double time = ((double)(toc-tic))/CLOCKS_PER_SEC;
    printf("%.5f\n",time);

    file = fopen(vecfileout, "w");
    for (int i=0; i<n; i++)
        fprintf(file, "%20.19g\n", vecout[i]);

    fclose(file);

    /*file = fopen("matsave.txt", "w");
    for (int i=0; i<nz; i++)
        fprintf(file, "%d %d %20.19g\n", row[i], col[i], mat[i]);

    fclose(file);*/
}
