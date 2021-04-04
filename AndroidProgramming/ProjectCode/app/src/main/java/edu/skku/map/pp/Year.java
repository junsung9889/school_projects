package edu.skku.map.pp;

import android.content.Context;
import android.os.Build;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Adapter;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.Toast;

import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;


/**
 * A simple {@link Fragment} subclass.
 */
public class Year extends Fragment {
    private DatabaseReference mPostReference;
    private ValueEventListener postListener;
    private ListViewAdapter adapter;
    public Year() {

    }

    @RequiresApi(api = Build.VERSION_CODES.N)
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        super.onCreateView(inflater,container,savedInstanceState);
        View view = inflater.inflate(R.layout.fragment_year, container, false);
        String name = getArguments().getString("UserName");
        ListView listView = (ListView)view.findViewById(R.id.listView2);

        adapter = new ListViewAdapter();
        mPostReference = FirebaseDatabase.getInstance().getReference();
        postListener = new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                adapter = new ListViewAdapter();
                for (DataSnapshot postSnapshot : dataSnapshot.getChildren()) {
                    String key = postSnapshot.getKey();
                    ListViewItem info = postSnapshot.getValue(ListViewItem.class);
                    if(key.equals(name+info.getTitle())){
                        String title = info.getTitle();
                        String year = info.getYear();
                        String imdbRating = postSnapshot.child("IMDBRates").getValue().toString();
                        String director = info.getDirector();
                        String genre = info.getGenre();
                        String metaScore = postSnapshot.child("MetaScore").getValue().toString();
                        String released = postSnapshot.child("ReleasedDate").getValue().toString();
                        String runtime = info.getRuntime();
                        adapter.addItem(title,year,imdbRating,director,genre,metaScore,released,runtime);
                        adapter.sortByYear();
                        adapter.notifyDataSetChanged();
                    }
                }
                listView.setAdapter(null);
                listView.setAdapter(adapter);
            }
            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        };
        mPostReference.child("User_favorite").addValueEventListener(postListener);

        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                ListViewItem listViewItem = (ListViewItem)listView.getAdapter().getItem(position);
                String title = listViewItem.getTitle();
                String year = listViewItem.getYear();
                String released = listViewItem.getReleased();
                String runtime = listViewItem.getRuntime();
                String genre = listViewItem.getGenre();
                String imdbRating = listViewItem.getImdbRating();
                String metaScore = listViewItem.getMetascore();
                String director = listViewItem.getDirector();
                Toast.makeText(getActivity(),"Title : "+title + "\nYear : "+year + "\nReleased Date : "+released +
                        "\nRuntime : " +runtime+ "\nGenre : " +genre+ "\nIMDB Rates : " +imdbRating+
                        "\nMetascore : "+metaScore + "\nDirector : "+director,Toast.LENGTH_LONG).show();
            }
        });

        listView.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
            @Override
            public boolean onItemLongClick(AdapterView<?> parent, View view, int position, long id) {
                ListViewItem listViewItem = (ListViewItem)listView.getAdapter().getItem(position);
                String title = listViewItem.getTitle();
                mPostReference.child("User_favorite").child(name+title).removeValue().addOnSuccessListener(new OnSuccessListener<Void>() {
                    @Override
                    public void onSuccess(Void aVoid) {
                        Toast.makeText(getActivity(),"Remove Success!",Toast.LENGTH_LONG).show();
                    }
                }).addOnFailureListener(new OnFailureListener() {
                    @Override
                    public void onFailure(@NonNull Exception e) {

                    }
                });

                return false;
            }
        });
        return view;
    }
}
