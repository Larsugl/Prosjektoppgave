from socket import *
from ClientHandler import handle


host = 'localhost'
port = 2520


s = socket(AF_INET, SOCK_STREAM)


def run_server():
    s.bind((host, port))
    s.listen()
    print("Server started on port:" + str(port))
    print("Server ready for clients")
    must_run = True
    while must_run:
        client, addr = s.accept()
        handler = handle(client, addr)
        handler.start()



if __name__ == '__main__':
    run_server()