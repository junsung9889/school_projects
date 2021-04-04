package edu.skku.map.pa1;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class SignUpActivity extends AppCompatActivity {
    private DatabaseReference mPostReference;
    Button button;
    EditText usernameET,passwordET,fullnameET,birthdayET,emailET;
    String username = "",password = "",fullname = "",birthday = "",email = "";
    ArrayList<String> data;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sign_up);

        usernameET = (EditText)findViewById(R.id.username);
        passwordET = (EditText)findViewById(R.id.password);
        fullnameET = (EditText)findViewById(R.id.fullname);
        birthdayET = (EditText)findViewById(R.id.birthday);
        emailET = (EditText)findViewById(R.id.email);
        mPostReference = FirebaseDatabase.getInstance().getReference();
        data = new ArrayList<String>();
        button = (Button)findViewById(R.id.button2);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                boolean value = true;
                username = usernameET.getText().toString();
                password = passwordET.getText().toString();
                fullname = fullnameET.getText().toString();
                birthday = birthdayET.getText().toString();
                email = emailET.getText().toString();
                if(username.length()*password.length()*fullname.length()*birthday.length()*email.length() == 0) {
                    Toast.makeText(SignUpActivity.this, "Please fill all blanks", Toast.LENGTH_SHORT).show();
                    return;
                }
                for(String name:data){
                    if(name.equals(username)){
                        Toast.makeText(SignUpActivity.this, "Please use another username", Toast.LENGTH_SHORT).show();
                        return;
                    }
                }
                postFirebaseDatabase(true);
                Intent intent = new Intent(SignUpActivity.this, LoginActivity.class);
                intent.putExtra("UserName", username);
                startActivity(intent);
            }
        });
        getFirebaseDatabase();
    }

    public void getFirebaseDatabase(){
        final ValueEventListener postListener = new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                data.clear();
                for(DataSnapshot postSnapshot : dataSnapshot.getChildren()){
                    String key = postSnapshot.getKey();
                    data.add(key);
                }
            }
            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        };
        mPostReference.child("User_list").addValueEventListener(postListener);
    }

    public void postFirebaseDatabase(boolean add){
        Map<String,Object> childUpdates = new HashMap<>();
        Map<String,Object> postValues = null;
        if(add){
            FirebasePost post = new FirebasePost(username,password,fullname,birthday,email);
            postValues = post.toMap();
        }
        childUpdates.put("/User_list/"+username,postValues);
        mPostReference.updateChildren(childUpdates);
        clearET();
    }

    public void clearET(){
        usernameET.setText("");
        passwordET.setText("");
        fullnameET.setText("");
        birthdayET.setText("");
        emailET.setText("");
    }
}
