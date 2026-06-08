Mauricio Rey Hernández Curiel
Files: main.cpp, Catalog.h, Video.h, videos_dataset.txt

------------------------------------------------------------------------
1. DESCRIPTION
------------------------------------------------------------------------
This program is an object-oriented system that organizes Movies and Series
in a Catalog. It reads information from videos_dataset.txt and loads each
video into its respective class, storing attributes such as type, ID,
title, genre, and length (in minutes). The program offers a menu of
actions where the user can load the dataset, search for videos by a
specific rating or genre, show episodes of a series, show movies by
rating, and rate a video.

------------------------------------------------------------------------
2. COMPILATION INSTRUCTIONS
------------------------------------------------------------------------
Open the terminal and navigate to the directory containing all the project
files (main.cpp, Catalog.h, Video.h, videos_dataset.txt), and execute the
following command:

Using G++:
    g++ main.cpp -o main

------------------------------------------------------------------------
3. RUNNING THE PROGRAM
------------------------------------------------------------------------
Once successfully compiled, run the executable from your terminal:

.\main.exe

Main catalog menu:

When running the program, it will display a menu with the following options:

(1) Load data file: Enter the name of the dataset (in this case "videos_dataset.txt").
(2) Show videos with a specific rating or genre: Reprompts with sub-options:
    [1] Search by rating: provide a rating from 0.5 to 5.
    [2] Search by genre: provide a genre (case insensitive).
    [3] Back: return to the main menu.
(3) Show episodes of a series with a specific rating: provide a series
    title and a rating (from 0 to 5).
(4) Show movies with a specific rating: provide a rating (from 0 to 5).
(5) Rate a video: provide a Movie or Episode title and a rating from 0.5 to 5.
(6) Exit: closes the program.

Note: If no matching title is found, the rating is outside the 0.5–5.0
scale, or a non-numeric value is entered where a number is expected, the
user will receive an error message.

------------------------------------------------------------------------
Project requirements

------------------------------------------------------------------------
Classes, their relationships, and polymorphism
------------------------------------------------------------------------
Mermaid viewer: https://mermaidviewer.com/share/7GkpIvSkWiWzvh4k2pRoz

1. Catalog (`Catalog.h`):
    Has an aggregation relationship with the Video class and manages the
    printing and searching of videos using a vector of Video pointers. It
    also loads data from the dataset, creating objects on the heap. Private
    helper methods such as `vector<Movie*> getMovies()` and
    `vector<Serie*> getSeries()` are used to avoid code repetition.

2. Video (`Video.h`):
    An abstract class that holds shared video attributes such as ID,
    ratings, title, and genre.

3. Movie (`Video.h`):
    Inherits from Video and adds a unique attribute, length, which stores
    the runtime in minutes.

4. Serie (`Video.h`):
    Inherits from Video and contains a vector of Episode pointers, which
    it uses for printing and calculating the average rating.

5. Episode (`Video.h`):
    Has a composition relationship with Serie and contains methods for
    printing and calculating the episode's average rating.

------------------------------------------------------------------------
Operator overloading
------------------------------------------------------------------------

A friend function is defined in both Video and Episode to overload the
`<<` operator, improving code readability and allowing Catalog to print
video information with a simple expression such as `cout << *movie`.

    friend ostream& operator<<(ostream& os, const Video& video) {
        video.showData();
        return os;
    }

Since showData() is a void method defined in each class, it is called
directly rather than building and assigning a string to os.

------------------------------------------------------------------------

------------------------------------------------------------------------
New code learned
------------------------------------------------------------------------

1. friend function:
    A friend function is declared inside a class but is not a member of
    it. It has access to the class's private and protected attributes.
    It is commonly used for operator overloading when the left operand
    is not the class itself (in this case ostream).

        friend ostream& operator<<(ostream& os, const Video& video) {
            video.showData();
            return os;
        }

2. fixed << setprecision(1) << value << defaultfloat:
    Controls how float numbers are printed to the console.
    `fixed` forces decimal notation.
    `setprecision(1)` sets the number of decimal places to 1.
    `defaultfloat` resets the format back to default afterwards
    so the next output isn't affected. Requires <iomanip>.

        cout << "Rating: " << fixed << setprecision(1)
             << getAverageRating() << defaultfloat << "\n";

3. dynamic_cast<>():
    Attempts to safely cast a base class pointer to a derived class
    pointer at runtime. If the object is not of the target type, it
    returns nullptr instead of causing undefined behavior. Used here
    to distinguish Movies from Series inside the videos vector.

        Movie* m = dynamic_cast<Movie*>(video);
        if (m != nullptr) {
            // video is confirmed to be a Movie
        }

4. set<double>:
    An ordered container from <set> that stores unique values. Lookup
    with .count() runs in O(log n) time. Used here to validate that a rating is one of the allowed values.

        set<double> validRatings = {0.5, 1.0, 1.5, 2.0, 2.5,
                                    3.0, 3.5, 4.0, 4.5, 5.0};
        if (!validRatings.count(rate)) {
            cout << "Invalid rating.\n";
        }

5. stringstream ss(line):
    Creates an in-memory stream from a string, allowing it to be
    parsed token by token using getline with a delimiter. Requires
    <sstream>. Used here to split each line of the dataset by commas.

        stringstream ss(line);
        string token;
        getline(ss, token, ','); // reads up to the next comma

6. cin.clear():
    Resets the error flags on cin after a failed read (for example 
    when the user types a letter where a number was expected). Without 
    this, cin stays in a failed state and all subsequent reads are skipped.

        if (!(cin >> value)) {
            cin.clear(); // clear the error flag
        }

7. cin.ignore(numeric_limits<streamsize>::max(), '\n'):
    Discards characters from the input buffer up to and including the
    next newline. This is necessary after cin >> to remove the leftover
    '\n' before calling getline(), which would otherwise read an empty
    line. numeric_limits<streamsize>::max() means "discard as many
    characters as needed". Requires <limits>.

        cin >> option;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, title); // now reads correctly

