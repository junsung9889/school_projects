package edu.skku.map.pa1;

import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;


/**
 * A simple {@link Fragment} subclass.
 */
public class personal extends Fragment {
    private DatabaseReference mPostReference;
    private ValueEventListener postListener;
    boolean select;
    public personal() {

        // Required empty public constructor
    }



    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        super.onCreateView(inflater,container,savedInstanceState);
        View view = inflater.inflate(R.layout.fragment_personal, container, false);
        String username = getArguments().getString("UserName");
        ListView listView = (ListView)view.findViewById(R.id.listView2);

        ListViewAdapter adapter = new ListViewAdapter();

        mPostReference = FirebaseDatabase.getInstance().getReference();
        postListener = new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                for (DataSnapshot postSnapshot : dataSnapshot.getChildren()) {
                    ListViewItem info = postSnapshot.getValue(ListViewItem.class);
                    if(info.getUsername().equals(username) && !info.isCheck()) {
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
