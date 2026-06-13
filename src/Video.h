#pragma once
#include <string>
#include <vector>
#include <iostream> 
#include <iomanip>
using namespace std;

class Video {
protected:
    int id;
    vector<double> ratings;
    string title;
    string genre;
public:
    Video(int id, string title, string genre) : id{id}, title{title}, genre{genre} {}
    virtual double getAverageRating() const = 0;
    virtual void showData() const = 0;
    void addRating(double rating) {
        ratings.push_back(rating);
    }
    string getTitle() const {
        return title;
    }

    string getGenre() const {
        return genre;
    }
    friend ostream& operator<<(ostream& os, const Video& video) {
        video.showData();
        return os;
    }

    virtual ~Video() = default;

};

class Movie:public Video {
private:
    int length;
public:
    Movie(int id, string title, string genre, int length) : Video(id, title, genre), length{length} {}

    double getAverageRating() const override {
        // returns the average rating of a Movie. Used in showData()
        if (ratings.empty()) {
            return 0;
        } 

        double count = 0;
        for (double rating : ratings) {
            count += rating;
        }
        return count / double(ratings.size());
    }

    void showData() const override{
        cout 
        << "----------------------Movie------------------------\n" 
        << id << ". " << title  
        << "\nGenre: " << genre << "\nRuntime: " << length << " mins \n"
        << "Rating: " << fixed << setprecision(1) << getAverageRating() << defaultfloat << "\n"
        << "----------------------Movie------------------------\n\n"; 
    }
};

class Episode {
private:
    int length;
    string title;
    int season;
    vector<double> ratings;
public:

    Episode(int length, string title, int season) : length{length}, title{title}, season{season} {}

    double getAverageRating() const {
        // returns the average rating of an Episode. Used in showData()
        if (ratings.empty()) {
            return 0;
        } 

        double count = 0;
        for (double rating : ratings) {
            count += rating;
        }
        return count / double(ratings.size());
    }

    void showEpisode() const {
        cout 
        << "S" << season << " - " << title << " - " << length << " mins - " 
        << "Rating: " << fixed << setprecision(1) << getAverageRating() << defaultfloat << "\n";
    }

    int getSeason() const {
        return season;
    }

    string getTitle() const {
        return title;
    }

    void addRating(double rating) {
        ratings.push_back(rating);
    }

    friend ostream& operator<<(ostream& os, const Episode& episode) {
        episode.showEpisode();
        return os;
    }
};

class Serie:public Video {
private:
    vector<Episode*> episodes;
public:
    Serie(int id, string title, string genre) : Video(id, title, genre) {}

    void addEpisode(Episode* episode) {
        episodes.push_back(episode);
    }

    double getAverageRating() const override {
        // returns the average rating of a Serie by getting the average rating of each Episode. Used in showData()
        if (episodes.empty()) {
            return 0;
        } 
        
        int n = 0;
        double count = 0;
        for (Episode* episode : episodes) {
            double episode_rating = episode->getAverageRating();
            if (episode_rating != 0) {
                count += episode_rating;
                n++;
            }
        }
        if (n == 0) {
            return 0;
        }
        return count / n;
    }

    void showData() const override {
        cout 
        << "----------------------Serie------------------------\n" 
        << id << ". " << title  
        << "\n   Serie - " << genre << "- Rating: " << fixed << setprecision(1) << getAverageRating() << defaultfloat << "\n";

        int episode_count = 1;
        int season_count = 1;
        for (Episode* episode: episodes) {

            if (episode->getSeason() != season_count) {
                season_count++;
                episode_count = 1;
            }

            cout << "\nE" << episode_count;
            episode->showEpisode();
            episode_count++;

        }

        cout << "----------------------Serie------------------------\n\n";
    }

    vector<Episode*>& getEpisodes() {
        return episodes;
    }
};