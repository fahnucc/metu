package ceng.ceng351.cengtubedb;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.ResultSet;
import java.sql.Statement;
import java.sql.PreparedStatement;
import java.util.ArrayList;


public class CengTubeDB {
         /**
         * Place your initialization code inside if required.
         *
         * This function will be called before all other operations. If your implementation
         * needs initialization, necessary operations should be done inside this function. For
         * example, you can set your connection to the database server inside this function.
         */
         Connection conn = null;


        public void initialize() {
            try {

                conn = DriverManager.getConnection("jdbc:mysql://144.122.71.168:3306/dbusername?autoReconnect=true&useSSL=false&allowMultiQueries=true", "username", "password");

            } catch (SQLException se) {
                se.printStackTrace();
            }
        }


        /**
         * Should create the necessary tables.
         *
         * @return the number of tables that are created successfully.
         */
        public int createTables(){

            String user = "CREATE TABLE User ("
                    + "userID INTEGER,"
                    + "userName CHAR(30),"
                    + "email CHAR(30),"
                    + "password CHAR(30),"
                    + "status CHAR(15),"
                    + "PRIMARY KEY (userID))";

            String video = "CREATE TABLE Video ("
                    + "videoID INTEGER,"
                    + "userID INTEGER,"
                    + "videoTitle CHAR(60),"
                    + "likeCount INTEGER,"
                    + "dislikeCount INTEGER,"
                    + "datePublished DATE,"
                    + "PRIMARY KEY (videoID),"
                    + "FOREIGN KEY (userID) REFERENCES User(userID) ON DELETE CASCADE)";

            String comment = "CREATE TABLE Comment ("
                    + "commentID INTEGER,"
                    + "userID INTEGER,"
                    + "videoID INTEGER,"
                    + "commentText TEXT(1000),"
                    + "dateCommented DATE,"
                    + "PRIMARY KEY (commentID),"
                    + "FOREIGN KEY (userID) REFERENCES User(userID) ON DELETE SET NULL,"
                    + "FOREIGN KEY (videoID) REFERENCES Video(videoID) ON DELETE CASCADE)";

            String watch = "CREATE TABLE Watch ("
                    + "userID INTEGER,"
                    + "videoID INTEGER,"
                    + "dateWatched DATE,"
                    + "PRIMARY KEY (userID, videoID),"
                    + "FOREIGN KEY (userID) REFERENCES User(userID) ON DELETE CASCADE,"
                    + "FOREIGN KEY (videoID) REFERENCES Video(videoID) ON DELETE CASCADE)";


            Statement stmt;
            int updateCount = 0;

            try{
                stmt = conn.createStatement();
                stmt.executeUpdate(user);
                updateCount++;
                stmt.close();
            } catch (SQLException se) {
                se.printStackTrace();
            }try{
                stmt = conn.createStatement();
                stmt.executeUpdate(video);
                updateCount++;
                stmt.close();
            } catch (SQLException se) {
                se.printStackTrace();
            }try{
                stmt = conn.createStatement();
                stmt.executeUpdate(comment);
                updateCount++;
                stmt.close();
            } catch (SQLException se) {
                se.printStackTrace();
            }try{
                stmt = conn.createStatement();
                stmt.executeUpdate(watch);
                updateCount++;
                stmt.close();
            } catch (SQLException se) {
                se.printStackTrace();
            }

            return updateCount;
        }


