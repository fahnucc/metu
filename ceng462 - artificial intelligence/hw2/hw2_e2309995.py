def InformedSearch(method_name, problem_file_name):
    file = open(problem_file_name, 'r')
    readed = file.readlines()

    env = []
    for elem in readed:
        lineString = elem.replace("\n", "")
        env.append(lineString.split("\t"))

    graph = {}
    availableSquares = []

    # Create graph and its keys for valid squares.
    for y in range(0, len(env)):
        for x in range(0, len(env[0])):
            if env[y][x] == '#':
                continue
            elif env[y][x] == 'S':
                start = (x, y)
            elif env[y][x] == 'E':
                final = (x, y)

            availableSquares.append((x, y))
            graph[(x, y)] = []

    # Fill graph connections with all cost = 1.
    for (x, y) in availableSquares:
        for (xi, yi) in [(-1, 0), (0, -1), (1, 0), (0, 1)]:
            nx = x + xi
            ny = y + yi
            if nx >= 0 and nx < len(env[0]) and ny >= 0 and ny < len(env) and env[ny][nx] != "#":
                graph[(x, y)].append(
                    ({"to": (nx, ny), "cost": 1}))

    if method_name == "UCS":
        path = ucs(graph, start, final)
    elif method_name == "AStar":
        # Create and fill heuristic values of each valid square.
        heuristic = {}
        for y in range(0, len(env)):
            for x in range(0, len(env[0])):
                if env[y][x] == 'E' or env[y][x] == 'S':
                    heuristic[(x, y)] = 0
                elif env[y][x] != '#':
                    heuristic[(x, y)] = int(env[y][x])

        path = aStar(graph, heuristic, start, final)

    return path


# This function is simplier than hw1 ucs function,
# since it has no min required visit counts.
def ucs(graph, start, final):
    q = list()
    v = list()

    q.append((0, start, [start]))

    while len(q) != 0:
        f, current_node, path = popHighestPriority(q)
        v.append(current_node)

        if current_node == final:
            return path
        else:
            for neighbor in graph[current_node]:
                if neighbor['to'] not in path and neighbor['to'] not in v:
                    q.append(
                        (f + neighbor['cost'], neighbor['to'], [neighbor['to']] + path))


def popHighestPriority(list):
    leastCost = list[0][0]
    highestPriority = list[0]

    for elem in list:
        if elem[0] < leastCost:
            leastCost = elem[0]
            highestPriority = elem

    list.remove(highestPriority)
    return highestPriority


def aStar(graph, heuristic, start, final):
    # Opened -> visited but not fully inspected yet
    opened = [start]
    # Closed -> visited and fully inspected
    closed = []

    # Distances from start node
    g = {}
    g[start] = 0

    parents = {}
    parents[start] = start

    path = []

    while len(opened) > 0:
        n = None
        # Find the node in opened list with lowest f = total cost + heuristic
        for v in opened:
            if n == None or g[v] + heuristic[v] < g[n] + heuristic[n]:
                n = v

        # If chosen opened square is not final destination, expand its children.
        if n != final:
            for nbor in graph[n]:
                m = nbor['to']
                cost = nbor['cost']

                # If chosen children is not inspected yet, add it to opened list, set parent, set g.
                if m not in opened and m not in closed:
                    opened.append(m)
                    parents[m] = n
                    g[m] = g[n] + cost
                # Else, compare total cost of child with new cost from current parent.
                else:
                    if g[n] + cost < g[m]:
                        g[m] = g[n] + cost
                        parents[m] = n

                        # If new path is found to m and it was closed, open it.
                        if m in closed:
                            closed.remove(m)
                            opened.append(m)

        # Return None if 0 path found.
        if n == None:
            return None

        # If current square is final, create path by following parents.
        if n == final:
            # Check if parent is same as child, i.e. start.
            while parents[n] != n:
                path.append(n)
                n = parents[n]

            path.append(start)
            return path

        # If code comes there, it means we traverse all children of current square.
        opened.remove(n)
        closed.append(n)

    # Return None if 0 path found.
    return None
