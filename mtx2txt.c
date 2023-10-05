/* 
*   Matrix Market I/O example program
*
*   Read a real (non-complex) sparse matrix from a Matrix Market (v. 2.0) file.
*   and copies it to stdout.  This porgram does nothing useful, but
*   illustrates common usage of the Matrix Matrix I/O routines.
*   (See http://math.nist.gov/MatrixMarket for details.)
*
*   Usage:  a.out [filename] > output
*
*       
*   NOTES:
*
*   1) Matrix Market files are always 1-based, i.e. the index of the first
*      element of a matrix is (1,1), not (0,0) as in C.  ADJUST THESE
*      OFFSETS ACCORDINGLY offsets accordingly when reading and writing 
*      to files.
*
*   2) ANSI C requires one to use the "l" format modifier when reading
*      double precision floating point numbers in scanf() and
*      its variants.  For example, use "%lf", "%lg", or "%le"
*      when reading doubles, otherwise errors will occur.
*/

#include <stdio.h>
#include <stdlib.h>
#include "mmio.h"
#include "spmat.h"

void mtx2txt(char* matname, int *MATSIZE, int *NONZENT)
{
    int ret_code;
    MM_typecode matcode;
    FILE *f;
    int M, N, nz, sym;   
    int i, *I, *J;
    double *val;
     
    if ((f = fopen(matname, "r")) == NULL) 
        exit(1);

    if (mm_read_banner(f, &matcode) != 0)
    {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }

    /* find out if the matrix is symmetric */
    sym = mm_is_symmetric(matcode);

    /* find out size of sparse matrix .... */

    if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) !=0)
        exit(1);

    /* reseve memory for matrices */

    I = (int *) malloc(nz * sizeof(int));
    J = (int *) malloc(nz * sizeof(int));
    val = (double *) malloc(nz * sizeof(double));

    /* NOTE: when reading in doubles, ANSI C requires the use of the "l"  */
    /*   specifier as in "%lg", "%lf", "%le", otherwise errors will occur */
    /*  (ANSI C X3.159-1989, Sec. 4.9.6.2, p. 136 lines 13-15)            */

    for (i=0; i<nz; i++)
    {
        fscanf(f, "%d %d %lg\n", &I[i], &J[i], &val[i]);
        I[i]--;  /* adjust from 1-based to 0-based */
        J[i]--;
    }

    if (f !=stdin) fclose(f);

    /************************/
    /* now write out matrix */
    /************************/

    //mm_write_banner(stdout, matcode);

    FILE *newf = fopen("output.txt", "w");
    if (sym==0){
       for (i=0; i<nz; i++)
           fprintf(newf, "%d %d %20.19g\n", I[i]+1, J[i]+1, val[i]);
    }
    if (sym==1){
       for (i=0; i<nz; i++)
           fprintf(newf, "%d %d %20.19g\n", I[i]+1, J[i]+1, val[i]);
       for (i=0; i<nz; i++){
	   if (I[i] != J[i])
              fprintf(newf, "%d %d %20.19g\n", J[i]+1, I[i]+1, val[i]);
       }
    }

    fclose(newf);

    *MATSIZE = J[nz-1]+1;
    if(sym==0){*NONZENT = nz;}
    if(sym==1){*NONZENT = 2*nz-(J[nz-1]+1);}
}

