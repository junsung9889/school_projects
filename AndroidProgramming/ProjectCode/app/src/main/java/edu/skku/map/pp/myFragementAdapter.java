package edu.skku.map.pp;

import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentActivity;
import androidx.viewpager2.adapter.FragmentStateAdapter;

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
                Fragment year = new Year();
                Bundle bundle = new Bundle(1);
                bundle.putString("UserName",username);
                year.setArguments(bundle);
                return year;
            case 1:
                Fragment rates = new Rates();
                Bundle bundle2 = new Bundle(1);
                bundle2.putString("UserName",username);
                rates.setArguments(bundle2);
                return rates;
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
