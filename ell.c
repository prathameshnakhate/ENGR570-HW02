#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "spmat.h"

void ell(int nmults, char *mmfile, char *vecfilein, char *vecfileout) {
   
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

    int nrows=1, rowIdx=0, tempi, nrowsmax=0;
    for (int i=1;i<nz;i++){
        tempi = rowIdx;
	rowIdx = row[i];
	if (rowIdx==tempi)
	   nrows++;
	else {
           if (nrows>nrowsmax)
	       nrowsmax=nrows;
	   nrows=1;
	}
    }

    int colell[n][nrowsmax];
    double matell[n][nrowsmax];
    cnt=0;
    for (int i=0;i<n;i++){
        for (int j=0;j<nrowsmax;j++){
	    if (row[cnt]==i){
	       colell[i][j]=col[cnt];
	       matell[i][j]=mat[cnt];
	       cnt++;
	    }
	    else{
	       colell[i][j]= -1;
	       matell[i][j]= -1;
	    }
	}
    }

    int colellf[n*nrowsmax];
    double matellf[n*nrowsmax];
    for (int j=0;j<nrowsmax;j++){
        for (int i=0;i<n;i++){
	    colellf[j*n+i]=colell[i][j];
	    matellf[j*n+i]=matell[i][j];
	}
    }
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
        for (int i=0; i<n; i++){
	    for (int j=0; j<nrowsmax; j++){
		if (colellf[j*n+i] != -1)
                   vecout[i] += matellf[j*n+i]*vecin[colellf[j*n+i]];
   	    }
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
    for (int i=0; i<n*nrowsmax; i++)
        fprintf(file, "%d %20.19g\n", colellf[i], matellf[i]);

    fclose(file);*/
}
