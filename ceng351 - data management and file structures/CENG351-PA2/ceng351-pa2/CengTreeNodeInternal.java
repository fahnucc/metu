import java.util.ArrayList;

public class CengTreeNodeInternal extends CengTreeNode
{
    private ArrayList<Integer> keys;
    private ArrayList<CengTreeNode> children;

    public CengTreeNodeInternal(CengTreeNode parent)
    {
        super(parent);

        // TODO: Extra initializations, if necessary.
    }

    // GUI Methods - Do not modify
    public ArrayList<CengTreeNode> getAllChildren()
    {
        return this.children;
    }
    public Integer keyCount()
    {
        return this.keys.size();
    }
    public Integer keyAtIndex(Integer index)
    {
        if(index >= this.keyCount() || index < 0)
        {
            return -1;
        }
        else
        {
            return this.keys.get(index);
        }
    }

    // Extra Functions

    public CengTreeNodeInternal(CengTreeNode parent, ArrayList<Integer> keys, ArrayList<CengTreeNode> children) {
        super(parent);
        this.keys = keys;
        this.children = children;
        this.setType(CengNodeType.Internal);
    }

    public int findIndexOfPointer(CengTreeNode pointer) {
        for (int i = 0; i < children.size(); i++) {
            if (children.get(i) == pointer) {
                return i;
            }
        }
        return -1;
    }

    public boolean isOverfull() {
        return this.keys.size() > (2 * order);
    }

    public ArrayList<Integer> getKeys() {
        return this.keys;
    }

    public void setKeys(ArrayList<Integer> keys) {
        this.keys = keys;
    }

    public void setChildren(ArrayList<CengTreeNode> children) {
        this.children = children;
    }
}
