package edu.skku.map.pa1;

import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;


/**
 * A simple {@link Fragment} subclass.
 */
public class public_ extends Fragment {
    private DatabaseReference mPostReference;
    private ValueEventListener postListener;
    boolean select;
    public public_() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_public_, container, false);
        ListView listView = (ListView)view.findViewById(R.id.listView);


        ListViewAdapter adapter = new ListViewAdapter();

        mPostReference = FirebaseDatabase.getInstance().getReference();
        postListener = new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                for (DataSnapshot postSnapshot : dataSnapshot.getChildren()) {
                    ListViewItem info = postSnapshot.getValue(ListViewItem.class);
                    select = info.isSelect();
                    if(info.isCheck()) {
                        adapter.addItem(info.getUsername(), info.getContents(), info.getTags());
                        adapter.notifyDataSetChanged();
                    }
                }
            }
            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        };
        mPostReference.child("User_post").addValueEventListener(postListener);

        listView.setAdapter(adapter);
        return view;
    }
}
