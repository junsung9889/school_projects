package edu.skku.map.pa1;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.view.GravityCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.appcompat.widget.Toolbar;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.MediaStore;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;


import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.material.navigation.NavigationView;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.StorageReference;
import com.google.firebase.storage.UploadTask;

import java.io.ByteArrayOutputStream;
import java.util.HashMap;
import java.util.Map;

public class NewPost extends AppCompatActivity implements NavigationView.OnNavigationItemSelectedListener {
    private DatabaseReference mPostReference;
    private static final int PICK_IMAGE = 777;
    private static final int POST_IMAGE = 888;
    private StorageReference mStorageRef;
    Uri currentImageUri;
    DrawerLayout drawerLayout;
    EditText contentET,tagET;
    String content = "",tag = "";
    ImageButton imageButton;
    CheckBox checkBox;
    Boolean check;
    MenuItem fullname,birthday,email;
    ValueEventListener postListener;
    ImageView profile;
    TextView username;
    String name = "";
    boolean select;

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        select =false;
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_new_post);
        contentET = (EditText)findViewById(R.id.contents);
        tagET = (EditText)findViewById(R.id.tags);
        imageButton = (ImageButton)findViewById(R.id.imageButton);
        checkBox = (CheckBox)findViewById(R.id.checkBox);
        mStorageRef = FirebaseStorage.getInstance().getReference("Images");
        mPostReference = FirebaseDatabase.getInstance().getReference();
        currentImageUri = null;

        Intent intent = getIntent();
        name = intent.getStringExtra("UserName");

        imageButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent gallery = new Intent(Intent.ACTION_PICK, MediaStore.Images.Media.INTERNAL_CONTENT_URI);
                startActivityForResult(gallery,POST_IMAGE);
            }
        });

        Toolbar toolbar =(Toolbar)findViewById(R.id.main_toolbar2);
        setSupportActionBar(toolbar);
        getSupportActionBar().setDisplayShowTitleEnabled(false);

        drawerLayout = (DrawerLayout)findViewById(R.id.drawerlayout);
        NavigationView navigationView = (NavigationView) findViewById(R.id.drawer);

        View headerView = navigationView.getHeaderView(0);
        profile = (ImageView)headerView.findViewById(R.id.profile);
        username = (TextView)headerView.findViewById(R.id.Username);
        username.setText(name);

        Menu menu = navigationView.getMenu();
        fullname = menu.findItem(R.id.item1);
        birthday = menu.findItem(R.id.item2);
        email = menu.findItem(R.id.item3);

        postListener = new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                for(DataSnapshot postSnapshot : dataSnapshot.getChildren()) {
                    String key = postSnapshot.getKey();
                    FirebasePost info = postSnapshot.getValue(FirebasePost.class);
                    if(key.equals(name)){
                        fullname.setTitle(info.fullname);
                        birthday.setTitle(info.birthday);
                        email.setTitle(info.email);
                    }
                }
            }
            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        };
        mPostReference.child("User_list").addValueEventListener(postListener);

        navigationView.setNavigationItemSelectedListener(this);

        ActionBarDrawerToggle drawerToggle = new ActionBarDrawerToggle(this,drawerLayout,toolbar,R.string.app_name,R.string.app_name);
        drawerToggle.syncState();

        Button button = (Button)findViewById(R.id.button7);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                content = contentET.getText().toString();
                tag = tagET.getText().toString();
                check = checkBox.isChecked();
                if(currentImageUri !=null) {
                    StorageReference ref = mStorageRef.child(name + content);
                    UploadTask uploadTask = ref.putFile(currentImageUri);
                    uploadTask.addOnFailureListener(new OnFailureListener() {
                        @Override
                        public void onFailure(@NonNull Exception e) {
                        }
                    });
                    uploadTask.addOnSuccessListener(new OnSuccessListener<UploadTask.TaskSnapshot>() {
                        @Override
                        public void onSuccess(UploadTask.TaskSnapshot taskSnapshot) {

                        }
                    });
                }
                if (content.length() == 0) {
                    Toast.makeText(NewPost.this, "Please input contents", Toast.LENGTH_SHORT).show();
                }
                else {
                    postListViewItem(true);
                    Intent intent = new Intent(NewPost.this, MainPage.class);
                    intent.putExtra("UserName",name);
                    startActivity(intent);
                }
            }
        });

        profile.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent gallery = new Intent(Intent.ACTION_PICK, MediaStore.Images.Media.INTERNAL_CONTENT_URI);
                startActivityForResult(gallery,PICK_IMAGE);
            }
        });
        final long ONE_MEGABYTE = 1024*1024*16;
        StorageReference ref = mStorageRef.child(name);
        ref.getBytes(ONE_MEGABYTE).addOnFailureListener(new OnFailureListener() {
            @Override
            public void onFailure(@NonNull Exception e) {

            }
        }).addOnSuccessListener(new OnSuccessListener<byte[]>() {
            @Override
            public void onSuccess(byte[] bytes) {
                Bitmap bitmap = BitmapFactory.decodeByteArray(bytes,0,bytes.length);
                ImageView img = (ImageView)headerView.findViewById(R.id.profile);
                img.setImageBitmap(bitmap);
            }
        });

    }
    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if(data!=null && data.getData() != null && requestCode == PICK_IMAGE){
            NavigationView navigationView = (NavigationView) findViewById(R.id.drawer);
            View headerView = navigationView.getHeaderView(0);
            ImageView img = (ImageView)headerView.findViewById(R.id.profile);
            img.setImageURI(data.getData());
            StorageReference ref = mStorageRef.child(name);
            UploadTask uploadTask = ref.putFile(data.getData());
            uploadTask.addOnFailureListener(new OnFailureListener() {
                @Override
                public void onFailure(@NonNull Exception e) {
                }
            });
            uploadTask.addOnSuccessListener(new OnSuccessListener<UploadTask.TaskSnapshot>() {
                @Override
                public void onSuccess(UploadTask.TaskSnapshot taskSnapshot) {

                }
            });
        }
        if(data!=null && data.getData() != null && requestCode == POST_IMAGE){
            select = true;
            imageButton.setImageURI(data.getData());
            currentImageUri = data.getData();
        }
    }

    @Override
    public boolean onNavigationItemSelected(@NonNull MenuItem item) {
        switch (item.getItemId()){
            case R.id.item1:
                break;
            case R.id.item2:
                break;
            case R.id.item3:
                break;
        }
        drawerLayout.closeDrawer(GravityCompat.START);
        return false;
    }

    public void postListViewItem(boolean add){
        Map<String,Object> childUpdates = new HashMap<>();
        Map<String,Object> postValues = null;
        if(add){
            ListViewItem post = new ListViewItem(name,content,tag,check,select);
            postValues = post.toMap();
        }
        childUpdates.put("/User_post/"+name+content,postValues);
        mPostReference.updateChildren(childUpdates);
    }
}
