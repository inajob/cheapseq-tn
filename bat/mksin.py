import math

# 0 - 255, full wave
print "char sinTable[512] = {"
points = []
for x in xrange(512):
  points.append((str)((int)(127*math.sin(math.pi * 2 * x / 512))));
print ",".join(points)
print "};"
