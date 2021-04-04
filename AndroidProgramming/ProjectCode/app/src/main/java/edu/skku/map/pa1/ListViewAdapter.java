package edu.skku.map.pa1;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;

import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.StorageReference;

import java.util.ArrayList;

public class ListViewAdapter extends BaseAdapter {
    private ArrayList<ListViewItem> listViewItemList = new ArrayList<ListViewItem>();
    private StorageReference mStorageRef;
    boolean select;
    public ListViewAdapter(){

    }
    public ListViewAdapter(boolean select){
        this.select = select;
    }
    @Override
    public int getCount() {
        return listViewItemList.size();
    }

    @Override
    public Object getItem(int position) {
        return listViewItemList.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        final int pos = position;
        final Context context = parent.getContext();
        if (convertView == null) {
            LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            convertView = inflater.inflate(R.layout.listview_item, parent, false);
        }
        ImageView profile = (ImageView)convertView.findViewById(R.id.profile);
        ImageView postImage = (ImageView)convertView.findViewById(R.id.postImage);
        TextView username = (TextView) convertView.findViewById(R.id.username);
        TextView contents = (TextView) convertView.findViewById(R.id.contents);
        TextView tags = (TextView) convertView.findViewById(R.id.tags);

        ListViewItem listViewItem = listViewItemList.get(position);

        username.setText(listViewItem.getUsername());
        contents.setText(listViewItem.getContents());
        tags.setText(listViewItem.getTags());

        final long ONE_MEGABYTE = 1024*1024*16;
        mStorageRef = FirebaseStorage.getInstance().getReference("Images");
        StorageReference ref = mStorageRef.child(username.getText().toString());
        ref.getBytes(ONE_MEGABYTE).addOnFailureListener(new OnFailureListener() {
            @Override
            public void onFailure(@NonNull Exception e) {

            }
        }).addOnSuccessListener(new OnSuccessListener<byte[]>() {
            @Override
            public void onSuccess(byte[] bytes) {
                Bitmap bitmap = BitmapFactory.decodeByteArray(bytes,0,bytes.length);
                profile.setImageBitmap(bitmap);
            }
        });

        ref = mStorageRef.child(username.getText().toString() + contents.getText().toString());
        ref.getBytes(ONE_MEGABYTE).addOnFailureListener(new OnFailureListener() {
            @Override
            public void onFailure(@NonNull Exception e) {

            }
        }).addOnSuccessListener(new OnSuccessListener<byte[]>() {
            @Override
            public void onSuccess(byte[] bytes) {
                Bitmap bitmap = BitmapFactory.decodeByteArray(bytes, 0, bytes.length);
                postImage.setImageBitmap(bitmap);
            }
        });

        return convertView;
    }

    public void addItem(String username, String contents, String tags) {
        ListViewItem item = new ListViewItem();

        item.setUsername(username);
        item.setContents(contents);
        item.setTags(tags);

        listViewItemList.add(item);
    }

}