        /**
         * Should drop the tables if exists.
         *
         * @return the number of tables that are dropped successfully.
         */
        public int dropTables(){

            String watch = "DROP TABLE IF EXISTS Watch";
            String comment = "DROP TABLE IF EXISTS Comment";
            String video = "DROP TABLE IF EXISTS Video";
            String user = "DROP TABLE IF EXISTS User";

            Statement stmt = null;
            int updateCount = 0;

            try {
                stmt = conn.createStatement();
                stmt.executeUpdate(watch);

                updateCount++;

            } catch (SQLException e) {
                e.printStackTrace();
            }try {
                stmt = conn.createStatement();
                stmt.executeUpdate(comment);

                updateCount++;

            } catch (SQLException e) {
                e.printStackTrace();
            }try {
                stmt = conn.createStatement();
                stmt.executeUpdate(video);

                updateCount++;

            } catch (SQLException e) {
                e.printStackTrace();
            }try {
                stmt = conn.createStatement();
                stmt.executeUpdate(user);

                updateCount++;

            } catch (SQLException e) {
                e.printStackTrace();
            } finally {
                try {
                    // Close connection
                    if (stmt != null) {
                        stmt.close();
                    }
                    if (conn != null) {
                        conn.close();
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }

            return updateCount;

        }


        /**
         * Should insert an array of User into the database.
         *
         * @return Number of rows inserted successfully.
         */
        public int insertUser(User[] users){

            String insertU = "INSERT INTO User(userID, userName, email, password, status) "
                    + "VALUES(?,?,?,?,?)";
            PreparedStatement stmt;
            int updateCount = 0;

            try {
                    stmt = conn.prepareStatement(insertU);

                    for (User x : users) {
                        stmt.setInt(1, x.getUserID());
                        stmt.setString(2, x.getUserName());
                        stmt.setString(3, x.getEmail());
                        stmt.setString(4, x.getPassword());
                        stmt.setString(5, x.getStatus());

                        stmt.addBatch();

                    }
                    int[] temp = stmt.executeBatch();
                    updateCount = temp.length;
            } catch (SQLException e) {
                e.printStackTrace();
            }

            return updateCount;

        }

        /**
         * Should insert an array of Video into the database.
         *
         * @return Number of rows inserted successfully.
         */
        public int insertVideo(Video[] videos){

            String insertV = "INSERT INTO Video(videoID, userID, videoTitle, likeCount, dislikeCount, datePublished) "
                    + "VALUES(?,?,?,?,?,?)";
            PreparedStatement stmt;
            int updateCount = 0;

            try {
                stmt = conn.prepareStatement(insertV);

                for (Video x : videos) {
                    stmt.setInt(1, x.getVideoID());
                    stmt.setInt(2, x.getUserID());
                    stmt.setString(3, x.getVideoTitle());
                    stmt.setInt(4, x.getLikeCount());
                    stmt.setInt(5, x.getDislikeCount());
                    stmt.setString(6, x.getDatePublished());
                    stmt.addBatch();
                }

                int[] temp = stmt.executeBatch();
                updateCount = temp.length;

            } catch (SQLException e) {
                e.printStackTrace();
            }

            return updateCount;

        }

        /**
         * Should insert an array of Comment into the database.
         *
         * @return Number of rows inserted successfully.
         */
        public int insertComment(Comment[] comments){

            String insertC = "INSERT INTO Comment(commentID, userID, videoID, commentText, dateCommented) "
                    + "VALUES(?,?,?,?,?)";
            PreparedStatement stmt;
            int updateCount = 0;

            try {
                stmt = conn.prepareStatement(insertC);

                for (Comment x : comments) {
                    stmt.setInt(1, x.getCommentID());
                    stmt.setInt(2, x.getUserID());
                    stmt.setInt(3, x.getVideoID());
                    stmt.setString(4, x.getCommentText());
                    stmt.setString(5, x.getDateCommented());
                    stmt.addBatch();
                }

                int[] temp = stmt.executeBatch();
                updateCount = temp.length;

            } catch (SQLException e) {
                e.printStackTrace();
            }

            return updateCount;
        }

        /**
         * Should insert an array of Watch into the database.
         *
         * @return Number of rows inserted successfully.
         */
        public int insertWatch(Watch[] watchEntries){

            String insertW = "INSERT INTO Watch(userID, videoID, dateWatched) "
                    + "VALUES(?,?,?)";
            PreparedStatement stmt;
            int updateCount = 0;

            try {
                stmt = conn.prepareStatement(insertW);

                for (Watch x : watchEntries) {
                    stmt.setInt(1, x.getUserID());
                    stmt.setInt(2, x.getVideoID());
                    stmt.setString(3, x.getDateWatched());
                    stmt.addBatch();
                }

                int[] temp = stmt.executeBatch();
                updateCount = temp.length;

            } catch (SQLException e) {
                e.printStackTrace();
            }

            return updateCount;

        }


        /**
         * Get videos which have higher likeCount than dislikeCount
         *
         * @return QueryResult.VideoTitleLikeCountDislikeCountResult[]
         */
        public QueryResult.VideoTitleLikeCountDislikeCountResult[] question3(){

            String q3 = "SELECT V.videoTitle, V.likeCount, V.dislikeCount " +
                    "FROM Video V " +
                    "WHERE V.likeCount > V.dislikeCount " +
                    "ORDER BY V.videoTitle ASC ";
            Statement stmt;
            ArrayList<QueryResult.VideoTitleLikeCountDislikeCountResult> resultList = new ArrayList<>();

            try {

                stmt = conn.createStatement();

                ResultSet rs = stmt.executeQuery(q3);
                while (rs.next()) {
                    String videoTitle = rs.getString("V.videoTitle");
                    int likeCount = rs.getInt("likeCount");
                    int dislikeCount = rs.getInt("dislikeCount");
                    resultList.add(new QueryResult.VideoTitleLikeCountDislikeCountResult(videoTitle, likeCount, dislikeCount));
                }

            } catch (SQLException e) {
                e.printStackTrace();
            }
            QueryResult.VideoTitleLikeCountDislikeCountResult[] result = new QueryResult.VideoTitleLikeCountDislikeCountResult[resultList.size()];
            result = resultList.toArray(result);

            return result;
        }

        /**
         * Get the videos commented by the given userID
         *
         * @param userID given userID
         *
         * @return QueryResult.VideoTitleUserNameCommentTextResult[]
         */
        public QueryResult.VideoTitleUserNameCommentTextResult[] question4(Integer userID){

            String q4 = "SELECT DISTINCT V.videoTitle, U.userName, C.commentText " +
                    "FROM User U, Comment C, Video V " +
                    "WHERE U.userID = ? AND " +
                    "C.videoID = V.videoID AND " +
                    "U.userID = C.userID " +
                    "ORDER BY V.videoTitle ASC";
            PreparedStatement stmt;
            ArrayList<QueryResult.VideoTitleUserNameCommentTextResult> resultList = new ArrayList<>();

            try {
                stmt = conn.prepareStatement(q4);
                stmt.setInt(1,userID);

                ResultSet rs = stmt.executeQuery();
                while (rs.next()) {
                    String videoTitle = rs.getString("V.videoTitle");
                    String userName = rs.getString("U.userName");
                    String commentText = rs.getString("C.commentText");
                    resultList.add(new QueryResult.VideoTitleUserNameCommentTextResult(videoTitle, userName, commentText));
                }

            } catch (SQLException e) {
                e.printStackTrace();
            }
            QueryResult.VideoTitleUserNameCommentTextResult[] result = new QueryResult.VideoTitleUserNameCommentTextResult[resultList.size()];
            result = resultList.toArray(result);

            return result;
        }

        /**
         * Find the oldest published video for a given userID which doesn’t contain ”VLOG" in its title
         *
         * @param userID given userID
         * @return QueryResult.VideoTitleUserNameDatePublishedResult[]
         */
        public QueryResult.VideoTitleUserNameDatePublishedResult[] question5(Integer userID){

            String q5 = "SELECT V.videoTitle, U.userName, V.datePublished " +
                    "FROM User U, Video V " +
                    "WHERE U.userID = V.userID AND " +
                    "U.userID = ? AND " +
                    "V.datePublished = (SELECT min(V.datePublished) FROM Video V, User U WHERE U.userID = V.userID AND U.userID = ? AND " +
                    "V.videoTitle NOT IN (SELECT videoTitle FROM Video WHERE videoTitle LIKE '%VLOG%')) " +
                    "ORDER BY V.videoTitle ASC";
            PreparedStatement stmt;
            ArrayList<QueryResult.VideoTitleUserNameDatePublishedResult> resultList = new ArrayList<>();

            try {
                stmt = conn.prepareStatement(q5);
                stmt.setInt(1,userID);
                stmt.setInt(2,userID);

                ResultSet rs = stmt.executeQuery();
                while (rs.next()) {
                    String videoTitle = rs.getString("V.videoTitle");
                    String userName = rs.getString("U.userName");
                    String datePublished = rs.getString("V.datePublished");
                    resultList.add(new QueryResult.VideoTitleUserNameDatePublishedResult(videoTitle, userName, datePublished));
                }

            } catch (SQLException e) {
                e.printStackTrace();
            }
            QueryResult.VideoTitleUserNameDatePublishedResult[] result = new QueryResult.VideoTitleUserNameDatePublishedResult[resultList.size()];
            result = resultList.toArray(result);

            return result;
        }


        /**
         * Get the trending top three videos for a given time interval
         *
         * YOU SHOULD INCLUDE dateStart and dateEnd in the result, it is a CLOSED interval.
         *
         * @param dateStart start date of the interval
         * @param dateEnd end date of the interval
         *
         * @return QueryResult.VideoTitleUserNameNumOfWatchResult[]
         */
        public QueryResult.VideoTitleUserNameNumOfWatchResult[] question6(String dateStart, String dateEnd){
            String q6 = "SELECT V.videoTitle, U.userName, COUNT(*) as number " +
                    "FROM User U, Video V, Watch W " +
                    "WHERE U.userID = V.userID AND " +
                    "W.videoID = V.videoID AND " +
                    "V.datePublished >= ? AND " +
                    "V.datePublished <= ? "+
                    " GROUP BY V.videoID " +
                    "ORDER BY number DESC " +
                    "LIMIT 3 ";
            PreparedStatement stmt;
            ArrayList<QueryResult.VideoTitleUserNameNumOfWatchResult> resultList = new ArrayList<>();

            try {
                stmt = conn.prepareStatement(q6);
                stmt.setString(1,dateStart);
                stmt.setString(2,dateEnd);
                ResultSet rs = stmt.executeQuery();

                while (rs.next()) {
                    String videoTitle = rs.getString("V.videoTitle");
                    String userName = rs.getString("U.userName");
                    int number = rs.getInt("number");
                    resultList.add(new QueryResult.VideoTitleUserNameNumOfWatchResult(videoTitle, userName, number));
                }

            } catch (SQLException e) {
                e.printStackTrace();
            }
            QueryResult.VideoTitleUserNameNumOfWatchResult[] result = new QueryResult.VideoTitleUserNameNumOfWatchResult[resultList.size()];
            result = resultList.toArray(result);

            return result;
        }


        /**
         * Get users and the number of videos watched only by her/him.
         *
         *
         * @return QueryResult.UserIDUserNameEmailResult[]
         */
        public QueryResult.UserIDUserNameNumOfVideosWatchedResult[] question7(){
            String q7 = "SELECT U.userID, U.userName, COUNT(*) as number " +
                    "FROM User U, Watch W " +
                    "WHERE U.userID = W.userID AND " +
                    "W.videoID NOT IN (SELECT W1.videoID FROM Watch W1, Watch W2, User U1, User U2 " +
                    "WHERE W1.userID = U1.userID AND W2.userID = U2.userID AND W1.videoID = W2.videoID AND U2.userID <> U1.userID ) " +
                    "GROUP BY U.userID, U.userName " +
                    "ORDER BY U.userID ASC";
            Statement stmt;
            ArrayList<QueryResult.UserIDUserNameNumOfVideosWatchedResult> resultList = new ArrayList<>();

            try {
                stmt = conn.createStatement();

                ResultSet rs = stmt.executeQuery(q7);
                while (rs.next()) {
                    int userID = rs.getInt("U.userID");
                    String userName = rs.getString("U.userName");
                    int number = rs.getInt("number");
                    resultList.add(new QueryResult.UserIDUserNameNumOfVideosWatchedResult(userID, userName, number));
                }

            } catch (SQLException e) {
                e.printStackTrace();
            }
            QueryResult.UserIDUserNameNumOfVideosWatchedResult[] result = new QueryResult.UserIDUserNameNumOfVideosWatchedResult[resultList.size()];
            result = resultList.toArray(result);

            return result;
        }


        /**
         * Get users who have watched and commented all of their own videos
         *
         * @return QueryResult.UserIDUserNameEmailResult[]
         */
        public QueryResult.UserIDUserNameEmailResult[]  question8(){

            String q7 = "SELECT DISTINCT U.userID, U.userName, U.email " +
                    "FROM User U, Video V " +
                    "WHERE U.userID = V.userID AND NOT EXISTS (SELECT V.videoID " +
                    "FROM Video V " +
                    "WHERE U.userID = V.userID AND " +
                    "U.userID NOT IN (SELECT W.userID " +
                    "FROM Watch W " +
                    "WHERE W.videoID = V.videoID) " +
                    "UNION " +
                    "SELECT V.videoID " +
                    "FROM Video V " +
                    "WHERE U.userID = V.userID AND " +
                    "U.userID NOT IN (SELECT C.userID " +
                    "FROM Comment C " +
                    "WHERE C.videoID = V.videoID)) " +
                    "ORDER BY U.userID ASC";
            Statement stmt;
            ArrayList<QueryResult.UserIDUserNameEmailResult> resultList = new ArrayList<>();

            try {
                stmt = conn.createStatement();

                ResultSet rs = stmt.executeQuery(q7);
                while (rs.next()) {
                    int userID = rs.getInt("U.userID");
                    String userName = rs.getString("U.userName");
                    String email = rs.getString("U.email");
                    resultList.add(new QueryResult.UserIDUserNameEmailResult(userID, userName, email));
                }

            } catch (SQLException e) {
                e.printStackTrace();
            }
            QueryResult.UserIDUserNameEmailResult[] result = new QueryResult.UserIDUserNameEmailResult[resultList.size()];
            result = resultList.toArray(result);

            return result;

        }

        /**
         * Get users that has at least one watch but no comments
         *
         * @return QueryResult.UserIDUserNameEmailResult[]
         */
        public QueryResult.UserIDUserNameEmailResult[]  question9(){

            String q7 = "SELECT DISTINCT U.userID, U.userName, U.email " +
                    "FROM User U, Video V, Watch W " +
                    "WHERE U.userID = W.userID AND " +
                    "U.userID NOT IN (SELECT U.userID FROM Comment C WHERE U.userID = C.userID) "+
                    "ORDER BY U.userID ASC";
            Statement stmt;
            ArrayList<QueryResult.UserIDUserNameEmailResult> resultList = new ArrayList<>();

            try {
                stmt = conn.createStatement();

                ResultSet rs = stmt.executeQuery(q7);
                while (rs.next()) {
                    int userID = rs.getInt("U.userID");
                    String userName = rs.getString("U.userName");
                    String email = rs.getString("U.email");
                    resultList.add(new QueryResult.UserIDUserNameEmailResult(userID, userName, email));
                }

            } catch (SQLException e) {
                e.printStackTrace();
            }
            QueryResult.UserIDUserNameEmailResult[] result = new QueryResult.UserIDUserNameEmailResult[resultList.size()];
            result = resultList.toArray(result);

            return result;

        }

        /**
         *  Update the users’ status to ”verified” if the view count of all their videos in total is more than the given number
         *
         * @param givenViewCount the threshold to use in the query
         *
         * @return int return the number of rows affected
         */
        public int question10(int givenViewCount){

            String q10 = "UPDATE User SET status = 'verified' WHERE userID IN (SELECT S.userID " +
                    "FROM (SELECT * FROM User) AS S " +
                    "WHERE EXISTS (SELECT S.userID, COUNT(*) as number FROM Video V, Watch W WHERE S.userID = V.userID AND " +
                    "V.videoID = W.videoID " +
                    "GROUP BY S.userID " +
                    "HAVING number > ?))";

            PreparedStatement stmt;
            int updateCount = 0;
            try {
                stmt = conn.prepareStatement(q10);
                stmt.setInt(1,givenViewCount);

                updateCount = stmt.executeUpdate();

            } catch (SQLException e) {
                e.printStackTrace();
            }

            return updateCount;

        }

        /**
         *  Given a video id and a string, update the title of the video with the given id to the new string
         *
         * @param videoID the identifier for the video to be updated
         * @param newTitle the new title that will replace the old one
         *
         * @return int return the number of rows affected
         */
        public int question11(Integer videoID, String newTitle){

            String q11 = "UPDATE Video SET videoTitle = ? WHERE videoID = ?";

            PreparedStatement stmt;
            int updateCount = 0;
            try {
                stmt = conn.prepareStatement(q11);
                stmt.setString(1,newTitle);
                stmt.setInt(2,videoID);
                updateCount = stmt.executeUpdate();

            } catch (SQLException e) {
                e.printStackTrace();
            }

            return updateCount;

        }

        /**
         *   Delete the video(s) with the given video title.
         *
         * @param videoTitle
         *
         * @return int return the number of rows in the Video table after delete operation
         */
        public int question12(String videoTitle){

            String q12 = "DELETE FROM Video WHERE videoTitle = ? ";
            String q12last = "SELECT COUNT(*) as number FROM Video ";

            PreparedStatement stmt;
            Statement stmt2;
            ResultSet rs;
            int numberOfRows = 0;
            try {
                stmt = conn.prepareStatement(q12);
                stmt.setString(1, videoTitle);
                stmt.executeUpdate();
                stmt.close();

                stmt2 = conn.createStatement();
                rs = stmt2.executeQuery(q12last);
                while (rs.next()) {
                    numberOfRows = rs.getInt("number");
                }

            } catch (SQLException e) {
                e.printStackTrace();
            }

            return numberOfRows;

        }

}