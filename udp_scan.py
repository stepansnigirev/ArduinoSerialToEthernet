import socket
import sys

# ip = "192.168.178.22"
ip = "255.255.255.255"
port = 8788
message = b"?"

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
s.bind(("", port))

s.sendto(message, (ip, port))
s.settimeout(1)

devices = []

while True:
    try:
        data, addr = s.recvfrom(1024)
        if data != message:
            devices.append(addr)
            print("received message from:", addr)
            print(data)
    except:
        print("Scan done. Found %i devices:" % len(devices))
        for d in devices:
            print("\t%s:%d" % d)
        sys.exit()
