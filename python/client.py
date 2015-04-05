#!/usr/bin/python
MAX_BYTE = 1024

import sys
import socket
import re
import struct


def parse(data):
    #num2 = re.findall(r'\d+', data)
    #rfind(str, beg=0,end=len(string))
    return re.sub(r'\D+', "", data)

def readheader(s):
     n_seq = s.recv(4)  #byte is same as string
     #int.from_bytes(b, 'big') # supported in 3.2+
     seq = struct.unpack('i', n_seq)[0] # 2.x, host byte order - str to int
     #struct.unpack('!i', b)[0] # 2.x, network byte order -  str to int
     
     b = s.recv(4)
     csum = struct.unpack('i', b)[0] 
     b = s.recv(4)
     datalen = struct.unpack('!i', b)[0] 

     #datalen = socket.ntohl(int(s.recv(4)))
     #print "seq %d, checksum %d, datalen %d" % (seq, checksum, datalen) 
     #str = "seq = {0}, csum = {1}, len = {2}".format(seq, checksum, datalen)

     return seq,n_seq,csum,datalen

def read_data(s, n_seq, n_csum, datalen):
    b = s.recv(datalen)
    is_valid = is_checksum_valid(n_seq, n_csum, b, datalen)
    return (is_valid, b)
    

def is_checksum_valid(n_seq, n_csum, data, datalen):
    checksum = n_seq
    for i in range(0, datalen/4):
        checksum = checksum ^ int(data[i])
        i = i + 4

    leftover = datalen%4
    if ( leftover > 0 ):
        append = 4 - leftover

    lastdata = data[i:leftover] + 0xab * append
    print 'datalen, leftover, lastdata', (datalen, leftover, lastdata)

def write_data(fo, data):
    if ( fo.closed == 0 ):
        fo.write(data)

def main(argv):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
    localhost = socket.gethostname()
    host = "Challenge2.airtime.com"
    port = 2323
    print 'local host is', localhost
    s.connect((host, port))
    b = s.recv(MAX_BYTE)
    print 'data received ', b
    num = parse(b)
    print 'parsed challenge number', num
    send_data = 'IAM:' + num + ':python-code@test.com:at\n'
    print 'sending ...', send_data
    s.send(u'IAM:' + num + ':python-code@test.com:at\n')
    #s.sendall(b'IAM:' + num + ':python-code@test.com:at\n')
    #s.send(send_data)

    b = s.recv(MAX_BYTE)
    print 'Received reply :', b

    (seq,n_seq,n_csum, datalen) = readheader(s)
    print "seq, n_seq, csum, datalen ",  seq, n_seq, n_csum, datalen 

    try:
        fo  = open('/tmp/python-data.pcm', 'w', 0)
    except IOError:
        print 'Error opening file ..' 

    while 1:
        (is_valid,data) = read_data(s, n_seq, n_csum, datalen)
        if ( is_valid ):
            #insert into a list
            write_data(fo,data)

if __name__ == "__main__":
    main(sys.argv)
