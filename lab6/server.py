import socket
import threading
from datetime import datetime

clients = []
lock = threading.Lock()

def message_handler(msg,sender_socker):
    with lock:
        for client in clients:
            if client != sender_socker:
                try:
                    client.send(msg.encode('utf-8'))
                except:
                    pass

def handle_client(client_sock,client_addr):
    with lock:
        clients.append(client_sock)
    client_id = client_addr[1]
    time_str = datetime.now().strftime("%H:%M:%S")
    print(f"[{time_str}] {client_addr} connected")

    while True:
        try:
            data = client_sock.recv(1024)
            if not data:
                break
            text = data.decode('utf-8').strip()

            time_str = datetime.now().strftime("%H:%M:%S")
            print(f"[{time_str}] {client_addr} message send: {text}")
            message_temp = f"[{time_str}] [{client_id}] message received: {text}"
            message_handler(message_temp,client_sock)
        except:
            break
    time_str = datetime.now().strftime("%H:%M:%S")
    print(f"[{time_str}] {client_addr} disconnected")
    with lock:
        if client_sock in clients:
            clients.remove(client_sock)
    client_sock.close()


def main():
    server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    server_sock.bind(('0.0.0.0', 12345))
    server_sock.listen(5)

    time_str = datetime.now().strftime("%H:%M:%S")

    try:
        while True:
            client_sock, addr = server_sock.accept()

            thread = threading.Thread(target=handle_client, args=(client_sock, addr))
            thread.daemon = True
            thread.start()

    except KeyboardInterrupt:
        with lock:
            for client in clients:
                try:
                    client.send("Closing server.\n".encode('utf-8'))
                    client.close()
                except:
                    pass

        server_sock.close()


if __name__ == "__main__":
    main()