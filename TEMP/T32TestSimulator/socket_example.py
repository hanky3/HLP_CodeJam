import socket
import select
import sys
import time
import struct
from threading import Thread, Lock

sock_list = {}

lock = Lock()
sock_running = True


def get_int_value(buf, start, size):
    if len(buf) < (start+size)*2:
        return 0

    pos = start
    int_val = 0
    for order in range(size):
        pos = start +(order*2)
        int_val |= (int(buf[pos:pos+2], 16) << (size-order-1)*8)
    return int_val

def add_sock(sock):
    lock.acquire()
    try:
        if not sock_list.__contains__(sock.fileno()):
            sock_list[sock.fileno()] = sock
    finally:
        lock.release()


def remove_sock(sock_id):
    lock.acquire()
    try:
        if sock_list.__contains__(sock_id):
            sock = sock_list[sock_id]
            sock_list.pop(sock_id)
            sock.close()
            print(len(sock_list), "!!")
    finally:
        lock.release()


def strip_buf_message(buf):
    delimiter = ['\r', '#', '\n']

    for d in delimiter:
        buf = buf.replace(d, '')

    return buf


def receive_sock_thread():
    while sock_running:
        lock.acquire()
        try:
            sock_ids = list(sock_list.keys())
            ret = []
            if len(sock_ids) > 0:
                ret = select.select(sock_ids, [], [], 1)
            else:
                time.sleep(1)
        finally:
            lock.release()

        if len(ret) == 0:
            continue

        delimiters = b'#'
        for s in ret[0]:
            try:
                sock = sock_list[s]

                received_data = bytes()
                while True:
                    buf = sock.recv(1024)
                    received_data += buf

                    if len(buf) == 0:
                        print("socket closed(FIN)")
                        remove_sock(s)
                        break

                    if buf[len(buf)-1] in delimiters:
                        received_data = received_data[0:len(received_data)-1]
                        break

                str_val = strip_buf_message(received_data.decode())
                print("received[%d] : %s(%d)" % (s, str_val, len(str_val)))

            except ConnectionResetError:
                print("socket closed by host : sock_id(%d)" % s)
                remove_sock(s)


def close_running():
    global sock_running
    sock_running = False


def test_sock_client():
    host = '127.0.0.1'
    port = 9000

    a = get_int_value("0c00", 0, 2)
    print("%04x"%a)

    try:
        csock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        csock.connect((host, port))
        print("sock connected!!")
        message = "SCFG BTAMHOSTPTI\r\n"

        csock.setblocking(False)
        csock.send(message.encode())
        add_sock(csock)

    except ConnectionError:
        print("Connection failed!")

    t = Thread(target=receive_sock_thread)
    t.start()

    while True:
        cmd = input("Enter Q to quit test!! : ")
        if cmd.lower() == "q":
            print("Quit the program!!")
            close_running()
            t.join()
            break


if __name__ == '__main__':
    test_sock_client()
