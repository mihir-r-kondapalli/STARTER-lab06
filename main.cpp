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
#include <unordered_map>
using namespace std;

#include "utilities.h"
#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);

// binary searches the set inclusive on both sides
bool binarySearch(int& begin, int end, string prefix, set<Movie>::iterator& itr)
{   
    advance(itr, (end-begin)/2);
    int comp;
    
    while(begin < end-1)
    {   
        comp = itr->compPref(prefix);

        if(comp == -1)
        {
            begin += (end-begin)/2;
            advance(itr, (end-begin)/2);
        }
        else if(comp == 0 || comp == 1)
        {
            end -= (end-begin)/2;
            advance(itr, (begin-end)/2);
        }
    }

    if(itr->compPref(prefix)==-1)
    {
        itr++;
        begin++;
    }

    return itr->compPref(prefix) == 0;
}

struct alphaComp {

    bool operator()(const Movie& m1, const Movie& m2) const
    {
        return m1.getTitle() < m2.getTitle();
    }
};


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
  
    // Create an object of a STL data-structure to store all the movie
    // alphabetical set
    set<Movie, alphaComp> movies;

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


    
    //Set Binary Search Tester

    /*set<Movie>::iterator itrTest;
    cout << fixed << setprecision(1);
    for(itrTest = movies.begin(); itrTest != movies.end(); itrTest++)
    {
        cout << itrTest->getTitle() << ", " << itrTest->getRating() << endl;
    }

    cout << endl;

    itrTest = movies.begin();
    int start = 0;
    bool success = binarySearch(start, movies.size()-1, "n", itrTest);
    cout << boolalpha << success << endl;
    cout << itrTest->getTitle() << endl;
    return 0;*/


    if (argc == 2){
            //print all the movies in ascending alphabetical order of movie names

            set<Movie, alphaComp>::iterator itr;
            cout << fixed << setprecision(1);
            for(itr = movies.begin(); itr != movies.end(); itr++)
            {
                cout << itr->getTitle() << ", " << itr->getRating() << endl;
            }

            return 0;
    }

    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    set<string> prefixSet;
    while (getline (prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
            prefixSet.insert(line);
        }
    }

    unordered_map< string, set<Movie>* > values;
    set<string>::iterator pref_itr = prefixSet.begin();

    /*
    // rating sorted movie set
    set<Movie>* currPrefSet = new set<Movie>();
    values[*pref_itr] = currPrefSet;
    int comp;

    set<Movie>::iterator itrM = movies.begin();

    
    old while loop
    while(itrM != movies.end())
    {   
        comp = itrM->compPref(*pref_itr);
        if(comp == 0)
        {
            currPrefSet->insert(*itrM);
            itrM++;
        }
        else if(comp == 1)
        {
            pref_itr++;

            if(pref_itr == prefixSet.end())
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
    }*/

    int start_i = 0;
    int end_i = movies.size()-1;
    int comp;

    set<Movie>* newSet;
    set<Movie>::iterator itrM = movies.begin();

    while(itrM != movies.end() && pref_itr != prefixSet.end())
    {
        comp = itrM->compPref(*pref_itr);

        if(comp == 0 || (comp==-1 && binarySearch(start_i, end_i, *pref_itr, itrM)))
        {
            newSet = new set<Movie>();

            while(itrM!=movies.end() && itrM->compPref(*pref_itr)==0)
            {
                newSet->insert(*itrM);
                itrM++;
                start_i++;
            }

            values[*pref_itr] = newSet;
        }
        else
        {
            values[*pref_itr] = new set<Movie>();
        }

        pref_itr++;
    }

    for(; pref_itr != prefixSet.end(); pref_itr++)
    {
        values[*pref_itr] = new set<Movie>();
    }

    //  For each prefix,
    //  Find all movies that have that prefix and store them in an appropriate data structure
    //  If no movie with that prefix exists print the following message

    cout << fixed << setprecision(1);
    string max_name;
    float max_r = 0;

    set<Movie>::iterator itr;

    for(int i = 0; i < prefixes.size(); i++)
    {
        if(values[prefixes[i]]->size()==0)
        {
            cout << "No movies found with prefix " << prefixes[i];
        }
        else
        {
            for(itr = values[prefixes[i]]->begin(); itr != values[prefixes[i]]->end(); itr++)
            {
                cout << itr->getTitle() << ", " << itr->getRating() << endl;
            }
        }

        cout << endl;
    }

    set<Movie>::iterator top;

    for(int i = 0; i < prefixes.size(); i++)
    {   
        if(values[prefixes[i]]->size()!=0)
        {
            top = values[prefixes[i]]->begin();
            cout << "Best movie with prefix " << prefixes[i] << " is: " << top->getTitle() << " with rating " << top->getRating() << endl;
        }
    }

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