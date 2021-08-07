#pragma once

#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <utility>
#include <future>
#include <unordered_map>

template <typename T>
class Synchronized {
public:
    explicit Synchronized(T initial = T())
            : value(std::move(initial))
    {
    }
    struct Access {
        std::lock_guard<std::mutex> guard;
        T& ref_to_value;
    };
    Access GetAccess() {
        return {std::lock_guard(m), value};
    }
private:
    T value;
    std::mutex m;
};

class InvertedIndex {
public:
    void Add(const std::string& document);
    const std::vector<std::pair<size_t, size_t>>& Lookup(const std::string& word);
    const std::string& GetDocument(size_t id) const {
        return docs[id];
    }
    size_t GetSize() {
        return docs.size();
    }

private:
    std::unordered_map<std::string, std::vector<std::pair<size_t, size_t>>> index;
    std::deque<std::string> docs;
};

class SearchServer {
public:
    SearchServer() = default;
    explicit SearchServer(std::istream& document_input);
    void UpdateDocumentBase(std::istream& document_input);
    void AddQueriesStream(std::istream& query_input, std::ostream& search_results_output);

private:
    Synchronized<InvertedIndex> index;
    std::mutex m;
    std::vector<std::future<void>> f;
};