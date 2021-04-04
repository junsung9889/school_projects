import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Scanner;
import java.sql.ResultSet;
import java.sql.DriverManager;

public class JDBCexample {
    private static Scanner scanner;
    public static void main(String[] args){
            menu();
    }
    private static void menu(){
        int num = 0;
        do{
            System.out.println();
            System.out.println("**********************************************");
            System.out.println("*            1.  User     Login              *");
            System.out.println("*            2.  User     Create             *");
            System.out.println("*            3.  Provider Login              *");
            System.out.println("*            4.  Provider Create             *");
            System.out.println("*            5.  Manage   System             *");
            System.out.println("*            6.  Exit                        *");
            System.out.println("**********************************************");
            System.out.print("Choose next step : ");
            scanner = new Scanner(System.in);
            num = scanner.nextInt();
            switch(num){
                case 1:
                    user_login();
                    break;
                case 2:
                    user_create();
                    break;
                case 3:
                    provider_login();
                    break;
                case 4:
                    provider_create();
                    break;
                case 5:
                    manage_system();
                    break;
                case 6:
                    System.out.println("---------Exit Program----------");
                    break;
                default:
                    System.out.println("----------Wrong Number---------");
                    break;
            }
        }while(num != 6);
    }
    private static void user_login(){
        try { 
            Class.forName ("com.mysql.jdbc.Driver"); 
            Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2016312160", "2016312160", "2wnstjd!");
            Statement stmt = conn.createStatement();
            ResultSet rset = stmt.executeQuery("select name from user");
            System.out.println();
            System.out.println("----------User List----------");
            while (rset.next()) {
                System.out.printf("%15s\n", rset.getString("name"));
            }
            System.out.println("-----------------------------");
            System.out.println();
            System.out.print("Who do you want to login? : ");
            scanner = new Scanner(System.in);
            String str = scanner.nextLine();
            rset = stmt.executeQuery("select * from user");
            while(rset.next()){
                if((rset.getString("name")).equals(str)){
                    System.out.println();
                    System.out.println("-----------------------------------------------------------------------------------------------------------------------------------");
                    System.out.printf("%8s|%8s|%15s|%13s|%10s|%15s|%18s|%12s|%12s|%12s|\n",
                        "name","address","account","phone","birthday","access_history",
                        "subscription_fee","amount_due","date_joined","data_joined"
                    );
                    System.out.println("-----------------------------------------------------------------------------------------------------------------------------------");
                    System.out.printf("%8s|%8s|%15s|%13s|%10s|%15s|%18d|%12d|%12s|%12d|\n", 
                           rset.getString("name"),                                 
                           rset.getString("address"),                                 
                           rset.getString("account"),
                           rset.getString("phone"),
                           rset.getDate("birthday"),
                           rset.getString("access_history"),
                           rset.getInt("subscription_fee"),
                           rset.getInt("amount_due"),
                           rset.getDate("date_joined"),
                           rset.getInt("data_joined")
                    );
                    System.out.println("---------------------------------------------------------------------------------------------------------------------------------");                               
                }
            }
            int num = 0;
            do{
                System.out.println();
                System.out.println("**********************************************");
                System.out.println("*            1.  Show      Items             *");
                System.out.println("*            2.  Download  Items             *");
                System.out.println("*            3.  Pay       Subscription      *");
                System.out.println("*            4.  Cancel    Subscription      *");
                System.out.println("*            5.  Back                        *");
                System.out.println("**********************************************");
                System.out.print("Choose next step : ");
                scanner = new Scanner(System.in);
                num = scanner.nextInt();
                switch(num){
                case 1:
                    show_items(str);
                    break;
                case 2:
                    download_items(str);
                    break;
                case 3:
                    pay_subscription(str);
                    break;
                case 4:
                    cancel_subscription(str);
                    break;
                case 5:
                    System.out.println("---------Go Back----------");
                    break;
                default:
                    System.out.println("----------Wrong Number---------");
                    break;
            }
            }while(num != 5 && num != 4);
        }
        catch (SQLException sqle) {         
            System.out.println("SQLException : " + sqle);        
        }
        catch (Exception e) {         
            System.out.println("Exception : " + e);        
        }
    }
    private static void show_items(String str){
        try { 
            Class.forName ("com.mysql.jdbc.Driver"); 
            Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2016312160", "2016312160", "2wnstjd!");
            Statement stmt = conn.createStatement();
            ResultSet rset = stmt.executeQuery("select * from download");
            System.out.println();
            System.out.println("------------Your Items------------");
            System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
            System.out.printf("%10s|%10s|%15s|%15s|%15s|%15s|\n",
                "user_name","item_id","provider_name","item_name","item_type","item_size"
            );
            System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
            while (rset.next()) {
                if((rset.getString("user_name")).equals(str)){
                    System.out.printf("%10s|%10s|%15s|%15s|%15s|%15d|\n", 
                           rset.getString("user_name"),                                 
                           rset.getString("item_id"),                                 
                           rset.getString("provider_name"),
                           rset.getString("item_name"),
                           rset.getString("item_type"),
                           rset.getInt("item_size")
                    );
                }
            }
            System.out.println("---------------------------------------------------------------------------------------------------------------------------------");                               
            System.out.println("----------------------------------");
            int num = 0;
            do{
                System.out.println();
                System.out.println("**********************************************");
                System.out.println("*            1.  More  Information           *");
                System.out.println("*            2.  Back                        *");
                System.out.println("**********************************************");
                System.out.print("Choose next step : ");
                scanner = new Scanner(System.in);
                num = scanner.nextInt();
                switch(num){
                case 1:
                    more_information();
                    break;
                case 2:
                    System.out.println("---------Go Back----------");
                    break;
                default:
                    System.out.println("----------Wrong Number---------");
                    break;
                }
            }while(num != 2);
        }
        catch (SQLException sqle) {         
            System.out.println("SQLException : " + sqle);        
        }
        catch (Exception e) {         
            System.out.println("Exception : " + e);        
        }
    }
    private static void more_information(){
        try {
            System.out.print("Which item(name) do you want to know? : ");
            scanner = new Scanner(System.in);
            String item_name = scanner.nextLine();
            Class.forName ("com.mysql.jdbc.Driver"); 
            Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2016312160", "2016312160", "2wnstjd!");
            Statement stmt = conn.createStatement();
            ResultSet rset = stmt.executeQuery("select * from item");
            System.out.println();
            System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
            System.out.printf("%20s|%15s|%15s|%15s|%10s|%10s|%10s|%10s|\n",
                "id","provider_name","name","type","size","author","subject","machine"
            );
            System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
            while (rset.next()) {
                if((rset.getString("name")).equals(item_name)){
                    System.out.printf("%20s|%15s|%15s|%15s|%10d|%10s|%10s|%10s|\n",                                 
                           rset.getString("id"),                                 
                           rset.getString("provider_name"),
                           rset.getString("name"),
                           rset.getString("type"),
                           rset.getInt("size"),
                           rset.getString("author"),
                           rset.getString("subject"),
                           rset.getString("machine")
                    );
                    System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                    System.out.printf("%20s|%15s|%15s|%48s|\n",
                        "os","language","local_storage","short_description"
                    );
                    System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                    System.out.printf("%20s|%15s|%15d|%48s|\n",                                 
                           rset.getString("os"),                                 
                           rset.getString("language"),
                           rset.getInt("local_storage"),
                           rset.getString("short_description")
                    );
                    System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                    System.out.printf("%20s|%15s|%15s|%15s|\n",
                        "address_retrieved","viewer","audio","last_updated"
                    );
                    System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                    System.out.printf("%20d|%15s|%15s|%15s|\n",                             
                           rset.getInt("address_retrieved"),                                 
                           rset.getString("viewer"),
                           rset.getString("audio"),
                           rset.getString("last_updated")
                    );
                }
            }
            System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
        }
        catch (SQLException sqle) {         
            System.out.println("SQLException : " + sqle);        
        }
        catch (Exception e) {         
            System.out.println("Exception : " + e);        
        }
    }
    private static void download_items(String str){
        try {
            System.out.println();
            Class.forName ("com.mysql.jdbc.Driver"); 
            Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2016312160", "2016312160", "2wnstjd!");
            Statement stmt = conn.createStatement();
            System.out.println("---------Items list-----------");
            System.out.println("Program");
            print_items("program");
            System.out.println("Document");
            print_items("document");
            System.out.println("Image");
            print_items("image");
            System.out.println("Video_clip");
            print_items("video_clip");
            System.out.println("Sound_clip");
            print_items("sound_clip");
            System.out.println("------------------------------------");
            System.out.print("Do you want to see by a given category?(yes/no) : ");
            scanner = new Scanner(System.in);
            String answer = scanner.nextLine();
            if(answer.equals("yes")){
                System.out.print("Input name of hardware: ");
                scanner = new Scanner(System.in);
                String hardware = scanner.nextLine();
                System.out.print("Input name of operating system: ");
                scanner = new Scanner(System.in);
                String os = scanner.nextLine();
                System.out.println("---------Items list-----------");
                System.out.println("Program");
                print_items_category("program",hardware,os);
                System.out.println("Document");
                print_items_category("document",hardware,os);
                System.out.println("Image");
                print_items_category("image",hardware,os);
                System.out.println("Video_clip");
                print_items_category("video_clip",hardware,os);
                System.out.println("Sound_clip");
                print_items_category("sound_clip",hardware,os);
                System.out.println("------------------------------------");
            }
            System.out.print("Which item(name) do you want to download? : ");
            scanner = new Scanner(System.in);
            String item_name = scanner.nextLine();
            String id = null, provider_name = null, type = null,size = null;
            if(item_name != null){
                ResultSet rset = stmt.executeQuery("select * from item where name = '"+item_name+"'");
                rset.next();
                id = rset.getString("id");
                provider_name = rset.getString("provider_name");
                type = rset.getString("type");
                size = Integer.toString(rset.getInt("size"));
            }
            if(type.equals("video_clip")){
                ResultSet rset = stmt.executeQuery("select * from download where user_name = '"+str+"'");
                int viewer_count = 0;
                int audio_count = 0;
                while(rset.next()){
                    if(rset.getString("item_name").equals("viewer_program"))
                        viewer_count++;
                    if(rset.getString("item_name").equals("audio_program"))
                        audio_count++;
                }
                if(viewer_count == 0 || audio_count == 0){
                    System.out.println("You have to download viewer_program & audio_program first!");
                    return;
                }
            }
            if(type.equals("sound_clip")){
                ResultSet rset = stmt.executeQuery("select * from download where user_name = '"+str+"'");
                int audio_count = 0;
                while(rset.next()){
                    if(rset.getString("item_name").equals("audio_program"))
                        audio_count++;
                }
                if(audio_count == 0){
                    System.out.println("You have to download audio_program first!");
                    return;
                }
            }
            stmt.executeUpdate("insert into download values('"+str+"','"+id+"','"+provider_name+"','"+item_name+"','"+type+"','"+size+"')");
            stmt.executeUpdate("update item set address_retrieved = address_retrieved + 1 where id = '"+id+"'");
            System.out.println();
            System.out.println("----------Download Complete-----------");
            System.out.println("----------Into "+str+"'s storage---------");
            System.out.println();
        }
        catch (SQLException sqle) {         
            System.out.println("SQLException : " + sqle);        
        }
        catch (Exception e) {         
            System.out.println("Exception : " + e);        
        }
    }
    private static void print_items(String type){
        try {
            Class.forName ("com.mysql.jdbc.Driver"); 
            Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2016312160", "2016312160", "2wnstjd!");
            Statement stmt = conn.createStatement();
            ResultSet rset = stmt.executeQuery("select * from item where type ='"+type+"'");
            while (rset.next()) {
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                System.out.printf("%20s|%15s|%15s|%15s|%10s|%10s|%10s|%10s|\n",
                    "id","provider_name","name","type","size","author","subject","machine"
                );
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                System.out.printf("%20s|%15s|%15s|%15s|%10d|%10s|%10s|%10s|\n",                                 
                       rset.getString("id"),                                 
                       rset.getString("provider_name"),
                       rset.getString("name"),
                       rset.getString("type"),
                       rset.getInt("size"),
                       rset.getString("author"),
                       rset.getString("subject"),
                       rset.getString("machine")
                );
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                System.out.printf("%20s|%15s|%15s|%48s|\n",
                    "os","language","local_storage","short_description"                    
                );
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                System.out.printf("%20s|%15s|%15d|%48s|\n",                                 
                       rset.getString("os"),                                 
                       rset.getString("language"),
                       rset.getInt("local_storage"),
                       rset.getString("short_description")
                );
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                System.out.printf("%20s|%15s|%15s|%15s|\n",
                    "address_retrieved","viewer","audio","last_updated"
                );
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");                    
                System.out.printf("%20d|%15s|%15s|%15s|\n",                             
                       rset.getInt("address_retrieved"),                                 
                       rset.getString("viewer"),
                       rset.getString("audio"),
                       rset.getString("last_updated")
                );
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
            }
            System.out.println();
        }
        catch (SQLException sqle) {         
            System.out.println("SQLException : " + sqle);        
        }
        catch (Exception e) {         
            System.out.println("Exception : " + e);        
        }
    }
    private static void print_items_category(String type,String hardware,String os){
        try {
            Class.forName ("com.mysql.jdbc.Driver"); 
            Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2016312160", "2016312160", "2wnstjd!");
            Statement stmt = conn.createStatement();
            ResultSet rset = stmt.executeQuery("select * from item where type = '"+type+"' and machine = '"+hardware+"' and os = '"+os+"'");
            while (rset.next()) {
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                System.out.printf("%20s|%15s|%15s|%15s|%10s|%10s|%10s|%10s|\n",
                    "id","provider_name","name","type","size","author","subject","machine"
                );
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                System.out.printf("%20s|%15s|%15s|%15s|%10d|%10s|%10s|%10s|\n",                                 
                       rset.getString("id"),                                 
                       rset.getString("provider_name"),
                       rset.getString("name"),
                       rset.getString("type"),
                       rset.getInt("size"),
                       rset.getString("author"),
                       rset.getString("subject"),
                       rset.getString("machine")
                );
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                System.out.printf("%20s|%15s|%15s|%48s|\n",
                    "os","language","local_storage","short_description"                    
                );
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                System.out.printf("%20s|%15s|%15d|%48s|\n",                                 
                       rset.getString("os"),                                 
                       rset.getString("language"),
                       rset.getInt("local_storage"),
                       rset.getString("short_description")
                );
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                System.out.printf("%20s|%15s|%15s|%15s|\n",
                    "address_retrieved","viewer","audio","last_updated"
                );
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");                    
                System.out.printf("%20d|%15s|%15s|%15s|\n",                             
                       rset.getInt("address_retrieved"),                                 
                       rset.getString("viewer"),
                       rset.getString("audio"),
                       rset.getString("last_updated")
                );
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
            }
            System.out.println();
        }
        catch (SQLException sqle) {         
            System.out.println("SQLException : " + sqle);        
        }
        catch (Exception e) {         
            System.out.println("Exception : " + e);        
        }
    }
    private static void pay_subscription(String str){
        try {
            Class.forName ("com.mysql.jdbc.Driver"); 
            Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2016312160", "2016312160", "2wnstjd!");
            Statement stmt = conn.createStatement();
            ResultSet rSet = stmt.executeQuery("select amount_due from user where name = '" +str+"'");
            rSet.next();
            int amount_due = rSet.getInt("amount_due");
            System.out.println("Your amount_due to pay : " + amount_due);
            System.out.print("How much do you want to pay? : ");
            scanner = new Scanner(System.in);
            String money = scanner.nextLine();
            stmt.executeUpdate("update user set amount_due = amount_due - '"+money+"' where name = '" +str+"'");
            System.out.println("--------Subscription Fee Paied---------");
            int remain = amount_due - Integer.parseInt(money);
            System.out.println("Your remain amount_due : "+remain);
        }
        catch (SQLException sqle) {         
            System.out.println("SQLException : " + sqle);        
        }
        catch (Exception e) {         
            System.out.println("Exception : " + e);        
        }
    }
    private static void cancel_subscription(String str){
        System.out.print("Do you really want to cancel subscription?(yes/no) : ");
        scanner = new Scanner(System.in);
        String answer = scanner.nextLine();
        if(answer.equals("yes")){
            try {
                Class.forName ("com.mysql.jdbc.Driver"); 
                Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2016312160", "2016312160", "2wnstjd!");
                Statement stmt = conn.createStatement();
                stmt.executeUpdate("delete from user where name = '" +str+"'");
                System.out.println("--------Subscription Cancelled---------");
            }
            catch (SQLException sqle) {         
                System.out.println("SQLException : " + sqle);        
            }
            catch (Exception e) {         
                System.out.println("Exception : " + e);        
            }
        }
    }
    private static void user_create(){
        scanner = new Scanner(System.in);
        System.out.print("What is your name ? : ");
        String name = scanner.nextLine();
        System.out.print("What is your address ? : ");
        String address = scanner.nextLine();
        System.out.print("What is your account ? : ");
        String account = scanner.nextLine();
        System.out.print("What is your phone number ? : ");
        String phone = scanner.nextLine();
        System.out.print("When is your birthday ? : ");
        String birthday = scanner.nextLine();
        System.out.print("What date is it today ? : ");
        String date_joined = scanner.nextLine();
        try {
            Class.forName ("com.mysql.jdbc.Driver"); 
            Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2016312160", "2016312160", "2wnstjd!");
            Statement stmt = conn.createStatement();
            stmt.executeUpdate("insert into user(name,address,account,phone,birthday,date_joined) values('"+name+"','"+address+"','"+account+"','"+phone+"','"+birthday+"','"+date_joined+"')");
            System.out.println("--------User "+name+" Enrolled---------");
        }
        catch (SQLException sqle) {         
            System.out.println("SQLException : " + sqle);        
        }
        catch (Exception e) {         
            System.out.println("Exception : " + e);        
        }
    }
    private static void provider_login(){
        try { 
            Class.forName ("com.mysql.jdbc.Driver"); 
            Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2016312160", "2016312160", "2wnstjd!");
            Statement stmt = conn.createStatement();
            ResultSet rset = stmt.executeQuery("select name from provider");
            System.out.println();
            System.out.println("----------User List----------");
            while (rset.next()) {
                System.out.printf("%15s\n", rset.getString("name"));
            }
            System.out.println("-----------------------------");
            System.out.println();
            System.out.print("Who do you want to login? : ");
            scanner = new Scanner(System.in);
            String str = scanner.nextLine();
            rset = stmt.executeQuery("select * from provider");
            while(rset.next()){
                if((rset.getString("name")).equals(str)){
                    System.out.println();
                    System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                    System.out.printf("%10s|%10s|%15s|%15s|%12s|%15s|%10s|%12s|%12s|%12s|\n",
                        "name","address","account","phone","birthday",
                        "joining_fee","due_you","stil_to_you","to_provider","data_joined"
                    );
                    System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                    System.out.printf("%10s|%10s|%15s|%15s|%12s|%15d|%10d|%12d|%12d|%12d|\n", 
                           rset.getString("name"),                                 
                           rset.getString("address"),                                 
                           rset.getString("account"),
                           rset.getString("phone"),
                           rset.getDate("birthday"),
                           rset.getInt("joining_fee"),
                           rset.getInt("due_you"),
                           rset.getInt("stil_to_you"),
                           rset.getInt("to_provider"),
                           rset.getInt("data_joined")
                    );
                    System.out.println("---------------------------------------------------------------------------------------------------------------------------------");                               
                }
            }
            int num = 0;
            do{
                System.out.println();
                System.out.println("*********************************************");
                System.out.println("*            1.  My        Item             *");
                System.out.println("*            2.  Upload    Item             *");
                System.out.println("*            3.  Back                       *");
                System.out.println("*********************************************");
                System.out.print("Choose next step : ");
                scanner = new Scanner(System.in);
                num = scanner.nextInt();
                switch(num){
                case 1:
                    my_item(str);
                    break;
                case 2:
                    upload_item(str);
                    break;
                case 3:
                    System.out.println("---------Go Back----------");
                    break;
                default:
                    System.out.println("----------Wrong Number---------");
                    break;
            }
            }while(num != 3);
        }
        catch (SQLException sqle) {         
            System.out.println("SQLException : " + sqle);        
        }
        catch (Exception e) {         
            System.out.println("Exception : " + e);        
        }
    }
    private static void my_item(String str){
        try {
            Class.forName ("com.mysql.jdbc.Driver"); 
            Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2016312160", "2016312160", "2wnstjd!");
            Statement stmt = conn.createStatement();
            ResultSet rset = stmt.executeQuery("select * from item where provider_name ='"+str+"'");
            while (rset.next()) {
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                System.out.printf("%20s|%15s|%15s|%15s|%10s|%10s|%10s|%10s|\n",
                    "id","provider_name","name","type","size","author","subject","machine"
                );
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                System.out.printf("%20s|%15s|%15s|%15s|%10d|%10s|%10s|%10s|\n",                                 
                       rset.getString("id"),                                 
                       rset.getString("provider_name"),
                       rset.getString("name"),
                       rset.getString("type"),
                       rset.getInt("size"),
                       rset.getString("author"),
                       rset.getString("subject"),
                       rset.getString("machine")
                );
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                System.out.printf("%20s|%15s|%15s|%48s|\n",
                    "os","language","local_storage","short_description"                    
                );
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                System.out.printf("%20s|%15s|%15d|%48s|\n",                                 
                       rset.getString("os"),                                 
                       rset.getString("language"),
                       rset.getInt("local_storage"),
                       rset.getString("short_description")
                );
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
                System.out.printf("%20s|%15s|%15s|%15s|\n",
                    "address_retrieved","viewer","audio","last_updated"
                );
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");                    
                System.out.printf("%20s|%15s|%15s|%15s|\n",                             
                       rset.getString("address_retrieved"),                                 
                       rset.getString("viewer"),
                       rset.getString("audio"),
                       rset.getString("last_updated")
                );
                System.out.println("---------------------------------------------------------------------------------------------------------------------------------");
            }
            System.out.println();
        }
        catch (SQLException sqle) {         
            System.out.println("SQLException : " + sqle);        
        }
        catch (Exception e) {         
            System.out.println("Exception : " + e);        
        }
    }
    private static void upload_item(String str){
        try { 
            Class.forName ("com.mysql.jdbc.Driver"); 
            Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2016312160", "2016312160", "2wnstjd!");
            Statement stmt = conn.createStatement();
            String id = null,provider_name = str,name = null,type = null,
            author = null,subject = null,machine = null,os = null,language = null ,short_description = null,
            viewer = null,audio = null,last_updated = null,size = null;
            scanner = new Scanner(System.in);
            System.out.print("What is ID? : ");
            id = scanner.nextLine();
            System.out.print("What is name? : ");
            name = scanner.nextLine();
            System.out.print("What is type? : ");
            type = scanner.nextLine();
            System.out.print("What is author? : ");
            author = scanner.nextLine();
            System.out.print("What is subject? : ");
            subject = scanner.nextLine();
            System.out.print("What is machine? : ");
            machine = scanner.nextLine();
            System.out.print("What is os? : ");
            os = scanner.nextLine();
            System.out.print("What is language? : ");
            language = scanner.nextLine();
            System.out.print("What is short_description? : ");
            short_description = scanner.nextLine();
            System.out.print("What is viewer? : ");
            viewer = scanner.nextLine();
            System.out.print("What is audio? : ");
            audio = scanner.nextLine();
            System.out.print("When is last_updated? : ");
            last_updated = scanner.nextLine();
            System.out.print("What is size? : ");
            size = scanner.nextLine();
            stmt.executeUpdate("insert into item(id,provider_name,name,type,size,author,subject,machine,os,language,short_description,viewer,audio,last_updated) values('"+id+"','"+provider_name+"','"+name+"','"+type+"','"+size+"','"+author+"','"+subject+"','"+machine+"','"+os+"','"+language+"','"+short_description+"','"+viewer+"','"+audio+"','"+last_updated+"')");
            System.out.println("---------upload successed------------");
        }
        catch (SQLException sqle) {         
            System.out.println("SQLException : " + sqle);        
        }
        catch (Exception e) {         
            System.out.println("Exception : " + e);        
        }
    }
    private static void provider_create(){
        scanner = new Scanner(System.in);
        System.out.print("What is your name ? : ");
        String name = scanner.nextLine();
        System.out.print("What is your address ? : ");
        String address = scanner.nextLine();
        System.out.print("What is your account ? : ");
        String account = scanner.nextLine();
        System.out.print("What is your phone number ? : ");
        String phone = scanner.nextLine();
        System.out.print("When is your birthday ? : ");
        String birthday = scanner.nextLine();
        try {
            Class.forName ("com.mysql.jdbc.Driver"); 
            Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2016312160", "2016312160", "2wnstjd!");
            Statement stmt = conn.createStatement();
            stmt.executeUpdate("insert into provider(name,address,account,phone,birthday) values('"+name+"','"+address+"','"+account+"','"+phone+"','"+birthday+"')");
            System.out.println("--------Provider "+name+" Enrolled---------");
        }
        catch (SQLException sqle) {         
            System.out.println("SQLException : " + sqle);        
        }
        catch (Exception e) {         
            System.out.println("Exception : " + e);        
        }
    }
    private static void manage_system(){
            int num = 0;
            do{
                System.out.println();
                System.out.println("*********************************************");
                System.out.println("*            1.  Send      Bills            *");
                System.out.println("*            2.  Manage    Items            *");
                System.out.println("*            3.  Back                       *");
                System.out.println("*********************************************");
                System.out.print("Choose next step : ");
                scanner = new Scanner(System.in);
                num = scanner.nextInt();
                switch(num){
                case 1:
                    send_bills();
                    break;
                case 2:
                    manage_items();
                    break;
                case 3:
                    System.out.println("---------Go Back----------");
                    break;
                default:
                    System.out.println("----------Wrong Number---------");
                    break;
                }
            }while(num != 3);
    }
    private static void send_bills(){
        try { 
            Class.forName ("com.mysql.jdbc.Driver"); 
            Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2016312160", "2016312160", "2wnstjd!");
            Statement stmt = conn.createStatement();
            ResultSet rset = stmt.executeQuery("select * from user");
            System.out.println("--------User billed start----------");
            while(rset.next()){
                String name = rset.getString("name");
                stmt = conn.createStatement();
                stmt.executeUpdate("update user set amount_due = amount_due + 50000 where name = '"+name+"'");
                System.out.println("--------------User "+name+" billed--------------");
            }
            System.out.println("--------User billed end----------");
            System.out.println();
            System.out.println("--------Provider billed start----------");
            rset = stmt.executeQuery("select * from item");
            while(rset.next()){
                String name = rset.getString("provider_name");
                String local_storage = Integer.toString(rset.getInt("local_storage"));
                stmt = conn.createStatement();
                stmt.executeUpdate("update provider set due_you = due_you + '"+local_storage+"' where name = '"+name+"'");
                System.out.println("--------------Provider "+name+" billed--------------");
            }
            System.out.println("--------Provider billed end----------");
        }
        catch (SQLException sqle) {         
            System.out.println("SQLException : " + sqle);        
        }
        catch (Exception e) {         
            System.out.println("Exception : " + e);        
        }
    }
    private static void manage_items(){
        try {
            Class.forName ("com.mysql.jdbc.Driver"); 
            Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2016312160", "2016312160", "2wnstjd!");
            Statement stmt = conn.createStatement();
            ResultSet rset = stmt.executeQuery("select * from download");
            int max = 0;
            String maxName = null;
            while(rset.next()){
                String name = rset.getString("item_name");
                stmt = conn.createStatement();
                ResultSet rset2 = stmt.executeQuery("select * from item where name = '"+name+"'");
                rset2.next();
                int num = rset2.getInt("address_retrieved");
                if(num > max){
                    max = num;
                    maxName = name;
                }
            }
            System.out.println("Most accessed item = "+maxName+", number = "+max);
            rset = stmt.executeQuery("select * from item");
            int low = 10000;
            String lowName = null;
            while(rset.next()){
                String name = rset.getString("name");
                int num = rset.getInt("address_retrieved");
                if(num < low){
                    low = num;
                    lowName = name;
                }
            }
            System.out.println("Least accessed item = "+lowName+", number = "+low);
            System.out.println();
            System.out.println("Drop items and providers did not pass threshold");
            System.out.println("----------start-----------");
            rset = stmt.executeQuery("select * from provider");
            while(rset.next()){
                int data_joined = rset.getInt("data_joined");
                String name = rset.getString("name");
                if(data_joined == 0){
                    stmt = conn.createStatement();
                    ResultSet rset3 = stmt.executeQuery("select * from item where provider_name = '"+name+"'");
                    System.out.println("Items that to be dropped:");
                    while(rset3.next()){
                        System.out.printf("%10s|%10s|%15s|%15s|%15d|\n", 
                           rset3.getString("id"),                                 
                           rset3.getString("name"),                                 
                           rset3.getString("provider_name"),
                           rset3.getString("type"),
                           rset3.getInt("size")
                    );
                    }
                    stmt = conn.createStatement();
                    stmt.executeUpdate("delete from provider where data_joined = 0");
                    System.out.println("---------Provider "+name+" deleted---------");
                }
            }
            System.out.println("----------end--------------");
            System.out.println();
            rset =stmt.executeQuery("select * from user");
            max = 0;
            maxName = null;
            while(rset.next()){
                String name = rset.getString("name");
                int num = rset.getInt("data_joined");
                if(num > max){
                    max = num;
                    maxName = name;
                }
            }
            System.out.println("Main user = "+maxName+", download times = "+max);
            System.out.println("These are the items using");
            rset = stmt.executeQuery("select * from download where user_name = '"+maxName+"'");
            while(rset.next()){
                System.out.printf("%10s|%10s|%15s|%15s|%15s|%15d|\n", 
                           rset.getString("user_name"),                                 
                           rset.getString("item_id"),                                 
                           rset.getString("provider_name"),
                           rset.getString("item_name"),
                           rset.getString("item_type"),
                           rset.getInt("item_size")
                    );
                System.out.println();
            }
        }
        catch (SQLException sqle) {         
            System.out.println("SQLException : " + sqle);        
        }
        catch (Exception e) {         
            System.out.println("Exception : " + e);        
        }
    }
}