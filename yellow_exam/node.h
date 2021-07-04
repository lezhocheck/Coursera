#pragma once

#include "date.h"
#include <memory>

enum class Comparison {
    Equal,
    Greater,
    GreaterOrEqual,
    Less,
    LessOrEqual,
    NotEqual
};

enum class LogicalOperation {
    And,
    Or,
};

class Node {
public:
    virtual bool Evaluate(const Date& date, const std::string& event) = 0;
};

class EmptyNode : public Node {
public:
    bool Evaluate(const Date& date, const std::string& event) override;
};

class DateComparisonNode : public Node {
public:
    DateComparisonNode(Comparison cmp, Date date)
            : comp_(cmp), date_(date) {}

    bool Evaluate(const Date& date, const std::string& event) override;

private:
    const Comparison comp_;
    const Date date_;
};

class EventComparisonNode : public Node {
public:
    EventComparisonNode(Comparison cmp, std::string event)
            : comp_(cmp), event_(event) {}

    bool Evaluate(const Date& date, const std::string& event) override;

private:
    const Comparison comp_;
    const std::string event_;
};

class LogicalOperationNode : public Node {
public:
    LogicalOperationNode(LogicalOperation op,
                         std::shared_ptr<Node> lhs,
                         std::shared_ptr<Node> rhs)
            : oper_(op), lhs_(lhs), rhs_(rhs) {}

    bool Evaluate(const Date& date, const std::string& event) override;

private:
    LogicalOperation oper_;
    std::shared_ptr<Node> lhs_, rhs_;
};