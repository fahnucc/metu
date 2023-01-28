import sys
import socket
import threading
import time
import pickle

HOST = "127.0.0.1"

timeout = True
PORT = sys.argv[1]
numberOfNodes = 0
neighbors = dict()
isThereUpdate = True


def print_costtable():
    for neighbor, cost in neighbors.items():
        print(f"{PORT} -{neighbor} | {cost}")


def send_costtable():
    global timeout, isThereUpdate, neighbors, PORT
    while timeout:
        if isThereUpdate:
            isThereUpdate = False
            for neighbor, cost in neighbors.items():
                with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                    s.connect(("localhost", int(neighbor)))
                    sending_object = {'sender': PORT, 'data': neighbors}
                    s.sendall(pickle.dumps(sending_object))
                    s.close()


def receive_costtable():
    global timeout, isThereUpdate, neighbors, PORT

    socket.setdefaulttimeout(5)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(("localhost", int(PORT)))
    s.listen()

    while True:
        # print("Waiting for connection...")
        try:
            conn, addr = s.accept()
            data = conn.recv(1024)
            data = pickle.loads(data)
            conn.close()
            senderPort = data['sender']
            neighborsInfo = data['data']
            for neighbor, cost in neighborsInfo.items():
                if neighbor != PORT:
                    if neighbors[neighbor] > neighbors[senderPort] + neighborsInfo[neighbor]:
                        neighbors[neighbor] = neighbors[senderPort] + \
                            neighborsInfo[neighbor]
                        isThereUpdate = True
        except socket.timeout:
            timeout = False
            break


if __name__ == "__main__":
    with open(PORT + ".costs", "r") as file:
        numberOfNodes = int(file.readline())
        neighbors[PORT] = 0
        for line in file:
            neighbor, cost = line.split()
            neighbors[neighbor] = int(cost)

    for i in range(numberOfNodes):
        if str(3000 + i) not in neighbors:
            neighbors[str(3000 + i)] = float("inf")

    send_thread = threading.Thread(target=send_costtable)
    receive_thread = threading.Thread(target=receive_costtable)
    receive_thread.start()
    time.sleep(3)
    send_thread.start()
    receive_thread.join()
    send_thread.join()
    print_costtable()
