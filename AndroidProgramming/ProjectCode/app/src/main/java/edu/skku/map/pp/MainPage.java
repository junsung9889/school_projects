package edu.skku.map.pp;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.viewpager2.widget.ViewPager2;
import android.content.Intent;
import android.os.Bundle;
import com.google.android.material.tabs.TabLayout;
import com.google.android.material.tabs.TabLayoutMediator;




public class MainPage extends AppCompatActivity {
    String name = "";
    ViewPager2 viewPager2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_page);

        Intent intent = getIntent();
        name = intent.getStringExtra("UserName");


        viewPager2 = findViewById(R.id.viewPager2);
        viewPager2.setAdapter(new myFragementAdapter(this, name));


        TabLayout tabLayout = findViewById(R.id.tabLayout);
        TabLayoutMediator tabLayoutMediator = new TabLayoutMediator(tabLayout, viewPager2, new TabLayoutMediator.TabConfigurationStrategy() {
            @Override
            public void onConfigureTab(@NonNull TabLayout.Tab tab, int position) {
                switch (position) {
                    case 0: {
                        tab.setText("Order by Year");
                        break;
                    }
                    case 1: {
                        tab.setText("Order by Rates");
                        break;
                    }
                }
            }
        });
        tabLayoutMediator.attach();
    }
}
