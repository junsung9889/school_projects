package edu.skku.map.pa1;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.view.GravityCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.viewpager2.widget.ViewPager2;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.MediaStore;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.material.navigation.NavigationView;
import com.google.android.material.tabs.TabLayout;
import com.google.android.material.tabs.TabLayoutMediator;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.StorageReference;
import com.google.firebase.storage.UploadTask;

import java.util.ArrayList;


public class MainPage extends AppCompatActivity implements NavigationView.OnNavigationItemSelectedListener {
    private DatabaseReference mPostReference;
    private static final int PICK_IMAGE = 777;
    ImageView profile;
    TextView username;
    private StorageReference mStorageRef;
    boolean check;
    Uri currentImageUri;
    DrawerLayout drawerLayout;
    MenuItem fullname,birthday,email;
    ValueEventListener postListener;
    String name = "";
    ViewPager2 viewPager2;

    @SuppressLint("ClickableViewAccessibility")
    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_page);
        check = false;
        mPostReference = FirebaseDatabase.getInstance().getReference();

        Button button = (Button)findViewById(R.id.button6);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainPage.this, NewPost.class);
                intent.putExtra("UserName",name);
                startActivity(intent);
            }
        });
        Toolbar toolbar =(Toolbar)findViewById(R.id.main_toolbar2);
        setSupportActionBar(toolbar);
        getSupportActionBar().setDisplayShowTitleEnabled(false);

        Intent intent1 = getIntent();
        String UserName = intent1.getStringExtra("UserName");

        drawerLayout = (DrawerLayout)findViewById(R.id.drawerlayout);
        NavigationView navigationView = (NavigationView) findViewById(R.id.drawer);

        View headerView = navigationView.getHeaderView(0);
        mStorageRef = FirebaseStorage.getInstance().getReference("Images");
        profile = (ImageView)headerView.findViewById(R.id.profile);
        username = (TextView)headerView.findViewById(R.id.Username);
        username.setText(UserName);
        name = username.getText().toString();

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

        viewPager2 = findViewById(R.id.viewPager2);
        viewPager2.setAdapter(new myFragementAdapter(this,name));


        TabLayout tabLayout = findViewById(R.id.tabLayout);
        TabLayoutMediator tabLayoutMediator = new TabLayoutMediator(tabLayout, viewPager2, new TabLayoutMediator.TabConfigurationStrategy() {
            @Override
            public void onConfigureTab(@NonNull TabLayout.Tab tab, int position) {
                switch(position){
                    case 0:{
                        tab.setText("Personal Posts");
                        break;
                    }
                    case 1:{
                        tab.setText("Public Posts");
                        break;
                    }
                }
            }
        });
        tabLayoutMediator.attach();

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

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if(requestCode == PICK_IMAGE && data!= null &&data.getData() != null){
            NavigationView navigationView = (NavigationView) findViewById(R.id.drawer);
            View headerView = navigationView.getHeaderView(0);
            ImageView img = (ImageView)headerView.findViewById(R.id.profile);
            currentImageUri = data.getData();
            check = true;
            img.setImageURI(data.getData());

            StorageReference ref = mStorageRef.child(name);
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
    }

}
