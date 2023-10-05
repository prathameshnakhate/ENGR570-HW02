#ifndef SPMAT_H
#define SPMAT_H

void den(int nmults, char *mmfile, char *vecfilein, char *vecfileout);
void coo(int nmults, char *mmfile, char *vecfilein, char *vecfileout);
void csr(int nmults, char *mmfile, char *vecfilein, char *vecfileout);
void ell(int nmults, char *mmfile, char *vecfilein, char *vecfileout);
void jds(int nmults, char *mmfile, char *vecfilein, char *vecfileout);

void mtx2txt(char* matname, int *MATSIZE, int *NONZENT);

#endif // SPMAT_H
