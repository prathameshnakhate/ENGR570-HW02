#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "spmat.h"

void den(int nmults, char *mmfile, char *vecfilein, char *vecfileout) {
    
    int n,nz;

    mtx2txt(mmfile,&n,&nz);

    FILE *file;
    char *filename = "output.txt"; 
    double mat[n][n];
    int row, col;
    double value;

    memset(mat, 0, n*n*sizeof(double));
    file = fopen(filename, "r");
    while (fscanf(file, "%d %d %lf", &row, &col, &value) == 3) {
        if (row < n+1 && col < n+1) {
            mat[row-1][col-1] = value;
        }
    }
    fclose(file);
    remove(filename);

    double vecin[n];
    int cnt = 0;
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
            for (int j=0; j<n; j++){
                vecout[i] = vecout[i] + mat[i][j]*vecin[j];
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
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
	    fprintf(file, "%20.19g\n", mat[i][j]);
	}
    }
    fclose(file);*/
}
