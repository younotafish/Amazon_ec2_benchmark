import sys
import os.path


for i in range(9):
    os.system("echo 3 > /proc/sys/vm/drop_caches")
    size = 8*2**i #in MB
    filename = "../raw_data/cache_size_"+str(size)
    if not os.path.exists(filename):
        f = open(filename, "w")
        for i in range(size//8):
            for _ in range(2**20):
                f.write("12345678")   
        f.close()
    if not os.path.exists("../bin/random_access"):
        print("test file.c not exist")
    else:
        for j in range(7):
            resfile = ("../res/content_seq/content%d_%d.csv" % (size,j))
            if not os.path.exists(resfile):
                f = open(resfile,"w+")
                f.close();
            print("sudo ../bin/sequential_access %s %d %s 5\n"%(filename,size,resfile))
            os.system("sudo ../bin/sequential_access %s %d %s 5&"%(filename,size,resfile))
        resfile = ("../res/content_seq/content%d_%d.csv" % (size,7))
        if not os.path.exists(resfile):
            f = open(resfile,"w+")
            f.close();
        print("sudo ../bin/sequential_access %s %d %s 5\n"%(filename,size,resfile))
        os.system("sudo ../bin/sequential_access %s %d %s 5"%(filename,size,resfile))
        
