#ifndef CARD_H
#define CARD_H

#include <string>
using namespace std;

class Movie
{
    public:

        Movie(string title, float rating);
        bool operator>(const Movie * other) const;
        bool operator==(const Movie * other) const;
        bool operator<(const Movie * other) const;
        string getTitle() const;
        int compPref(string pref) const;
        float getRating() const;

    private:
        string title;
        float rating;
};

#endif