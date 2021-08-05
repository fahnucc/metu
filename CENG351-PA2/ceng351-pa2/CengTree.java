import java.util.ArrayList;

public class CengTree {
    public CengTreeNode root;
    // Any extra attributes...

    public CengTree(Integer order) {
        CengTreeNode.order = order;
        // TODO: Initialize the class
        this.root = null;

    }

    public void addVideo(CengVideo video) {
        // TODO: Insert Video to Tree

        if (this.root == null) {

            ArrayList<CengVideo> videos = new ArrayList<>();
            videos.add(video);
            this.root = new CengTreeNodeLeaf(null, videos);
            this.root.type = CengNodeType.Leaf;

        } else {

            CengTreeNodeLeaf leafNode;

            if (root instanceof CengTreeNodeLeaf) {

                leafNode = (CengTreeNodeLeaf) root;

            } else {

                leafNode = findNode((CengTreeNodeInternal) root, video.getKey());

            }

            if (!leafNode.insert(video)) {

                int index = findIndexToAdd(video.getKey(), leafNode);
                leafNode.getVideos().add(index, video);

                int midpoint = root.getOrder();
                ArrayList<CengVideo> leftChild = new ArrayList<>(leafNode.getVideos().subList(0, midpoint));
                ArrayList<CengVideo> rightChild = new ArrayList<>(leafNode.getVideos().subList(midpoint, leafNode.getVideos().size()));

                if (leafNode.getParent() == null) {

                    ArrayList<Integer> parent_keys = new ArrayList<>();
                    parent_keys.add(rightChild.get(0).getKey());
                    CengTreeNodeInternal parent = new CengTreeNodeInternal(null, parent_keys, null);

                    leafNode.setVideos(leftChild);
                    leafNode.setParent(parent);
                    CengTreeNodeLeaf rightChildNode = new CengTreeNodeLeaf(parent, rightChild);
                    ArrayList<CengTreeNode> children = new ArrayList<>();
                    children.add(0, leafNode);
                    children.add(1, rightChildNode);
                    parent.setChildren(children);

                    this.root = parent;
                    this.root.type = CengNodeType.Internal;

                } else {

                    Integer pushedKey = rightChild.get(0).getKey();
                    CengTreeNodeInternal parent = (CengTreeNodeInternal) leafNode.getParent();
                    int index2 = parent.findIndexOfPointer(leafNode);

                    parent.getKeys().add(index2, pushedKey);
                    leafNode.setVideos(leftChild);
                    CengTreeNodeLeaf rightChildNode = new CengTreeNodeLeaf(parent, rightChild);
                    parent.getAllChildren().add(index2 + 1, rightChildNode);

                    while (parent != null) {

                        if (parent.isOverfull()) {

                            splitInternalNode(parent);

                        } else {

                            break;

                        }
                        parent = (CengTreeNodeInternal) parent.getParent();
                    }
                }
            }
        }
    }

    public ArrayList<CengTreeNode> searchVideo(Integer key) {
        // TODO: Search within whole Tree, return visited nodes.
        // Return null if not found.

        ArrayList<CengTreeNode> visitedNodes = new ArrayList<>();
        CengTreeNodeLeaf leafNode = findLeafNodeContainKey(this.root, key);

        if (leafNode == null) {

            System.out.println("Could not find " + key.toString() + ".");

        } else {

            if (leafNode == this.root) {

                visitedNodes.add(leafNode);
                printLeafNodeContains(leafNode, key);
                return visitedNodes;

            } else {

                CengTreeNodeInternal node = (CengTreeNodeInternal) this.root;
                visitedNodes.add(node);
                printNode(node, 1);

                while (true) {

                    int index;
                    for (index = 0; index < node.getKeys().size(); index++) {
                        if (key < node.getKeys().get(index)) {

                            break;

                        }
                    }

                    if (node.getAllChildren().get(index) instanceof CengTreeNodeLeaf) {

                        visitedNodes.add(node.getAllChildren().get(index));
                        printLeafNodeContains((CengTreeNodeLeaf) node.getAllChildren().get(index), key);
                        return visitedNodes;

                    } else {

                        printNode(node.getAllChildren().get(index), levelCalc(node.getAllChildren().get(index)));
                        visitedNodes.add(node.getAllChildren().get(index));
                        node = (CengTreeNodeInternal) node.getAllChildren().get(index);
                    }
                }
            }
        }

        return null;
    }

    public void printTree() {
        // TODO: Print the whole tree to console

        if (this.root instanceof CengTreeNodeLeaf) {

            printNode(this.root, 1);

        } else {

            printTreeHelper((CengTreeNodeInternal) this.root);

        }


    }

    public CengTreeNodeLeaf findLeafNodeContainKey(CengTreeNode node, Integer key) {

        if (node instanceof CengTreeNodeLeaf) {

            for (int i = 0; i < ((CengTreeNodeLeaf) node).getVideos().size(); i++) {
                if (key == ((CengTreeNodeLeaf) node).getVideos().get(i).getKey()) {

                    return (CengTreeNodeLeaf) node;

                }
            }
            return null;

        } else {

            int index = 0;
            for (Integer x : ((CengTreeNodeInternal) node).getKeys()) {
                if (key < x) {

                    break;

                }
                index++;
            }

            return findLeafNodeContainKey(((CengTreeNodeInternal) node).getAllChildren().get(index), key);
        }
    }

