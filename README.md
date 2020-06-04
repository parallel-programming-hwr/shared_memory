# shared memory
Simple example of inter process communication with shm_open().

The main process forks itself and then waits for the child to finish. The child writes into a shared memory and exits. After that the main process prints what the child wrote into the shared memory.

## Note
The file noerror.c does the same as main.c, but no error handling.
## compile
```bash
make
```
## run 
```bash
./main
```
