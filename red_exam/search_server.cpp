#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>

using namespace std;

vector<string> SplitIntoWords(const string& line) {
    istringstream words_input(line);
    return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

void UpdateDocumentBaseAsync(istream& document_input, Synchronized<InvertedIndex>& index) {
    InvertedIndex new_index;
    for (string current_document; getline(document_input, current_document);) {
        new_index.Add(move(current_document));
    }
    swap(index.GetAccess().ref_to_value, new_index);
}

SearchServer::SearchServer(istream& document_input) {
    UpdateDocumentBaseAsync(document_input, index);
}

void SearchServer::UpdateDocumentBase(istream& document_input){
    f.push_back(async(UpdateDocumentBaseAsync, ref(document_input), ref(index)));
}

void AddQueriesStreamAsync(istream& query_input, ostream& search_results_output, Synchronized<InvertedIndex>& index) {
    size_t size = index.GetAccess().ref_to_value.GetSize();
    vector<pair<size_t, size_t>>  docid_count(size);

    for (string current_query; getline(query_input, current_query);)
    {
        for (const auto& word : SplitIntoWords(current_query)) {
            for (const auto& docid : index.GetAccess().ref_to_value.Lookup(word)) {
                auto hit = docid_count[docid.first].second;
                docid_count[docid.first] = { docid.first, docid.second + hit };
            }
        }

        partial_sort(
                docid_count.begin(),
                docid_count.begin() + min<size_t>(5, size),
                docid_count.end(),
                [](pair<size_t, size_t>& lhs, pair<size_t, size_t>& rhs) {
                    int64_t lhs_docid = lhs.first;
                    auto lhs_hit_count = lhs.second;
                    int64_t rhs_docid = rhs.first;
                    auto rhs_hit_count = rhs.second;
                    return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
                }
        );

        search_results_output << current_query << ':';
        for (auto [docid, hitcount] : Head(docid_count, 5)) {
            if(hitcount != 0){
                search_results_output << " {"
                                      << "docid: " << docid << ", "
                                      << "hitcount: " << hitcount << '}';
            }
        }
        search_results_output << "\n";
        docid_count.assign(size, { 0, 0 });
    }
}

void SearchServer::AddQueriesStream(istream& query_input,
                                    ostream& search_results_output) {
    f.push_back(async(AddQueriesStreamAsync, ref(query_input), ref(search_results_output), ref(index)));
}

void InvertedIndex::Add(const string& document) {
    docs.push_back(move(document));
    const size_t docid = docs.size() - 1;
    for (const auto& word : SplitIntoWords(docs.back())) {
        if (empty(index[word]) || index[word].back().first != docid) {
            index[word].emplace_back(docid, 1);
        } else {
            index[word].back().second++;
        }
    }
}

const vector<pair<size_t, size_t>>& InvertedIndex::Lookup(const string& word) {
    auto it = index.find(word);
    if (it != index.end()) {
        return it->second;
    }
    else {
        return {};
    }
}