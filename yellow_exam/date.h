#pragma once

#include <iostream>

class Date {
public:
    Date(const int& year, const int& month, const int& day);

    int GetYear() const;
    int GetMonth() const;
    int GetDay() const;

private:
    int _year, _month, _day;
};

Date ParseDate(std::istream& is);

bool operator == (const Date& lhs, const Date& rhs);
bool operator < (const Date& lhs, const Date& rhs);
bool operator <= (const Date& lhs, const Date& rhs);
bool operator > (const Date& lhs, const Date& rhs);
bool operator >= (const Date& lhs, const Date& rhs);
bool operator != (const Date& lhs, const Date& rhs);

std::ostream& operator << (std::ostream& stream, const Date& date);
