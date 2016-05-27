#
# Makefile for building the client, the server and the multi-process searching
# program.
#
# Author: Aldo Ziflaj
# April 25, 2016
#

MPICC = mpicc
MPI_SEARCH_SRC = file_search.c

all:
	$(MPICC) $(MPI_SEARCH_SRC) -o search
	echo "BUILT"

buildrun:
	$(MAKE)
	mpirun -n 5 ./search testfile.txt

clean:
	$(RM) search

.SILENT: all clean
.PHONY: all clean
