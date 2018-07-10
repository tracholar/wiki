
fp = open("data.dat", 'w')

n = 0
for line in open("/Users/zuoyuan/Documents/code/test_ffm/train"):
    row = line.split(" ")
    rs = [row[0]]
    for r in row[1:]:
        fkv = r.split(":")
        k = int( int(fkv[0]) * 1e6 + int(fkv[1])  )
        rs.append("%d:1" % k)
    fp.write(" ".join(rs) + "\n")

    n += 1
    if n % 10000 == 0:
        print '\r ROW %d' % n,
fp.close()