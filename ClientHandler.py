from threading import Thread
from socket import *

class handle(Thread):

    def __init__(self, c, addr):
        Thread.__init__(self)
        self.c = c
        self.addr = addr

    def run(self):
        try:
            print(self.c)
            message = "something"
            while message != "":
                try:
                    message = self.c.recv(1024).decode()
                    print("%s: %s" % (self.addr, message))
                except error as e:
                    print("error {0}:{1}".format(e.errno, e.strerror))
                    self.c.close()
        except error as e:
            print("error {0}:{1}".format(e.errno, e.strerror))