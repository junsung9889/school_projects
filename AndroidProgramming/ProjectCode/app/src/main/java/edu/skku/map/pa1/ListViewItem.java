package edu.skku.map.pa1;

import android.net.Uri;
import android.widget.ImageView;

import java.util.HashMap;
import java.util.Map;

public class ListViewItem {
    private String username;
    private String contents;
    private String tags;
    private boolean check;
    private boolean select;

    public ListViewItem(){

    }

    public ListViewItem(String username, String contents, String tags,boolean check,boolean select) {
        this.check = check;
        this.username = username;
        this.contents = contents;
        this.tags = tags;
        this.select = select;
    }

    public Map<String, Object> toMap(){
        HashMap<String,Object> result = new HashMap<>();
        result.put("username",username);
        result.put("contents",contents);
        result.put("tags",tags);
        result.put("check",check);
        result.put("select",select);
        return result;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getContents() {
        return contents;
    }

    public void setContents(String contents) {
        this.contents = contents;
    }

    public String getTags() {
        return tags;
    }

    public void setTags(String tags) {
        this.tags = tags;
    }

    public boolean isCheck() {
        return check;
    }

    public void setCheck(boolean check) {
        this.check = check;
    }
    public boolean isSelect() {
        return select;
    }

    public void setSelect(boolean check) {
        this.select = select;
    }

}
