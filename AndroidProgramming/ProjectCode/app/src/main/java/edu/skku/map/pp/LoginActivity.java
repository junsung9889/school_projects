package edu.skku.map.pp;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;

public class LoginActivity extends AppCompatActivity {
    private DatabaseReference mPostReference;
    EditText usernameET, passwordET;
    String username = "", password = "";
    ArrayList<String> data;
    ArrayList<String> pw;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        mPostReference = FirebaseDatabase.getInstance().getReference();
        usernameET = (EditText) findViewById(R.id.username);
        passwordET = (EditText) findViewById(R.id.password);
        data = new ArrayList<String>();
        pw = new ArrayList<String>();


        TextView textView = (TextView) findViewById(R.id.textView6);
        textView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(LoginActivity.this, SignUpActivity.class);
                startActivity(intent);
            }
        });
        getFirebaseDatabase();
        Button button = (Button) findViewById(R.id.button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                username = usernameET.getText().toString();
                password = passwordET.getText().toString();
                getFirebaseDatabase();
                int count = 0;

                if (!data.contains(username)) {
                    Toast.makeText(LoginActivity.this, "Wrong Username", Toast.LENGTH_SHORT).show();
                }
                else {
                    for(String user:data){
                        if(user.equals(username))
                            break;
                        count++;
                    }
                    if (!pw.get(count).equals(password)) {
                        Toast.makeText(LoginActivity.this, "Wrong Password", Toast.LENGTH_SHORT).show();
                    }
                    else {
                        Intent intent = new Intent(LoginActivity.this, MovieSearch.class);
                        intent.putExtra("UserName", username);
                        startActivity(intent);
                    }
                }
            }
        });
        Intent intent1 = getIntent();
        String UserName = intent1.getStringExtra("UserName");
        usernameET.setText(UserName);
    }

    public void getFirebaseDatabase() {
        final ValueEventListener postListener = new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                data.clear();
                pw.clear();
                for (DataSnapshot postSnapshot : dataSnapshot.getChildren()) {
                    FirebasePost info = postSnapshot.getValue(FirebasePost.class);
                    assert info != null;
                    data.add(info.username);
                    pw.add(info.password);
                }
            }
            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        };
        mPostReference.child("User_list").addValueEventListener(postListener);
    }
}
