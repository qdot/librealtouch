import realtouch
import socket
import sys

class RealTouchNetworkServer(object):
  def __init__(self, r):
    self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
    self.s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    self.s.bind(("localhost", 666))
    self.r = r

  def runLoop(self):
    self.s.listen(5)
    c, addr = self.s.accept()
    while True:
      b = c.recv(1024)
      if len(b) > 0:
        print b
        command = b.split(" ")
        print command
        if command[0] == "V":
          self.r.vectorMovement(int(command[1]) * 10, command[2], command[3], int(command[4][0:-1]))
    pass

def main():
  r = realtouch.RealTouch()

  r.getSerial()
  r.getFirmwareVersion()

  n = RealTouchNetworkServer(r)
  n.runLoop()

  return 0

if __name__ == "__main__":
  sys.exit(main())
