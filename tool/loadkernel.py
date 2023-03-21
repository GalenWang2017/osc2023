import sys
import argparse
import struct
import time

print("read kernel8.img")
with open("kernel8.img", "rb") as f:
    kernelFile = f.read()
s = len(kernelFile)
print("size of kernel: ", s)
# with open("/dev/ttyUSB1", 'wb', buffering = 0) as s:
#     for i in str(len(kernelFile)):
#         time.sleep(0.5)
#         s.write(struct.pack("<I", len(kernelFile)))
#     for i in range(len(kernelFile)):
#         time.sleep(0.01)
#         s.write(bytes(kernelFile[i]))
# print(struct.pack('<I', len(kernelFile)))
with open("/dev/ttyUSB0", 'wb', buffering = 0) as p:
    time.sleep(1)
    p.write(struct.pack("<I", len(kernelFile)))
    time.sleep(1)
    p.write(kernelFile)
print("finish")
