#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "spmat.h"

void jds(int nmults, char *mmfile, char *vecfilein, char *vecfileout) {

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

    int nrows=1, rownum=0, tempi, nrowsmax=0,nrowarr[n];
    cnt=0;
    for (int i=1;i<nz;i++){
        tempi = rownum;
	rownum = row[i];
	if (rownum==tempi)
	   nrows++;
	else {
	   nrowarr[cnt]=nrows;
           if (nrows>nrowsmax)
	       nrowsmax=nrows;
	   nrows=1;
	   cnt++;
	}
    }
    nrowarr[cnt]=nrows;
    
    typedef int irow[nrowsmax];
    typedef double drow[nrowsmax];

    int colell[n][nrowsmax],rowIdx[n];
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
	rowIdx[i]=i;
    }

    for (int i=0;i<n-1;i++){
        for (int j=i+1;j<n;j++){
	    if (nrowarr[j]>nrowarr[i]){
	       int temp_n = nrowarr[i];
	       nrowarr[i] = nrowarr[j];
	       nrowarr[j] = temp_n;

	       int temp_r = rowIdx[i];
               rowIdx[i] = rowIdx[j];
               rowIdx[j] = temp_r;

	       irow temp_c;
	       memcpy(temp_c, colell[i], sizeof(irow));
               memcpy(colell[i], colell[j], sizeof(irow));
               memcpy(colell[j], temp_c, sizeof(irow));

	       drow temp_m;
               memcpy(temp_m, matell[i], sizeof(drow));
               memcpy(matell[i], matell[j], sizeof(drow));
               memcpy(matell[j], temp_m, sizeof(drow));
	    }
	}
    }

    file = fopen("rtest.txt", "w");
    for (int i=0; i<n; i++)
        fprintf(file, "%d\n", nrowarr[i]);

    fclose(file);

    int coljds[nz], iterPtr[nrowsmax+1];
    double matjds[nz];
    cnt=0;
    iterPtr[0]=0;
    for (int j=0;j<nrowsmax;j++){
        for (int i=0;i<n;i++){
	    if (colell[i][j] != -1){
	       coljds[cnt]=colell[i][j];
	       matjds[cnt]=matell[i][j];
	       cnt++;
	    }
	}
	iterPtr[j+1]=cnt;
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
        for (int i=1; i<nrowsmax+1; i++){
            int strt = iterPtr[i-1];
            int end = iterPtr[i];
	    cnt=0;
	    for (int j=strt; j<end; j++){
	        vecout[rowIdx[cnt]] += matjds[j]*vecin[coljds[j]];
		cnt++;
	    }
	}
	for (int i=0; i<n; i++)
	    vecin[i] = vecout[i];
    }

    clock_t toc = clock();
    double time = ((double)(toc-tic))/CLOCKS_PER_SEC;
    printf("%.5f\n",time); 
    
    file = fopen(vecfileout, "w");
    for (int i=0; i<n; i++)
        fprintf(file, "%20.19g\n", vecout[i]);

    fclose(file);

   /* file = fopen("matsave.txt", "w");
    for (int i=0; i<n; i++)
        fprintf(file, "%d\n", rowIdx[i]);

    for (int i=0; i<nrowsmax+1; i++)
	fprintf(file, "%d\n", iterPtr[i]);

    for (int i=0; i<nz; i++)
        fprintf(file, "%d %20.19g\n", coljds[i], matjds[i]);

    fclose(file);*/
}
