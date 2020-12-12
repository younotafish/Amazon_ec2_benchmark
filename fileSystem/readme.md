# HOW TO USE IT?


1. Compile the source code: 
   1. ```make```

2. Test cache size: 

   1. ```
      cd ./support
      
      python3 testcache.py
      ```

3. Test sequential read

   1. ```
      python3 test_seq_read.py
      ```

4. Test random access

   1. ```
      python3 test_rand_read.py
      ```

5. Test remote access

   1. install the NFS in another machine (another EC2 in our case) as NFS client, and use the original EC2 as server.
   2. In the client machine,run test in the same way as above. In this case, however, the file being read is actually in server machine.

6. Test contention:

   1. ```
      python3 test_contention.py
      python3 analyze_content.py
      ```

7. After each process, we can get the result in `res` directory.