    public void printLeafNodeContains(CengTreeNodeLeaf node, Integer key) {

        CengVideo wanted = null;
        for (CengVideo video : node.getVideos()) {
            if (video.getKey() == key) {

                wanted = video;

            }
        }
        String tab = "\t";
        Integer tabNum = levelCalc(node) - 1;
        String fullName = wanted.fullName();
        System.out.println(tab.repeat(tabNum) + "<record>" + fullName + "</record>");

    }

    public void printTreeHelper(CengTreeNodeInternal node) {

        Integer levelNow = levelCalc(node);
        printNode(node, levelNow);

        for (int i = 0; i < node.getAllChildren().size(); i++) {
            CengTreeNode child = node.getAllChildren().get(i);

            if (child instanceof CengTreeNodeLeaf) {

                printNode(child, levelNow + 1);

            } else {

                printTreeHelper((CengTreeNodeInternal) child);

            }
        }
    }

    public Integer levelCalc(CengTreeNode node) {

        Integer level = 1;
        CengTreeNode parent = node.getParent();

        while (parent != null) {

            level += 1;
            parent = parent.getParent();

        }

        return level;
    }

    public void printNode(CengTreeNode node, Integer level) {

        Integer tabNum = level - 1;
        String tab = "\t";

        if (node instanceof CengTreeNodeInternal) {

            System.out.println(tab.repeat(tabNum) + "<index>");
            for (Integer key : ((CengTreeNodeInternal) node).getKeys()) {
                System.out.println(tab.repeat(tabNum) + key);
            }

            System.out.println(tab.repeat(tabNum) + "</index>");

        } else if (node instanceof CengTreeNodeLeaf) {

            System.out.println(tab.repeat(tabNum) + "<data>");
            for (int i = 0; i < ((CengTreeNodeLeaf) node).getVideos().size(); i++) {
                String fullName = ((CengTreeNodeLeaf) node).getVideos().get(i).fullName();
                System.out.println(tab.repeat(tabNum) + "<record>" + fullName + "</record>");
            }

            System.out.println(tab.repeat(tabNum) + "</data>");

        }
    }

    public CengTreeNodeLeaf findNode(CengTreeNodeInternal node, Integer key) {

        ArrayList<Integer> keys = node.getKeys();
        int i;

        for (i = 0; i < keys.size(); i++) {
            if (key < keys.get(i)) {

                break;

            }
        }

        CengTreeNode childNode = node.getAllChildren().get(i);
        if (childNode instanceof CengTreeNodeLeaf) {

            return (CengTreeNodeLeaf) childNode;

        } else {

            return findNode((CengTreeNodeInternal) node.getAllChildren().get(i), key);

        }
    }

    public Integer findIndexToAdd(Integer key, CengTreeNodeLeaf node) {
        int index;
        for (index = 0; index < node.getVideos().size(); index++) {
            if (key < node.getVideos().get(index).getKey()) {

                break;

            }
        }
        return index;
    }

    public void splitInternalNode(CengTreeNodeInternal internalNode) {

        int midpoint = root.getOrder();
        int newParentKey = internalNode.getKeys().get(midpoint);

        ArrayList<Integer> leftKeys = new ArrayList<>(internalNode.getKeys().subList(0, midpoint));
        ArrayList<Integer> rightKeys = new ArrayList<>(internalNode.getKeys().subList(midpoint + 1, internalNode.getKeys().size()));
        ArrayList<CengTreeNode> leftChildren = new ArrayList<>(internalNode.getAllChildren().subList(0, midpoint + 1));
        ArrayList<CengTreeNode> rightChildren = new ArrayList<>(internalNode.getAllChildren().subList(midpoint + 1, internalNode.getAllChildren().size()));

        internalNode.setChildren(leftChildren);
        internalNode.setKeys(leftKeys);

        CengTreeNodeInternal rightSibling = new CengTreeNodeInternal(internalNode.getParent(), rightKeys, rightChildren);

        for (CengTreeNode child : rightChildren) {
            if (child != null) {

                child.setParent(rightSibling);

            }
        }

        if (internalNode.getParent() == null) {

            ArrayList<Integer> keys = new ArrayList<>();
            keys.add(newParentKey);
            ArrayList<CengTreeNode> children = new ArrayList<>();
            children.add(0, internalNode);
            children.add(1, rightSibling);
            CengTreeNodeInternal newRoot = new CengTreeNodeInternal(null, keys, children);
            internalNode.setParent(newRoot);
            rightSibling.setParent(newRoot);

            this.root = newRoot;
            this.root.type = CengNodeType.Internal;

        } else {

            CengTreeNodeInternal parent = (CengTreeNodeInternal) internalNode.getParent();
            int index = parent.findIndexOfPointer(internalNode);
            parent.getKeys().add(index, newParentKey);
            parent.getAllChildren().add(index + 1, rightSibling);
            rightSibling.setParent(parent);

        }
    }

}
