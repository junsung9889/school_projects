package edu.skku.map.pp;

import java.util.HashMap;
import java.util.Map;

public class FirebasePost {
    public String username;
    public String password;
    public String fullname;
    public String birthday;
    public String email;

    public FirebasePost(){

    }
    public FirebasePost(String username, String password, String fullname, String birthday, String email) {
        this.username = username;
        this.password = password;
        this.fullname = fullname;
        this.birthday = birthday;
        this.email = email;
    }

    public Map<String, Object> toMap(){
        HashMap<String,Object> result = new HashMap<>();
        result.put("username",username);
        result.put("password",password);
        result.put("fullname",fullname);
        result.put("birthday",birthday);
        result.put("email",email);
        return result;
    }
}
