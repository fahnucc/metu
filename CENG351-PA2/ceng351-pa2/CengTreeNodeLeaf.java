import java.util.ArrayList;

public class CengTreeNodeLeaf extends CengTreeNode
{
    private ArrayList<CengVideo> videos;
    // TODO: Any extra attributes

    public CengTreeNodeLeaf(CengTreeNode parent)
    {
        super(parent);

        // TODO: Extra initializations
    }

    // GUI Methods - Do not modify
    public int videoCount()
    {
        return videos.size();
    }
    public Integer videoKeyAtIndex(Integer index)
    {
        if(index >= this.videoCount()) {
            return -1;
        } else {
            CengVideo video = this.videos.get(index);

            return video.getKey();
        }
    }

    // Extra Functions

    // yer varsa videoyu direk ekle, yoksa false don
    public boolean insert(CengVideo video) {
        if (this.videos.size() >= (2 * order)) {
            return false;
        } else {
            int index;
            for (index = 0 ; index < videos.size() ; index++) {
                if (videos.get(index).getKey() > video.getKey()) {
                    break;
                }
            }
            this.videos.add(index, video);
            return true;
        }
    }

    public CengTreeNodeLeaf(CengTreeNode parent, ArrayList<CengVideo> videos) {
        super(parent);
        this.videos = videos;
        this.setType(CengNodeType.Leaf);
    }

    public ArrayList<CengVideo> getVideos() {
        return videos;
    }

    public void setVideos(ArrayList<CengVideo> videos) {
        this.videos = videos;
    }


}
