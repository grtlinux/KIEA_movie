'''
 sample chatting server
 chatserver.py

 programmed by Gang Seong Lee
 2000.7.20 --> 2002.1.10

 server:
   python chatserver.py

 client : telnet localhost 8037

 exit : /quit
'''

import socket
from SocketServer import ThreadingTCPServer, StreamRequestHandler
import thread

PORT = 8037
lock = thread.allocate_lock()   # ��ȣ ���� ������ ���� �� ��ü

class UsersList:
    def __init__(self):
        self.users = {}

    def __contains__(self, name):   # in ������ �޽��
        return name in self.users

    def addUser(self, conn, addr, name):
        if name in self.users:
            conn.send('�̹� ��ϵ� �̸��Դϴ�.\n')
            return None
        lock.acquire()  # ��ȣ ����
        self.users[name] = (conn, addr)
        lock.release()
        # ���� �޽��� ���
        self.broadcastMessage('['+name+'] ' + '�Բ��� �����ϼ̽��ϴ�\n')
        conn.send('[%s]�� �������.\n'%name)    # ���ο��� ���� �޽���
        print len(self.users), 'connections'    # ������ ǥ�õǴ� �޼���
        return name

    def removeUser(self, name):
        if name not in self.users:
            return
        lock.acquire()  # ��ȣ ����
        del self.users[name]
        lock.release()
        self.broadcastMessage('['+name+'] '+'�Բ��� �����̽��ϴ�\n')
        print len(self.users), 'connections'    # ������ ǥ�õǴ� �޼���

    def handleMessage(self, name, msg):
        if msg[0] != '/':
            self.broadcastMessage('['+name+'] ' + msg)
            return
        cmd = msg[1:].rstrip().split()
        if cmd[0] == 'quit':
            self.removeUser(name)
            return -1

    def broadcastMessage(self, msg):
        for conn, addr in self.users.values():
            conn.send(msg)

class RequestHandler(StreamRequestHandler):
    users = UsersList()
    # ���� �䱸�� ������ ȣ��ȴ�.
    def handle(self):
        print 'connection from', self.client_address    # ������ ǥ�õǴ� �޼���
        conn = self.request
        self.file = conn.makefile('rb')
        try:
            name = self.readAndRegisterName()
            data = self.file.readline()
            while data:
                if self.users.handleMessage(name, data) == -1:
                    conn.close()
                    break
                data = self.file.readline()
        except socket.error:
            print 'Socket Error'
        print 'Disconnected from', self.client_address    # ���� �޼���
        self.users.removeUser(name)

    # ������ �̸��� �о ����Ѵ�.
    def readAndRegisterName(self):
        while 1:
            self.request.send('Name ? ')
            name = self.file.readline().strip()    # �̸� �б�
            if self.users.addUser(self.request, self.client_address, name):
                return name

#-------------------------------------------------------
if __name__ == '__main__':
    server = ThreadingTCPServer(("", PORT), RequestHandler)
    print 'listening on port', PORT
    server.serve_forever()
