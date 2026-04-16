import socket
import threading
import os


def receive(sock):
    while True:
        try:
            data = sock.recv(1024)
            if not data:
                print("Server disconnected")
                break

            print("Received: " + data.decode('utf-8').strip())
        except:
            print("Server error")
            break

    os._exit(0)


def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(("127.0.0.1", 12345))

    recv_thread = threading.Thread(target=receive, args=(sock,))
    recv_thread.daemon = True
    recv_thread.start()

    try:
        while True:
            message = input()
            if message:
                sock.send((message + '\n').encode('utf-8'))
    except KeyboardInterrupt:
        sock.close()


if __name__ == "__main__":
    main()
