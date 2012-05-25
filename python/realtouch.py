import sys, os
import array
import logging

class RealTouch(object):
  def __init__(self):
    self.fd = open("/dev/hidraw4", "w+b")
    pass

  def getSerial(self):
    g = array.array('B', [0xa] + [0] * 63)
    self.fd.write(g)
    print ["0x%.02x " % (ord(x)) for x in self.fd.read(64)]

  def getFirmwareVersion(self):
    g = array.array('B', [0] * 64)
    self.fd.write(g)
    print ["0x%.02x " % (ord(x)) for x in self.fd.read(64)]

def main():
  r = RealTouch()

  r.getSerial()
  r.getFirmwareVersion()

  return 0

if __name__ == "__main__":
  sys.exit(main())