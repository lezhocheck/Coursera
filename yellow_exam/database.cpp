#include "database.h"
#include <iostream>

using namespace std;

void Database::Add(const Date& date, const string& str){
    if(base_set[date].insert(str).second){
        base_vec[date].push_back(str);
    }
}

void Database::Print(ostream& stream) const{
    for(const auto& p : base_vec){
        for(const auto& s : p.second){
            stream << p.first << " " << s << endl;
        }
    }
}

pair<Date, string> Database::Last(const Date& date) const{
    auto date_iter = base_vec.upper_bound(date);
    if (date_iter == base_vec.begin())
    {
        throw invalid_argument("No entries");
    }

    date_iter--;
    return make_pair(date_iter->first, date_iter->second.back());
}

ostream &operator<<(ostream& stream, const pair<Date, string> &p) {
    stream << p.first << " " << p.second;
    return stream;
}