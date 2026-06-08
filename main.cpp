#include "Video.h"
#include "Catalog.h"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;

// Input handeers
    
// Reads a trimmed line
string readText(const string& prompt) {
    string input;
    cout << prompt;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, input);
    return input;
}
 
// Reads an int. Re-prompts on bad input
int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            return value;
        }
        cout << "Please enter a valid integer.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
 
// Reads a double. Re-prompts on bad input
double readDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            return value;
        }
        cout << "Please enter a valid number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    Catalog catalog;
 
    int option;
    do {
        cout << "----------------------------------------\n"
             << "(1) Load data file\n"
             << "(2) Show videos with a specific rating or genre\n"
             << "(3) Show episodes of a series with a specific rating\n"
             << "(4) Show movies with a specific rating\n"
             << "(5) Rate a video\n"
             << "(6) Exit\n"
             << "----------------------------------------\n";
        option = readInt("Option: ");
 
        if (option == 1) {
            string filename;
            cout << "Enter filename: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, filename);
            catalog.loadData(filename);
 
        } else if (option == 2) {
            int sub;
            do {
                cout << "\n  (1) Search by rating\n"
                     << "  (2) Search by genre\n"
                     << "  (3) Back\n";
                sub = readInt("  Option: ");

                if (sub == 1) {
                    double rate = readDouble("  Enter rating (0.1 – 5.0): ");
                    catalog.showByRating(rate);

                } else if (sub == 2) {
                    string genre = readText("  Enter genre: ");
                    catalog.showByGenre(genre);

                } else if (sub == 3) {
                    cout << "  Returning to main menu.\n";

                } else {
                    cout << "  Invalid option.\n";
                }
            } while (sub != 3);
 
        } else if (option == 3) {
            string title = readText("Enter series title: ");
            double rate  = readDouble("Enter rating (0.1 – 5.0): ");
            catalog.showSerieEpisodesByRating(title, rate);
 
        } else if (option == 4) {
            double rate = readDouble("Enter rating (0.1 – 5.0): ");
            catalog.showMoviesByRating(rate);
 
        } else if (option == 5) {
            string title = readText("Enter title to rate: ");
            double rate  = readDouble("Enter rating (0.5 – 5.0): ");
            catalog.rateVideo(title, rate);
 
        } else if (option == 6) {
            cout << "Exiting. Goodbye!\n";
 
        } else {
            cout << "Invalid option. Please choose 1 – 6.\n";
        }
 
    } while (option != 6);
 
    return 0;
}