#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "spmat.h"

void csr(int nmults, char *mmfile, char *vecfilein, char *vecfileout) {

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

    for (int i = 0; i < nz - 1; i++) {
        for (int j = i + 1; j < nz; j++) {
            if (row[i] > row[j]) {
                int temp_r = row[i];
                row[i] = row[j];
                row[j] = temp_r;

                int temp_c = col[i];
                col[i] = col[j];
                col[j] = temp_c;

                double temp_m = mat[i];
                mat[i] = mat[j];
                mat[j] = temp_m;
            }
        }
    }

    cnt=1;
    int nrows=1, rowIdx=0, tempi;
    int rowPtrs[n+1];
    memset(rowPtrs, 0, (n+1)*sizeof(int));
    for (int i=1;i<nz;i++){
        tempi = rowIdx;
	rowIdx = row[i];
	if (rowIdx==tempi)
	   nrows++;
	else {
	   rowPtrs[cnt]=rowPtrs[cnt-1]+nrows;
	   cnt++;
	   nrows=1;
	}
    }
    rowPtrs[cnt]=rowPtrs[cnt-1]+nrows;

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
        for (int i=1; i<n+1; i++){
	     int strt = rowPtrs[i-1];
	     int end = rowPtrs[i];
	     for (int j=0;j<(end-strt); j++)
                  vecout[i-1] +=  mat[strt+j]*vecin[col[strt+j]];
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

    file = fopen("matsave.txt", "w");
    for (int i=0; i<n+1; i++)
        fprintf(file, "%d\n", rowPtrs[i]);
    for (int i=0; i<nz; i++)
        fprintf(file, "%d %20.19g\n", col[i], mat[i]);

    fclose(file);
}
