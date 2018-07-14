import struct

fp = open("data.dat", 'wb')

n = 0
for line in open("/Users/zuoyuan/Documents/code/test_ffm/train"):
    row = line.split(" ")
    fp.write(struct.pack('d',  float(row[0])))
    fp.write(struct.pack('i', len(row) - 1))
    for r in row[1:]:
        fkv = r.split(":")
        fp.write(struct.pack('i', int(fkv[0]) * 1000000 + int(fkv[1])))
        fp.write(struct.pack('d', float(fkv[2])))

    n += 1
    if n % 100000 == 0:
        print '\r ROW %d' % n
    #if n >= 100000:
    #    break

fp.close()