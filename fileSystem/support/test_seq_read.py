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
    print("sudo ../bin/sequential_access /dev/sda1 %d ../res/seq_read.csv 5\n" % (size))
    os.system("sudo ../bin/sequential_access /dev/sda1 %d ../res/seq_read.csv 5\n" % (size))
