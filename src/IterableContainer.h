#pragma once
// todo:全部include进来会不会导致程序变大
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>

namespace Nlog
{
template <typename T, typename Container>
class IterableContainer
{
  public:
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;

    IterableContainer() {}

    virtual ~IterableContainer() {}

    iterator begin() { return GetContainer().begin(); }
    iterator end() { return GetContainer().end(); }

  private:
    virtual Container& GetContainer() = 0;
};

template <typename T, typename Container = std::deque<T>>
class IterableQueue : public IterableContainer<T, Container>, public std::queue<T, Container>
{
  public:
    IterableQueue(std::queue<T, Container> q)
    {
        std::size_t cnt = 0;
        // todo 3 + 1，多push进去一个，这样能生成省略号
        while(cnt++ < 3 + 1 && !q.empty())
        {
            this->push(q.front());
            q.pop();
        }
    }

  private:
    inline Container& GetContainer() { return this->c; }
};

template <typename T, typename Container = std::vector<T>, typename Compare = std::less<typename Container::value_type>>
class IterablePriorityQueue : public IterableContainer<T, Container>, public std::priority_queue<T, Container, Compare>
{
  public:
    IterablePriorityQueue(std::priority_queue<T, Container, Compare> q)
    {
        std::size_t cnt = 0;
        // todo 3 + 1
        while(cnt++ < 3 + 1 && !q.empty())
        {
            this->push(q.top());
            q.pop();
        }
    }
  private:
    inline Container& GetContainer() { return this->c; }
};

template <typename T, typename Container = std::deque<T>>
class IterableStack : public IterableContainer<T, Container>, public std::stack<T, Container>
{
  public:
    IterableStack(std::stack<T, Container> s)
    {
        std::size_t cnt = 0;
        // todo 3 + 1
        while(cnt++ < 3 + 1 && !s.empty())
        {
            this->push(s.top());
            s.pop();
        }
    }
  private:
    inline Container& GetContainer() { return this->c; }
};
}