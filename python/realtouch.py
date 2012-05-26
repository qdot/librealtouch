import sys, os
import array
import logging

class RealTouch(object):
  def __init__(self):
    self.fd = open("/dev/hidraw4", "w+b")
    pass

  def getSerial(self):
    g = array.array('B', [0] * 64)
    g[0] = [0xa]
    self.fd.write(g)
    print ["0x%.02x " % (ord(x)) for x in self.fd.read(64)]

  def getFirmwareVersion(self):
    g = array.array('B', [0] * 64)
    self.fd.write(g)
    print ["0x%.02x " % (ord(x)) for x in self.fd.read(64)]

  def setHeat(self, magnitude):
    g = array.array('B', [0] * 64)
    g[0] = 0x05
    g[1] = magnitude
    self.fd.write(g)
    print ["0x%.02x " % (ord(x)) for x in self.fd.read(64)]
    
  def fireLube(self, magnitude, time):
    g = array.array('B', [0] * 64)
    # Command byte 0x06
    g[0] = 0x06
    # Magnitude, single byte
    g[1] = magnitude
    # Time in milliseconds, 2 bytes, little endian packing
    g[2] = time & 0xff
    g[3] = (time & 0xff00) >> 0x8
    self.fd.write(g)
    print ["0x%.02x " % (ord(x)) for x in self.fd.read(64)]

  def stopMovement(self, axis):
    # Mapping of CDK Stop command values to USB command values
    stop_dict = {
      "A" : 0x60,
      "H" : 0x40,
      "L" : 0x50,
      "T" : 0x00,
      "B" : 0x10,
      "U" : 0x20,
      "S" : 0x30
      }
    g = array.array('B', [0] * 64)
    g[0] = 0x01
    g[1] = stop_dict[axis]
    self.fd.write(g)
    print ["0x%.02x " % (ord(x)) for x in self.fd.read(64)]
    

def main():
  r = RealTouch()

  r.getSerial()
  r.getFirmwareVersion()

  return 0

if __name__ == "__main__":
  sys.exit(main())