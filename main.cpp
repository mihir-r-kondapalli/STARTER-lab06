// Winter'24
// Instructor: Diba Mirza
// Student name: Mihir Kondapalli
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <sstream>
#include <map>
using namespace std;

#include "utilities.h"
#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile (argv[1]);
 
    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }
  
    // Create an object of a STL data-structure to store all the movies
    set<Movie> movies;

    string line, movieName;
    double movieRating;

    Movie* mov;

    // Read each file and store the name and rating
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
            // Use std::string movieName and double movieRating
            // to construct your Movie objects
            // cout << movieName << " has rating " << movieRating << endl;
            // insert elements into your data structure
            mov = new Movie(movieName, movieRating);
            movies.insert(*mov);
    }

    movieFile.close();

    if (argc == 2){
            //print all the movies in ascending alphabetical order of movie names

            set<Movie>::iterator itr;
            cout << fixed << setprecision(1);
            for(itr = movies.begin(); itr != movies.end(); itr++)
            {
                cout << (*itr).getTitle() << ", " << (*itr).getRating() << endl;
            }

            return 0;
    }

    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline (prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }

    map<string, set<Movie>* > values;

    int pref_counter = 0;
    set<Movie>* currPref = new set<Movie>();
    values[prefixes[pref_counter]] = currPref;

    set<Movie>::iterator itrM;

    for(itrM = movies.begin(); itrM != movies.end(); itrM++)
    {
        int comp = (*itrM).compPref(prefixes[pref_counter]);
        if(comp == 0)
        {
            (*currPref).insert(*itrM);
        }
        else if(comp == 1)
        {
            pref_counter++;
            if(pref_counter >= prefixes.size())
            {
                break;
            }
            currPref = new set<Movie>();
            values[prefixes[pref_counter]] = currPref;
        }
    }

    //  For each prefix,
    //  Find all movies that have that prefix and store them in an appropriate data structure
    //  If no movie with that prefix exists print the following message

    for(int i = 0; i<prefixes.size(); i++)
    {
        if(values.find(prefixes[i])==values.end() || (*values[prefixes[i]]).size()==0)
        {
            cout << "No movies found with prefix " << prefixes[i] << endl << endl;
        }
        else
        {
            Movie top = *(values[prefixes[i]]->begin());
            cout << "Best movie with prefix " << prefixes[i] << " is: " << top.getTitle() << " with rating " << std::fixed << std::setprecision(1) << top.getRating() << endl;
        }
    }

    //  For each prefix,
    //  Print the highest rated movie with that prefix if it exists.

    return 0;
}

/* Add your run time analysis for part 3 of the assignment here as commented block*/

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}