#!/usr/bin/env python3
#SERVER
#This will be the controller board.
#Via UDP data will be received and processed to do other tasks
import sys
#sys.path.append('/home/pi/modules') #needed to import custom keymap python file
#from kb_map import keymap
import socket

test_string = 'hello there 126547'
NULL_CHAR = chr(0)
UDP_IP = '192.168.1.16' #IP address of the machine to which this script will run
UDP_PORT = 5000 

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

#Print received data
while True:
    data, addr = sock.recvfrom(1024) #buffer size
    print ("Received data: ", data)

#HID Keyboard emulation
def write_report(report):
    with open('/dev/hidg0', 'rb+') as fd:
        fd.write(report.encode())

#Function to write to HID file the Usage ID
for c in data:

    #write_report(NULL_CHAR*2+keymap[c]+NULL_CHAR*5)
    write_report(NULL_CHAR*8) #needed to release the key, otherwise repeated letters won't be printed (ex: literaLLy)

write_report(NULL_CHAR*2+chr(40)+NULL_CHAR*5)
# Release all keys
write_report(NULL_CHAR*8)
