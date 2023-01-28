import sys
import socket
import threading as th
import time
import pickle


def sender(PORT, neighbors):
    # Send data
    while not timeoutEvent.is_set():
        if updateEvent.is_set():
            updateEvent.clear()
            for neighbor, cost in neighbors.items():
                with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                    s.connect(("localhost", int(neighbor)))
                    s.sendall(pickle.dumps((PORT, neighbors)))


def receiver(PORT, neighbors):
    # Create a socket
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind(("localhost", int(PORT)))
        s.listen()

        # Receive data
        while not timeoutEvent.is_set():
            try:
                conn, addr = s.accept()
                data = conn.recv(1024)
                senderPort, neighborsInfo = pickle.loads(data)
                conn.close()
                for neighbor, cost in neighborsInfo.items():
                    if neighbor != PORT:
                        if neighbors[neighbor] > neighbors[senderPort] + cost:
                            updateEvent.set()
                            neighbors[neighbor] = neighbors[senderPort] + cost
            except socket.timeout:
                timeoutEvent.set()


if __name__ == "__main__":
    PORT = sys.argv[1]
    neighbors = dict()

    # Read the cost table
    with open(PORT + ".costs", "r") as file:
        numberOfNodes = int(file.readline())
        neighbors[PORT] = 0
        for line in file:
            neighbor, cost = line.split()
            neighbors[neighbor] = int(cost)

    # Fill the rest of the neighbors with INF
    for i in range(numberOfNodes):
        if str(3000 + i) not in neighbors:
            neighbors[str(3000 + i)] = float("inf")

    # Sort the neighbors
    neighbors = dict(sorted(neighbors.items(), key=lambda x: x[0]))

    # Init event for timeout expection
    timeoutEvent = th.Event()  # False
    # Init and set event for update condition
    updateEvent = th.Event()  # False
    updateEvent.set()  # True

    # Set socket timeout
    socket.setdefaulttimeout(5)

    # Start threads
    senderThread = th.Thread(target=sender, args=(PORT, neighbors))
    receiverThread = th.Thread(target=receiver, args=(PORT, neighbors))
    receiverThread.start()
    # Wait for all receivers to start first
    time.sleep(2)
    senderThread.start()
    # Wait for threads to finish
    senderThread.join()
    receiverThread.join()

    # Print all costs table
    for neighbor, cost in neighbors.items():
        print(f"{PORT} -{neighbor} | {cost}")
