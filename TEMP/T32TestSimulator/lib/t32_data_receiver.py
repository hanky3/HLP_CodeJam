from collections import *
from threading import Thread, Lock
import socket
import re
import select
import time


class T32DataPacket:
    def __init__(self, data_buff, msg_formatter):
        pass

    def parse(self, data):
        pass

    def get(self, name, pos=0, size=0):
        pass

    def set(self, name, data):
        pass


class T32DataReceiver(Thread):
    def __init__(self, func):
        Thread.__init__(self)
        self.daemon = True
        self.lock = Lock()
        self.buffer_lock = Lock()
        self.sock_list = defaultdict(lambda: -1)
        self.disconnect_callback = func
        self.received_data_list = deque()

    def add_connection(self, conn):
        self.lock.acquire()
        try:
            sock_id = conn.get_sockid()
            if sock_id is not None:
                if self.sock_list[sock_id] == -1:
                    self.sock_list[sock_id] = conn
                    print(">> add new connection - %d" % sock_id)
        finally:
            self.lock.release()

        pass

    def remove_connection(self, sock_id, conn_obj=None):
        self.lock.acquire()
        try:
            if self.sock_list.__contains__(sock_id):
                sock = self.sock_list[sock_id]
                self.sock_list.pop(sock_id)
                sock.close()
                if self.disconnect_callback is not None:
                    self.disconnect_callback(sock_id)
        finally:
            self.lock.release()

    def pop_data(self, name, wait_time=5, flush_prev_data=False):
        time_unit = 0.1
        remain_times = wait_time
        matched_data = None

        while remain_times > 0.01:
            self.buffer_lock.acquire()
            repeat_cnt = 0
            try:
                repeat_cnt = 1
                if flush_prev_data or len(self.received_data_list) == 0:
                    repeat_cnt = len(self.received_data_list)

                for i in range(repeat_cnt):
                    data = self.received_data_list.popleft()
                    if data[0] == name:
                        matched_data = data
            finally:
                self.buffer_lock.release()

            if matched_data is not None:
                break
            if repeat_cnt > 0 and flush_prev_data is False:
                break

            time.sleep(time_unit)
            remain_times -= time_unit

        if matched_data is not None:
            print(">> Pop Data - %s[params : %d]" % (matched_data[0], len(matched_data[1])))
            for index in range(len(matched_data[1])):
                print("  > value[%d] : %s" % (index, matched_data[1][index]))

        return matched_data

    def add_data(self, msg_name, received):
        self.buffer_lock.acquire()
        try:
            print(">> Received - %s" % msg_name)
            self.received_data_list.append((msg_name, received))
        finally:
            self.buffer_lock.release()

    def run(self):
        prev_received_data = bytes()
        while True:
            self.lock.acquire()
            try:
                sock_ids = list(self.sock_list.keys())
            finally:
                self.lock.release()

            ret = []
            if len(sock_ids) > 0:
                ret = select.select(sock_ids, [], [], 1)
            else:
                time.sleep(0.1)

            if len(ret) == 0:
                continue

            delimiters = b'#'
            for s in ret[0]:
                try:
                    sock = self.sock_list[s]

                    received_data = prev_received_data
                    while True:
                        buf = sock.recv(1024)
                        received_data += buf

                        if buf == None or len(buf) == 0:
                            print("socket closed(FIN)")
                            self.remove_connection(s)
                            break

                        pos = received_data.find(delimiters)
                        if pos != -1:
                            prev_received_data = received_data[pos+1:]
                            received_data = received_data[:pos]
                            break

                    # data parse
                    data_list = re.split('\s', received_data.decode())
                    if len(data_list) > 0:
                        msg_name = data_list.pop(0)

                        # Handle the ack for system configuration data
                        if msg_name == "C:":
                            msg_name = data_list.pop(0)

                        param_values = data_list
                        self.add_data(msg_name, param_values)

                except ConnectionResetError:
                    print("socket closed by host : sock_id(%d)" % s)
                    self.remove_connection(s)

