# t32 connection manager
import socket
import sys
from collections import defaultdict
from threading import Thread, Lock
from lib.t32_data_receiver import T32DataReceiver


class T32ConnManager:
    def __init__(self, data_receiver = None):
        self.receiver = data_receiver
        self.connected = False
        self.ip_addr = 'localhost'
        self.port = 9000
        self.sock = None

    def connect(self, address='127.0.0.1', port=9000):
        self.ip_addr = address
        self.port = 9000
        print("=> start connect - IP(%s), PORT(%d)" % (address, port))

        try :
            csock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            csock.connect((self.ip_addr, self.port))
            print("=> T32 Connected")

            csock.setblocking(False)
            self.connected = True
            self.sock = csock

        except ConnectionError:
            print("Connection failed!")
            return False

    def disconnect(self):
        print("=> disconnect()")
        if not self.connected:
            return

        try:
            self.sock.close()
            self.sock = None
        except ConnectionError:
            print("=>Close by force!!")
            self.sock = None

        self.connected = False

    def recv(self, max_size=1024):
        if not self.connected:
            return None
        return self.sock.recv(max_size)

    def send(self, data):
        if not self.connected:
            return

        self.sock.send(data)

    def close(self):
        if not self.connected:
            return False

        self.disconnect()
        return True

    def get_sockid(self):
        if self.connected:
            return self.sock.fileno()
        return None


if __name__ == "__main__":
    conn_mgr = T32ConnManager()
    conn_mgr.connect()

    close_func = lambda sock : print("Socket Disconnected!! - %d" % sock)

    data_receiver = T32DataReceiver(close_func)
    data_receiver.start()
    if conn_mgr.connected:
        data_receiver.add_connection(conn_mgr)
        msg = "SCFG BTAMHOSTPTI\r\n"
        conn_mgr.send(msg.encode())

        data_receiver.pop_data("SCFG")

        received = data_receiver.pop_data("HCIHC_TEST_HCI_PACKET_TX")
        if received is None:
            print("No data received!!")
            sys.exit(-1)

    input_val = input("Enter Q :")
    if input_val == 'q':
        data_receiver.remove_connection(conn_mgr.get_sockid())
        exit(0)

