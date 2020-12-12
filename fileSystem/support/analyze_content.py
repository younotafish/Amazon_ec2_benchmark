import sys

path = "../res/content_seq"
with open("%s/analysis.csv" % path, "w+") as dest:
    for i in range(9):
        size = 8*2**i
        avg_time = 0.0
        throughput = 0.0
        filesize = 0
        for j in range(8):
            with open("%s/content%d_%d.csv" % (path, size, j), "r") as src:
                data = src.readline().split(" ")
                filesize = int(data[0])
                throughput += float(data[1])
                avg_time += float(data[2])
                pass
            pass
        avg_time = avg_time / 8
        dest.write("%d %.2f %.2f\n" % (filesize, throughput/8, avg_time))
