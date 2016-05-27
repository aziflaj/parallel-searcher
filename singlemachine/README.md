# Single Machine version

This application is built to run in a single Linux machine, but it should work on other UNIX-based based machines too.


## How do I run?

To run the test case:
```bash
# install MPI
$ sudo yum install openmpi

# Clone the repo
$ git clone https://github.com/aziflaj/parallel-searcher/
$ cd parallel-searcher/singlemachine

# prepare the server process to run
$ make testrun
```

The above should give the content of the `testfile.txt`. A single `make` will build the executable for you.
