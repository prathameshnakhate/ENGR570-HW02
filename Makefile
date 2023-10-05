SpMV: main.o den.o coo.o csr.o ell.o jds.o mtx2txt.o mmio.o
	gcc main.o den.o coo.o csr.o ell.o jds.o mtx2txt.o mmio.o -o SpMV.exe

main.o: main.c spmat.h
	gcc -c main.c

den.o: den.c spmat.h
	gcc -c den.c

coo.o: coo.c spmat.h
	gcc -c coo.c

csr.o: csr.c spmat.h
	gcc -c csr.c

ell.o: ell.c spmat.h
	gcc -c ell.c

jds.o: jds.c spmat.h
	gcc -c jds.c

mtx2txt.o: mtx2txt.c mmio.h spmat.h
	gcc -c mtx2txt.c

mmio.o: mmio.c mmio.h
	gcc -c mmio.c

clean: 
	rm *.o SpMV.exe
