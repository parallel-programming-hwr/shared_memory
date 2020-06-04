# shared memory
Simple example of inter process communication with shm_open().

The main process forks itself and then prints whatever the child writes into te shared memory. The child writes 5 time "hallo <#>" and then exits. After printing 5 times whatever the child wrote, the main process unlinks the shared memory object.

## Note
The file noerror.c does the same as main.c, but no error handling.

If you kill the process before exiting, you need to close the shared memory object manually. 
```bash
rm -f /dev/shm/SHARED_MEM
```
## compile
```bash
make
```
## run 
```bash
./main
```
