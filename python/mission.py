#!/usr/bin/python
import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
host = s.socket.gethostname()
print 'local machine is ', host

'connecting to the server'

serv_host="Challenge2.airtime.com"
serv_port = 2323
s.connect(serv_host, serv_port)
buf = s.recv(1024)
print 'buffer ', buf
s.close



