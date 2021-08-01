#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <set>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class PriorityCollection {
public:
  using Id = int;

  PriorityCollection() : prior_vec(1000000, -1){}
  Id Add(T object){
      vec.push_back(move(object));
      priority_id.insert({0, vec.size() - 1});
      prior_vec[vec.size() - 1] = 0;
      return vec.size() - 1;
  }

  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin){
      for(auto it = range_begin; it != range_end; ++it){
          *(ids_begin++) = move(Add(move(*it)));
      }
  }

  bool IsValid(Id id) const{
      return prior_vec[id] != -1;
  }

  const T& Get(Id id) const{
      return vec[id];
  }

  void Promote(Id id){
      auto it = priority_id.find({prior_vec[id], id});
      auto m = priority_id.extract(*it).value();
      priority_id.insert({m.first + 1, m.second});
      prior_vec[id]++;
  }

  pair<const T&, int> GetMax() const{
      auto it = priority_id.rbegin();
      return {move(vec.at(it->second)), it->first};
  }

  pair<T, int> PopMax(){
      auto it = priority_id.rbegin();
      int prior = it->first, id = it->second;
      priority_id.extract(*it);
      prior_vec[id] = -1;
      return {move(vec.at(id)), prior };
  }

private:
  set<pair<int, Id>> priority_id;
  vector<int> prior_vec;
  vector<T> vec;
};


class StringNonCopyable : public string {
public:
  using string::string;
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

void TestMax(){
    PriorityCollection<char> chars;
    {
        chars.Add('a');
        chars.Promote(0);
        chars.Add('b');
        chars.Add('c');
        chars.Promote(2);
        ASSERT_EQUAL(chars.GetMax().first, 'c');
        ASSERT_EQUAL(chars.GetMax().second, 1);

        auto t = chars.PopMax();
        ASSERT_EQUAL(t.first, 'c');
        ASSERT_EQUAL(t.second, 1);

        chars.Promote(0);
        ASSERT_EQUAL(chars.GetMax().first, 'a');
        ASSERT_EQUAL(chars.GetMax().second, 2);
    }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  RUN_TEST(tr,TestMax);
  return 0;
}
