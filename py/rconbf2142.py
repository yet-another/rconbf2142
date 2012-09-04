import socket
import hashlib

def connect(ip, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        sock.connect((ip, port))

    except socket.error or socket.herror or socket.gaierror:
        return False

    return sock

def login(sock, password):
    data = ""
    while not "\n\n" in data:
        try:
            data += sock.recv(4096)

        except socket.error or socket.herror or socket.gaierror:
            return False

    index = data.find("### Digest seed: ")
    if index == -1: return False

    m = hashlib.md5()
    m.update(data[index + 17:data.find('\n', index)])
    m.update(password)
    pwhash = m.hexdigest()

    if "Authentication success" in invoke(sock, "login {0}".format(pwhash)):
        return True

    return False

def invoke(sock, msg):
    return _recv(sock) if _send(sock, "\x02{0}\n".format(msg)) else False

def _send(sock, msg):
    sent = 0
    msglen = len(msg)

    while sent < msglen:
        try:
            sent += sock.send(msg)

        except socket.error or socket.herror or socket.gaierror:
            return False

    return True

def _recv(sock):
    data = ""
    while not '\x04' in data:
        try:
            data += sock.recv(4096)

        except socket.error or socket.herror or socket.gaierror:
            return False

    return data[:data.find('\x04')]
