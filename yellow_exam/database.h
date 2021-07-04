#pragma once

#include "date.h"
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <set>

class Database {
public:
    void Add(const Date &date, const std::string& event);

    void Print(std::ostream& stream) const;

    template<typename Pred>
    int RemoveIf(Pred pred)
    {
        int counter = 0;
        for (auto it = base_vec.begin(); it != base_vec.end();) {
            auto border = std::stable_partition(it->second.begin(), it->second.end(),
                                                [it, pred](std::string &event)
                                                {return pred(it->first, event);});

            for(auto sit = it->second.begin(); sit != border; ++sit){
                base_set[it->first].erase(*sit);
            }
            it->second.erase(it->second.begin(), border);
            counter += border - it->second.begin();

            if (it->second.empty()){
                base_set.erase(it->first);
                it = base_vec.erase(it);
            }
            else{
                ++it;
            }
        }
        return counter;
    }

    template<typename Pred>
    std::vector<std::string> FindIf(Pred pred) const{
        std::vector<std::string> answer;

        for (auto& date : base_vec)
        {

            for (auto& date_event : date.second)
            {
                if (pred(date.first, date_event))
                {
                    std::string result;
                    std::stringstream ss;
                    ss << date.first << " " << date_event;
                    result = ss.str();
                    answer.push_back(result);
                }
            }
        }
        return answer;
    }

    std::pair<Date, std::string> Last(const Date& date) const;

private:
    std::map<Date, std::vector<std::string>> base_vec;
    std::map<Date, std::set<std::string>> base_set;
};

std::ostream& operator << (std::ostream& stream, const std::pair<Date, std::string>& p);