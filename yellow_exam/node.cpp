#include <string>
#include "node.h"

using namespace std;

bool EmptyNode::Evaluate(const Date &date, const string &event) {
    return true;
}

bool DateComparisonNode::Evaluate(const Date& date, const string& str){
    if (comp_ == Comparison::Less) {
        return date < date_;
    }
    if (comp_ == Comparison::LessOrEqual) {
        return date <= date_;
    }
    if (comp_ == Comparison::Greater) {
        return date > date_;
    }
    if (comp_ == Comparison::GreaterOrEqual) {
        return date >= date_;
    }
    if (comp_ == Comparison::Equal) {
        return date == date_;
    }
    if (comp_ == Comparison::NotEqual) {
        return date != date_;
    }

    return true;
}

bool EventComparisonNode::Evaluate(const Date& date, const string& event){
    if(comp_ == Comparison::Equal){
        return event == event_;
    }
    if(comp_ == Comparison::Greater){
        return  event > event_;
    }
    if(comp_ == Comparison::GreaterOrEqual){
        return  event >= event_;
    }
    if(comp_ == Comparison::Less){
        return  event < event_;
    }
    if(comp_ == Comparison::LessOrEqual){
        return  event <= event_;
    }
    if(comp_ == Comparison::NotEqual){
        return  event != event_;
    }

    return true;
}

bool LogicalOperationNode::Evaluate(const Date& date, const string& str){
    if(oper_ == LogicalOperation::And){
        return lhs_->Evaluate(date, str) && rhs_->Evaluate(date, str);
    }
    if(oper_ == LogicalOperation::Or){
        return lhs_->Evaluate(date, str) || rhs_->Evaluate(date, str);
    }

    return true;
}