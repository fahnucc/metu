def SolveGame(method_name, problem_file_name, player_type):
    file = open(problem_file_name, 'r')
    readedLine = file.readline()
    readedLine = readedLine.replace("[", "")
    readedLine = readedLine.replace("]", "")
    root = readedLine.split(",")
    root = [eval(i) for i in root]

    if method_name == "Minimax":
        result = minimax(root, (True if player_type == "MAX" else False), 0)

    return result


def minimax(node, isMax, count):
    result = minimax2(node, isMax, count)
    return [result[2], result[1]]


def minimax2(node, isMax, count):
    if isTerminal(node):
        return (evalMinMax(isMax), count, None)

    children = GetAllChildren(node)
    selectedResult = minimax2(children[0], not isMax, count)
    selectedScore = selectedResult[0]
    selectedCount = selectedResult[1]
    selectedChild = children[0]

    sum = selectedCount
    for ind, child in enumerate(children):
        if ind == 0:
            continue
        result = minimax2(child, not isMax, count)
        score = result[0]
        newCount = result[1]
        sum += newCount
        if (isMax and score > selectedScore) or (not isMax and score < selectedScore):
            selectedScore = score
            selectedChild = child
            count = sum

    return (selectedScore, count + 1, selectedChild)


def isTerminal(node):
    for pile in node:
        if pile != 0:
            return False
    return True


def evalMinMax(isMax):
    if isMax:
        return -1
    else:
        return 1


def GetAllChildren(root):
    children = []
    for i in range(len(root)):
        curr = 0
        while curr < root[i]:
            child = list(root)
            child[i] = curr
            children.append(child)
            curr += 1
    return children
