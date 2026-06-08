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
        if (cin >> value) return value;
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
        if (cin >> value) return value;
        cout << "Please enter a valid number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    Catalog catalog;
    catalog.loadData("videos_dataset.txt");

    int option;
    do {
        cout << "\nType and option from the following list: \n"
             << "(1) Search for a Video \n"
             << "(2) Show all Movies \n" 
             << "(3) Show all Series \n"
             << "(4) Rate a Video \n"
             << "(5) Search for Videos with a specific rating or genre \n"
             << "(6) Show the episodes of a specific serie with a specific rating \n"
             << "(7) Show the movies with a specific rating \n"
             << "(8) Exit program \n";
        option = readInt("Option: ");
        
        if (option == 1) {            
            string title = readText("Type the title of the Video: ");
            catalog.showByTitle(title);
        } 
        else if (option == 2) {
            catalog.showMovies();
        } 
        else if (option == 3) {
            catalog.showSeries();
        } 
        else if (option == 4) {
            string title = readText("Type the title of the Video: ");
            double rate = readDouble("Type your rating (from 0.5 to 5): ");
            catalog.rateVideo(title, rate);
        } 
        else if (option == 5) {

            int subOption; 
            do {
                cout << "(1) Search by rating \n"
                     << "(2) Search by genre \n" 
                     << "(3) Cancel\n";  
                subOption = readInt("Option: ");

                if (subOption == 1) {
                    double rate = readDouble("Enter rating: ");
                    catalog.showByRating(rate);
                    break;  

                } else if (subOption == 2) {
                    string genre = readText("Enter genre: ");
                    catalog.showByGenre(genre);
                    break;

                } else if (subOption == 3) {
                    cout << "Search cancelled\n\n";
                } else {
                    cout << "Please select a correct option\n";
                }
            } while (subOption != 3);

        } else if (option == 6) {
            string title = readText("Enter serie title: ");
            double rate = readDouble("Enter rating: ");
            catalog.showSerieEpisodesByRating(title, rate);
        } else if (option == 7) {
            double rate = readDouble("Enter rating: ");
            catalog.showMoviesByRating(rate);
        }
        else if (option == 8) {
            cout << "Exiting program. \n";
        } 
        else {
            cout << "Wrong option\n\n";
        }

    } while (option != 8);

    return 0;

}