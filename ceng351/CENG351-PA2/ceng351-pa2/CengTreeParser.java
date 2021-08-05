import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

public class CengTreeParser
{
    public static ArrayList<CengVideo> parseVideosFromFile(String filename)
    {
        ArrayList<CengVideo> videoList = new ArrayList<CengVideo>();

        // You need to parse the input file in order to use GUI tables.
        // TODO: Parse the input file, and convert them into CengVideos

        ArrayList<String> strings = new ArrayList<>();

        try (Scanner s = new Scanner(new FileReader(filename))) {
            while (s.hasNext()) {
                strings.add(s.nextLine());
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        ArrayList<String[]> data = new ArrayList<>();

        for (int i = 0; i < strings.size(); i++) {
            data.add(strings.get(i).split("\\|"));
            videoList.add(new CengVideo(Integer.parseInt(data.get(i)[0]), data.get(i)[1], data.get(i)[2], data.get(i)[3]));
        }
        return videoList;
    }

    public static void startParsingCommandLine() throws IOException
    {
        // TODO: Start listening and parsing command line -System.in-.
        // There are 4 commands:
        // 1) quit : End the app, gracefully. Print nothing, call nothing, just break off your command line loop.
        // 2) add : Parse and create the video, and call CengVideoRunner.addVideo(newlyCreatedVideo).
        // 3) search : Parse the key, and call CengVideoRunner.searchVideo(parsedKey).
        // 4) print : Print the whole tree, call CengVideoRunner.printTree().

        // Commands (quit, add, search, print) are case-insensitive.

        Scanner sn = new Scanner(System.in);
        String input = sn.nextLine();
        String[] data;
        while(!input.startsWith("quit")) {
            if (input.startsWith("add")) {
                data = input.split("\\|");
                CengVideoRunner.addVideo(new CengVideo(Integer.parseInt(data[1]), data[2], data[3], data[4]));
            }
            else if(input.startsWith("search")) {
                data = input.split("\\|");
                CengVideoRunner.searchVideo(Integer.parseInt(data[1]));
            }
            else if(input.startsWith("print")) {
                CengVideoRunner.printTree();
            }
            input = sn.nextLine();
        }
    }
}
