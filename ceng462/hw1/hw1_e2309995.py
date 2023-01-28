def UnInformedSearch(method_name, problem_file_name):
    for line in open(problem_file_name, "r"):
        e = eval(line)
        min = e.get('min')
        env = e.get('env')

    # Connections => {(x1, y1): [(x2, y2, cost2), (x3, y3, cost3)], ...}
    connections = {}
    # customerPositions => [(x1, y1), (x2, y2), ...]
    customerPositions = []

    # Fill customerPositions, find start and final,
    # and add new keys for valid customer positions
    # in connections dict.
    for row in range(0, len(env)):
        for col in range(0, len(env[0])):
            if env[row][col] == '.':
                continue
            elif env[row][col] == 'C':
                customerPositions.append((row, col))
            elif env[row][col] == 'S':
                start = (row, col)
            elif env[row][col] == 'F':
                final = (row, col)
            connections[(row, col)] = []

    # Do not search if min is greater than customer count.
    # There will be no path.
    if min > len(customerPositions):
        return None

    # Fill connections. Each customer, start and final are connected to each other.
    # Add final to start.
    connections[start].append(
        {"to": final, "cost": getManhattanDistance(start, final)})
    # For each customer, add that customer to start.
    for c in customerPositions:
        connections[start].append(
            ({"to": c, "cost": getManhattanDistance(start, c)}))
        # For each customer, add all other customers to that customer.
        for otherC in customerPositions:
            if c != otherC:
                connections[c].append(
                    ({"to": otherC, "cost": getManhattanDistance(c, otherC)}))
        # For each customer, add final to that customer.
        connections[c].append(
            ({"to": final, "cost": getManhattanDistance(c, final)}))

    if method_name == "BFS":
        path = bfs(connections, start, final, min)
    elif method_name == "DFS":
        path = dfs(connections, start, final, min)
    elif method_name == "UCS":
        path = ucs(connections, start, final, min)

    return path


def getManhattanDistance(start, end):
    return abs(start[0]-end[0]) + abs(start[1]-end[1])


def dfs(connections, start, final, min):
    path = [start]
    visited = [start]
    currentVertex = start
    for count in range(0, min):
        for customer in reversed(connections[currentVertex]):
            if customer['to'] != final and customer['to'] not in visited:
                path.append(customer['to'])
                visited.append(customer['to'])
                currentVertex = customer['to']
                break

    path.append(final)
    return path


def bfs(connections, start, final, min):
    path = [start]
    visited = [start]
    currentVertex = start
    for count in range(0, min):
        for customer in connections[currentVertex]:
            if customer['to'] != final and customer['to'] not in visited:
                path.append(customer['to'])
                visited.append(customer['to'])
                currentVertex = customer['to']
                break

    path.append(final)
    return path


def ucs(connections, start, final, min):
    # Create priority queue
    q = list()
    # Append (cost, vertex, path) tuple for start point to queue.
    q.append((0, start, [start]))

    # Loop until queue is empty
    while len(q) != 0:
        # Pop least cost, highest priority, element from queue.
        f, current_node, path = popHighestPriority(q)

        # If goal is reached return path.
        if current_node == final:
            return path
        else:
            # Traverse all neighbors and if they are not visited before, append
            # (cost, vertex, path) tuple for neighbor.
            # Do not visit final vertex until minimum number of customers visited.
            for neighbor in connections[current_node]:
                if neighbor['to'] not in path and not (len(path) < min + 1 and neighbor['to'] == final):
                    q.append(
                        (f + neighbor['cost'], neighbor['to'], path + [neighbor['to']]))


def popHighestPriority(list):
    leastCost = list[0][0]
    highestPriority = list[0]

    for elem in list:
        if elem[0] < leastCost:
            leastCost = elem[0]
            highestPriority = elem

    list.remove(highestPriority)
    return highestPriority
