#pragma once
#include "Video.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <set>


using namespace std;

class Catalog {
private:
    vector<Video*> videos;

    // Comparison for float ratings
    static bool withinRange(double a, double b, double epsilon = 0.1) {
        return abs(a - b) <= epsilon;
    }

    // Converts string to lower case
    static void toLower(string& s) {
        for (char& c : s ) c = tolower(c);
    }

    // Extract only movies from the catalog
    vector<Movie*> getMovies() const {
        vector<Movie*> res;
        for (Video* video : videos) {
            Movie* m = dynamic_cast<Movie*>(video);
            if (m) {
                res.push_back(m);
            }
        }
        return res;
    }

    // Extract only series from the catalog
    vector<Serie*> getSeries() const {
        vector<Serie*> res;
        for (Video* video : videos) {
            Serie* s = dynamic_cast<Serie*>(video);
            if (s) {
                res.push_back(s);
            }
        }
        return res;
    }
public:

    // prints the movies that match the provided rate. If not found, prints a message
    void showMoviesByRating(double rate) const {
        bool found = false;

        for (Movie* m : getMovies()) {
            if (withinRange(m->getAverageRating(), rate)){
                found = true;
                cout << *m;
            }
        }
        if (!found) {
            cout << "No movie(s) found with rating: " << rate << "\n\n";
        }
    }

    // prints the episodes of the given serie's title with the provided rate. If not found, prints a message
    void showSerieEpisodesByRating(string title, double rate) const {
        bool episode_found = false;
        bool serie_found = false;

        for (Serie* s : getSeries()) {
            if (s->getTitle() == title) {
                serie_found = true;
                for (Episode* e : s->getEpisodes()) {
                    if (withinRange(e->getAverageRating(), rate)) {
                        episode_found = true;
                        cout << *e;
                    }
                }
            }
        }
        if (!serie_found) {
            cout << "Warning. No serie found with title: " << title << "\n\n";
        } else if (!episode_found) {
            cout << "Warning. No episode(s) found with rating " << rate << " for " << title << "\n\n"; 
        }
    }

    // prints the info of any video found with the provided rate. If not found, prints a message
    void showByRating(double rate) const {
        bool found = false;
        for (Movie* m : getMovies()) {
            if (withinRange(m->getAverageRating(), rate)) {
                found = true;
                cout << *m;
            }
        }
        for (Serie* s : getSeries()){
            for (Episode* e : s->getEpisodes()) {
                if (withinRange(e->getAverageRating(), rate)) {
                    found = true;
                    cout << *e;
                }
            }
        }
        if (!found) {
            cout << "Warning. No video(s) found with rating: " << rate << "\n\n";
        }
    }

    // prints the info of any video found with the provided genre. If not found, prints a message
    void showByGenre(string genre) {
        toLower(genre);
        bool found = false;
        for (Video* video : videos) {

            string video_genre = video->getGenre();
            toLower(video_genre);

            if (video_genre == genre) {
                found = true;
                cout << *video;
            }

        }
        if (!found) {
            cout << "Warning. No video(s) found with genre: " << genre << "\n\n";
        }
    }

    // prints the info of any video found with the provided title. If not found, prints a message
    void showByTitle(const string &title) const {
        
        bool found = false;
        for (Video* v : videos) {
            // Check the video itself (movie or series header)
            if (v->getTitle() == title) {
                found = true;
                cout << *v;
            }
            // Also search inside series episodes
            Serie* s = dynamic_cast<Serie*>(v);
            if (s) {
                for (Episode* e : s->getEpisodes()) {
                    if (e->getTitle() == title) {
                        found = true;
                        cout << *e;
                    }
                }
            }
        }
        if (!found) {
            cout << "Warning. No video found titled: " << title << "\n\n";
        }
    }

    // shows the data of all the movies
    void showMovies() {
        for (Movie* m : getMovies()) cout << *m;
    }

    // shows the data of all the series
    void showSeries() {
        for (Serie* s : getSeries()) cout << *s;
    }

    // validates rate, finds title matches and adds the rating to the video's respective rating vector
    void rateVideo(string &title, double rate){
        
        set<double> validRatings = {0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0};
        if (!(validRatings.count(rate))) {
            cout << "Invalid rate. Must be a multiple of 0.5 between 0.5 and 5.0\n\n";
            return;     
        }

        bool found = false;
        for (Video* video : videos) {

            Serie* s = dynamic_cast<Serie*>(video);
            if (s) {
                // check series title match
                if (s->getTitle() == title) {
                    found = true;
                    cout << "Warning. Can't rate a serie! Try rating the serie's episodes\n\n";
                }

                // check episodes title match
                for (Episode* e : s->getEpisodes()) {
                    if (e->getTitle() == title) {
                        found = true;
                        e->addRating(rate);
                        cout << "Episode " << title << " rated succesfully\n";
                    }
                }
            }
            // check movies title match
            else if (video->getTitle() == title) {
                found = true;
                video->addRating(rate);
                cout << "Movie " << title << " rated succesfully\n";
            }
        } 
        if (!found) {
            cout << "Warning. No video(s) found with rating: " 
                << fixed << setprecision(1) << rate << defaultfloat << "\n\n";
        }
    }

    // loads data base into the respective constructors
    void loadData(string filename) {
        
        ifstream file(filename);
        if (!file) {
            cout << "Error opening file\n";
        }
        string line;
        Serie* lastSerie = nullptr;
        
        while(getline(file,line)) {
            stringstream ss(line);
            string type;
            getline(ss,type,',');

            if (type=="M") {
                string strid, title, strlength, genre;
                int id, length;

                getline(ss, strid, ',');
                getline(ss, title, ',');
                getline(ss, strlength, ',');
                getline(ss, genre, ',');

                id = stoi(strid);
                length = stoi(strlength);
                
                Movie* movie = new Movie(id, title, genre, length);
                videos.push_back(movie);

            } else if (type == "S") {

                string strid, title, genre;
                int id;

                getline(ss, strid, ',');
                getline(ss, title, ',');
                getline(ss, genre, ',');

                id = stoi(strid);

                Serie* serie = new Serie(id, title, genre);
                videos.push_back(serie);
                lastSerie = serie;

                
            } else if (type == "E") {

                if (lastSerie == nullptr) {
                    continue;
                }

                string title, strlength, strseason;
                int length, season;

                getline(ss, title, ',');
                getline(ss, strlength, ',');
                getline(ss, strseason, ',');

                length = stoi(strlength);
                season = stoi(strseason);

                Episode* episode = new Episode(length, title, season);
                lastSerie->addEpisode(episode);
            }

        
        }
    }
};