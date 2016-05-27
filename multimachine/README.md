# Multi Machine version

This application is built to run in two different machines, a client and the server. Make sure to install MPI bindings for C in the server machine.

## How do I run?

To run the server:
```bash
# install MPI
$ sudo yum install openmpi

# Clone the repo
$ git clone https://github.com/aziflaj/parallel-searcher/
$ cd parallel-searcher/multimachine

# prepare the server process to run
$ make server
$ ./server 3000 # specify the port, which is set to 3000 in this case
```

To run the client:
```bash
# Clone the repo
$ git clone https://github.com/aziflaj/parallel-searcher/
$ cd parallel-searcher/multimachine

# prepare the server process to run
$ make client
$ ./client 192.168.56.101 3000 # specify the IP of the server and the port
```
