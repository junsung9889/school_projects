package edu.skku.map.pa1;

import android.icu.text.Transliterator;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentActivity;
import androidx.viewpager2.adapter.FragmentStateAdapter;

import static androidx.viewpager.widget.PagerAdapter.POSITION_NONE;

public class myFragementAdapter extends FragmentStateAdapter {
    private String username;
    public myFragementAdapter(@NonNull FragmentActivity fragmentActivity){
        super(fragmentActivity);
    }

    public myFragementAdapter(@NonNull FragmentActivity fragmentActivity,String username) {
        super(fragmentActivity);
        this.username = username;
    }


    @NonNull
    @Override
    public Fragment createFragment(int position) {
        switch (position){
            case 0:
                Fragment personal = new personal();
                Bundle bundle = new Bundle(1);
                bundle.putString("UserName",username);
                personal.setArguments(bundle);
                return personal;
            case 1:
                return new public_();
        }
        return null;
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public int getItemCount() {
        return 2;
    }
}
