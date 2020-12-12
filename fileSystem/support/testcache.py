import sys
import os.path


for i in range(9):
    os.system("echo 3 > /proc/sys/vm/drop_caches")
    size = 8*2**i #in MB
    if not os.path.exists("../raw_data/cache_size_"+str(size)):
        filename = "../raw_data/cache_size_"+str(size)
        f = open(filename, "w")
        for i in range(size//8):
            for _ in range(2**20):
                f.write("12345678")   
        f.close()
    print("sudo ../bin/file_cache_size ../raw_data/cache_size_+%d %d ../res/cache.csv 5\n"%(size,size))
    os.system("sudo ../bin/file_cache_size ../raw_data/cache_size_%d %d ../res/cache.csv 5"%(size,size))
