package edu.skku.map.pp;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.HttpUrl;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class MovieSearch extends AppCompatActivity {
    private DatabaseReference mPostReference;
    Button button1,button2,button3;
    EditText editText;
    TextView textView1,textView2,textView3,textView4,textView5,textView6,textView7,textView8;
    String name = "";
    String title = "",year = "",released ="",runtime="",genre="",imdbRating="",metaScore="",director="";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_movie_search);

        editText = (EditText)findViewById(R.id.url);
        textView1 = (TextView)findViewById(R.id.response1);
        textView2 = (TextView)findViewById(R.id.response2);
        textView3 = (TextView)findViewById(R.id.response3);
        textView4 = (TextView)findViewById(R.id.response4);
        textView5 = (TextView)findViewById(R.id.response5);
        textView6 = (TextView)findViewById(R.id.response6);
        textView7 = (TextView)findViewById(R.id.response7);
        textView8 = (TextView)findViewById(R.id.response8);
        button1 = (Button)findViewById(R.id.sendingGet);
        button2 = (Button)findViewById(R.id.button2);
        button3 = (Button)findViewById(R.id.button3);


        button1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                OkHttpClient client = new OkHttpClient();

                HttpUrl.Builder urlBuilder = Objects.requireNonNull(HttpUrl.parse("http://www.omdbapi.com/?")).newBuilder();
                urlBuilder.addQueryParameter("t",editText.getText().toString());
                urlBuilder.addQueryParameter("apikey","d768db40");

                String url = urlBuilder.build().toString();
                Request req = new Request.Builder().url(url).build();

                client.newCall(req).enqueue(new Callback() {
                    @Override
                    public void onFailure(@NotNull Call call, @NotNull IOException e) {
                        e.printStackTrace();
                    }

                    @Override
                    public void onResponse(@NotNull Call call, @NotNull Response response) throws IOException {
                        final String myResponse =response.body().string();

                        Gson gson = new GsonBuilder().create();
                        final ListViewItem data = gson.fromJson(myResponse,ListViewItem.class);
                        title = data.getTitle();
                        year = data.getYear();
                        released = data.getReleased();
                        runtime = data.getRuntime();
                        genre = data.getGenre();
                        imdbRating = data.getImdbRating();
                        metaScore = data.getMetascore();
                        director = data.getDirector();

                        MovieSearch.this.runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                textView1.setText("Title : " + data.getTitle());
                                textView2.setText("Year : " + data.getYear());
                                textView3.setText("Released Date : " + data.getReleased());
                                textView4.setText("Runtime : " + data.getRuntime());
                                textView5.setText("Genre : " + data.getGenre());
                                textView6.setText("IMDB Rates : " + data.getImdbRating());
                                textView7.setText("Metascore : " + data.getMetascore());
                                textView8.setText("Director : "+ data.getDirector());
                            }
                        });
                    }
                });
            }
        });

        button2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(!textView1.getText().toString().equals("")) {
                    mPostReference = FirebaseDatabase.getInstance().getReference();
                    postFirebaseDatabase(true);
                    Toast.makeText(MovieSearch.this, "Add Success!", Toast.LENGTH_SHORT).show();
                }
            }
        });

        button3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MovieSearch.this,MainPage.class);
                intent.putExtra("UserName",name);
                startActivity(intent);
            }
        });

        if(savedInstanceState != null){
            textView1.setText(savedInstanceState.getString("title"));
            textView2.setText(savedInstanceState.getString("year"));
            textView3.setText(savedInstanceState.getString("released"));
            textView4.setText(savedInstanceState.getString("runtime"));
            textView5.setText(savedInstanceState.getString("genre"));
            textView6.setText(savedInstanceState.getString("imdbRating"));
            textView7.setText(savedInstanceState.getString("metaScore"));
            textView8.setText(savedInstanceState.getString("director"));
        }

        Intent intent = getIntent();
        name = intent.getStringExtra("UserName");
    }

    public void postFirebaseDatabase(boolean add){
        Map<String,Object> childUpdates = new HashMap<>();
        Map<String,Object> postValues = null;
        if(add){
            ListViewItem post = new ListViewItem(title,year,released,runtime,director,genre,imdbRating,metaScore);
            postValues = post.toMap();
        }
        childUpdates.put("/User_favorite/"+(name+title),postValues);
        mPostReference.updateChildren(childUpdates);
    }
    @Override
    protected void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);
        String title = textView1.getText().toString();
        String year = textView2.getText().toString();
        String released = textView3.getText().toString();
        String runtime = textView4.getText().toString();
        String genre = textView5.getText().toString();
        String imdbRating = textView6.getText().toString();
        String metaScore = textView7.getText().toString();
        String director = textView8.getText().toString();
        outState.putString("title",title);
        outState.putString("year",year);
        outState.putString("released",released);
        outState.putString("runtime",runtime);
        outState.putString("genre",genre);
        outState.putString("imdbRating",imdbRating);
        outState.putString("metaScore",metaScore);
        outState.putString("director",director);
    }
}
