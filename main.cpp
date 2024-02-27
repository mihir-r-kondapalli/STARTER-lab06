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
    set<string> prefixeSet;
    while (getline (prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
            prefixeSet.insert(line);
        }
    }

    map<string, set<Movie>* > values;

    set<string>::iterator pref_itr = prefixeSet.begin();
    set<Movie>* currPrefSet = new set<Movie>();
    values[*pref_itr] = currPrefSet;

    set<Movie>::iterator itrM = movies.begin();

    while(itrM != movies.end())
    {
        int comp = (*itrM).compPref(*pref_itr);
        if(comp == 0)
        {
            (*currPrefSet).insert(*itrM);
            itrM++;
        }
        else if(comp == 1)
        {
            pref_itr++;
            if(pref_itr == prefixeSet.end())
            {
                break;
            }
            currPrefSet = new set<Movie>();
            values[*pref_itr] = currPrefSet;
        }
        else
        {
            itrM++;
        }
    }

    //  For each prefix,
    //  Find all movies that have that prefix and store them in an appropriate data structure
    //  If no movie with that prefix exists print the following message

    set<Movie>::iterator itr;
    cout << fixed << setprecision(1);

    vector<float> top_ratings;
    float max_r = 0;

    for(int i = 0; i < prefixes.size(); i++)
    {
        max_r = 0;
        
        for(itr = values[prefixes[i]]->begin(); itr != values[prefixes[i]]->end(); itr++)
        {
            
            cout << (*itr).getTitle() << ", " << (*itr).getRating() << endl;

            if((*itr).getRating() > max_r){max_r = (*itr).getRating();}
        }

        cout << endl;
        top_ratings.push_back(max_r);
    }

    for(int i = 0; i < prefixes.size(); i++)
    {
        if((*values[prefixes[i]]).size()==0)
        {
            cout << "No movies found with prefix " << prefixes[i] << endl << endl;
        }
        else
        {
            Movie top = *(values[prefixes[i]]->begin());
            cout << "Best movie with prefix " << prefixes[i] << " is: " << top.getTitle() << " with rating " << top_ratings.at(i) << endl;
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