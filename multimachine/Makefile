#
# Makefile for building the client, the server and the multi-process searching
# program.
#
# Author: Aldo Ziflaj
# April 25, 2016
#

CC = gcc
FLAGS = -std=gnu99

MPICC = mpicc

CLIENT_SRC = client.c
SERVER_SRC = server.c
MPI_SEARCH_SRC = file_search.c

all:
	$(MAKE) clean client server

client:
	$(RM) client
	$(CC) $(FLAGS) $(CLIENT_SRC) -o client
	echo "CLIENT BUILT"

server:
	$(CC) $(FLAGS) $(SERVER_SRC) -o server
	$(MPICC) $(MPI_SEARCH_SRC) -o search
	echo "SERVER BUILT"

clean:
	$(RM) client server file_search

.SILENT: all client server search clean
.PHONY: all client server search clean
