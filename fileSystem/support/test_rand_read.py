import sys
import os

for i in range(8):
    size = 8*2**i  # in MB
    if not os.path.exists("../raw_data/cache_size_"+str(size)):
        filename = "../raw_data/cache_size_"+str(size)
        f = open(filename, "w")
        for i in range(size//8):
            for _ in range(2**20):
                f.write("12345678")
        f.close()
    print("sudo ../bin/random_access /dev/sda1 %d ../res/rand_read.csv 20\n" % (size))
    os.system("sudo ../bin/random_access /dev/sda1 %d ../res/rand_read.csv 20\n" % (size))
