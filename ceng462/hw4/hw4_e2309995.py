import copy

# I implemented only first question.
# I could not get correct distribution values for query variable,
# but I tried my best. I could not figure out where I did mistake.
#  So, I explained my solution below.
# In addition to all of that, I found that my code not works for
# not topological sorted nodes.


def DoInference(method_name, problem_file_name, num_iteration):
    #  READ FILE
    file = open(problem_file_name, 'r')
    readed = file.readlines()

    nodes = []
    parentTable = {}
    readState = 0
    probabilityTable = {}
    query = {}
    for elem in readed:
        lineString = elem.replace("\n", "")
        if lineString == "[BayesNetNodes]":
            readState = 0
            continue
        elif lineString == "[Paths]":
            readState = 1
            continue
        elif lineString == "[ProbabilityTable]":
            readState = 2
            continue
        elif lineString == "[Query]":
            readState = 3
            continue

        if readState == 0:
            nodes.append(lineString)
        elif readState == 1:
            elements = lineString.replace(" ", "").replace(
                "(", "").replace(")", "").replace("'", "").replace("[", "").replace("]", "").split(",")
            node = elements[len(elements) - 1]
            parents = elements[:len(elements) - 1]
            parentTable[node] = parents
        elif readState == 2:
            ind0 = lineString.find("'")
            ind1 = lineString[ind0 + 1:].find("'")
            node = lineString[ind0+1:ind0+ind1+1]
            ind0 = lineString.find("{")
            ind1 = lineString.find("}")
            prob = lineString[ind0 +
                              1:ind1].replace("'", "").replace(" ", "")
            # No parent
            if prob.find("T") == -1:
                probabilityTable[node] = float(prob.replace(",", ""))
            # One parent
            elif prob.find("(") == -1:
                probs = prob.replace("True:", "").replace(
                    "False:", "").split(",")
                probabilityTable[node] = {True: float(
                    probs[0]), False: float(probs[1])}
            # Two parents
            else:
                probabilityTable[node] = {}
                probs = prob.split(",(")
                for i in range(0, len(probs)):
                    probs[i] = probs[i].replace(
                        "(", "").replace(")", "").split(":")
                for i in probs:
                    tupleName = i[0].split(",")
                    key1 = True if tupleName[0] == 'True' else False
                    key2 = True if tupleName[1] == 'True' else False
                    key = tuple((key1, key2))
                    probabilityTable[node][key] = float(i[1])
        elif readState == 3:
            ind0 = lineString.find("'")
            ind1 = lineString[ind0 + 1:].find("'")
            node = lineString[ind0+1:ind0+ind1+1]
            query["node"] = node
            query["params"] = {}
            ind0 = lineString.find("{")
            ind1 = lineString[ind0 + 1:].find("}")
            params = lineString[ind0 + 1:ind0 + ind1 + 1].split(",")
            for i in params:
                i = i.replace("'", "").replace(" ", "")
                i = i.split(":")
                query["params"][i[0]] = bool(i[1])

    #  READ FILE END
    # print("Nodes: ", nodes)
    # print("Parent Table: ", parentTable)
    # print("Probability Table: ", probabilityTable)
    # print("Query: ", query)

    if method_name == "ENUMERATION":
        print(enum_ask(nodes, parentTable, probabilityTable, query))


def getChildren(node, parents):
    children = []
    for k, v in parents.items():
        if node in v:
            children.append(k)
    return children


def getParentCount(node, parents):
    if node not in parents.keys():
        return 0
    else:
        return len(parents[node])


def getListFromDictKeys(dict):
    list = []
    for key in dict.keys():
        list.append(key)

    return list


def enum_ask(nodes, parentTable, probabilityTable, query):
    P = {}
    for xi in (True, False):
        # Copy initial e from query
        e = copy.deepcopy(query["params"])
        # Assign query variable's value
        e[query['node']] = xi

        P[xi] = enum_all(
            nodes, parentTable, probabilityTable, query, copy.deepcopy(e))

    # Create tuple and normalize
    dist = (P[True], P[False])
    return normalize(dist)


def enum_all(vars, parentTable, probabilityTable, query, e):
    if vars == []:
        return 1.0

    V = vars[0]

    # If node is in evidence list (it may be added later)
    # return possibility of that node according to its parents
    # and continue with rest of nodes
    if V in e.keys():
        result = p(V, parentTable, probabilityTable, e) * \
            enum_all(vars[1:], parentTable,
                     probabilityTable, query, e)
    # Else calculate all possibilities with assigning possible values to V
    else:
        result = 0
        for v in (True, False):
            newe = copy.deepcopy(e)
            newe[V] = v
            result += p(V, parentTable, probabilityTable, newe) * enum_all(vars[1:], parentTable,
                                                                           probabilityTable, query, newe)
    return result


def p(variable, parentTable, probabilityTable, evidenceList):
    # If variable has no parents, get probability directly from table
    if variable not in parentTable.keys():
        key = variable
        result = probabilityTable[variable]
    # Else find correct key by looking evidenceList then get probability
    # from table
    else:
        parentCount = len(parentTable[variable])

        if parentCount == 1:
            parent = parentTable[variable][0]
            key = evidenceList[parent]
            result = probabilityTable[variable][key]
        else:
            parent1 = parentTable[variable][0]
            parent2 = parentTable[variable][1]
            val1 = evidenceList[parent1]
            val2 = evidenceList[parent2]
            key = (val1, val2)
            result = probabilityTable[variable][key]

    return result


def normalize(x):
    x0 = x[0] / (x[0] + x[1])
    x1 = x[1] / (x[0] + x[1])
    return (round(x0, 3), round(x1, 3))
