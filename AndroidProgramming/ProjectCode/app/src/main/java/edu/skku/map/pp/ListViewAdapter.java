package edu.skku.map.pp;

import android.content.Context;
import android.os.Build;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import androidx.annotation.RequiresApi;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public class ListViewAdapter extends BaseAdapter {
    private ArrayList<ListViewItem> listViewItemList = new ArrayList<ListViewItem>();

    public ListViewAdapter(){

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

        TextView title = (TextView) convertView.findViewById(R.id.title);
        TextView year = (TextView) convertView.findViewById(R.id.year);
        TextView imdbRates = (TextView) convertView.findViewById(R.id.imdbRates);

        ListViewItem listViewItem = listViewItemList.get(position);

        title.setText(listViewItem.getTitle());
        year.setText(listViewItem.getYear());
        imdbRates.setText(listViewItem.getImdbRating());


        return convertView;
    }

    public void addItem(String title, String year, String ImdbRates,String director,String genre,String metascore,String released, String runtime) {
        ListViewItem item = new ListViewItem();
        item.setTitle(title);
        item.setYear(year);
        item.setImdbRating(ImdbRates);
        item.setDirector(director);
        item.setGenre(genre);
        item.setMetascore(metascore);
        item.setReleased(released);
        item.setRuntime(runtime);
        listViewItemList.add(item);
    }

    @RequiresApi(api = Build.VERSION_CODES.N)
    public void sortByYear(){
        Collections.sort(listViewItemList,new Comparator<ListViewItem>() {
            @Override
            public int compare(ListViewItem o1, ListViewItem o2) {
                if(Integer.parseInt(o1.getYear()) < Integer.parseInt(o2.getYear())){
                    return 1;
                }
                else if(Integer.parseInt(o1.getYear()) > Integer.parseInt(o2.getYear())){
                    return -1;
                }
                return 0;
            }
        });
    }

    @RequiresApi(api = Build.VERSION_CODES.N)
    public void sortByRating(){
        Collections.sort(listViewItemList,new Comparator<ListViewItem>() {
            @Override
            public int compare(ListViewItem o1, ListViewItem o2) {
                if(Float.parseFloat(o1.getImdbRating()) < Float.parseFloat(o2.getImdbRating())){
                    return 1;
                }
                else if(Float.parseFloat(o1.getImdbRating()) > Float.parseFloat(o2.getImdbRating())){
                    return -1;
                }
                return 0;
            }
        });
    }
}
