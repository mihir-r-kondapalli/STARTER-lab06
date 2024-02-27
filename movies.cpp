#include <string>
#include "movies.h"

using namespace std;

Movie::Movie(string title, float rating)
{
    this->title = title;
    this->rating = rating;
}

bool Movie::operator>(const Movie & other) const
{
    return rating < other.getRating();
}

bool Movie::operator==(const Movie & other) const
{
    return rating == other.getRating();
}

bool Movie::operator<(const Movie & other) const
{
    return rating > other.getRating();
}

string Movie::getTitle() const
{
    return title;
}

int Movie::compPref(string pref) const
{
    if(title < pref){return -1;} // less than
    if(title.substr(0, pref.size()) == pref){return 0;} // match!
    return 1; // greater than
}

float Movie::getRating() const
{
    return rating;
}