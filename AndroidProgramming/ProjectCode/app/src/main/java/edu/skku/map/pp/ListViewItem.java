package edu.skku.map.pp;

import java.util.HashMap;
import java.util.Map;

public class ListViewItem {
    private String Title;
    private String Year;
    private String Released;
    private String Runtime;
    private String Director;
    private String Genre;
    private String imdbRating;
    private String Metascore;

    public ListViewItem() {

    }

    public ListViewItem(String title, String year, String released, String runtime, String director, String genre, String imdbRating, String metascore) {
        this.Title = title;
        Year = year;
        Released = released;
        Runtime = runtime;
        Director = director;
        Genre = genre;
        this.imdbRating = imdbRating;
        Metascore = metascore;
    }


    public Map<String, Object> toMap() {
        HashMap<String, Object> result = new HashMap<>();
        result.put("Title", Title);
        result.put("Year", Year);
        result.put("ReleasedDate", Released);
        result.put("Runtime",Runtime);
        result.put("Genre", Genre);
        result.put("IMDBRates", imdbRating);
        result.put("MetaScore", Metascore);
        result.put("Director",Director);
        return result;
    }


    public String getTitle() {
        return Title;
    }

    public void setTitle(String title) {
        Title = title;
    }

    public String getYear() {
        return Year;
    }

    public void setYear(String year) {
        Year = year;
    }

    public String getReleased() {
        return Released;
    }

    public void setReleased(String released) {
        Released = released;
    }

    public String getRuntime() {
        return Runtime;
    }

    public void setRuntime(String runtime) {
        Runtime = runtime;
    }

    public String getDirector() {
        return Director;
    }

    public void setDirector(String director) {
        Director = director;
    }

    public String getGenre() {
        return Genre;
    }

    public void setGenre(String genre) {
        Genre = genre;
    }

    public String getImdbRating() {
        return imdbRating;
    }

    public void setImdbRating(String imdbRating) {
        this.imdbRating = imdbRating;
    }

    public String getMetascore() {
        return Metascore;
    }

    public void setMetascore(String metascore) {
        Metascore = metascore;
    }
}
