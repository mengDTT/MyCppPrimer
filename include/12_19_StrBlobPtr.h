# pragma once

#include <memory>
#include <vector>
#include <string>
#include <exception>
using namespace std;

class StrBlobPtr;
class StrBlob{
    friend class StrBlobPtr;
    friend bool operator==(const StrBlob &lhs, const StrBlob &rhs);
    friend bool operator!=(const StrBlob &lhs, const StrBlob &rhs);
    friend bool operator<(const StrBlob &lhs, const StrBlob &rhs);
  public:

    typedef vector<string>::size_type size_type;
    StrBlob();
    StrBlob(initializer_list<string> il);

    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }

    void push_back(const string &t) { data->push_back(t); }
    void pop_back();

    string &front();
    const string &front() const;
    string &back();
    const string &back() const;

    StrBlobPtr begin();
    StrBlobPtr end();
    string &operator[](size_t n);
    const string &operator[](size_t n) const;

private:
    shared_ptr<vector<string>> data;
    void check(size_type i, const string &msg) const;
};

class StrBlobPtr{
    friend bool operator==(const StrBlobPtr &lhs, const StrBlobPtr &rhs);
    friend bool operator!=(const StrBlobPtr &lhs, const StrBlobPtr &rhs);
    friend bool operator<(const StrBlobPtr &lhs, const StrBlobPtr &rhs);
    public:
    StrBlobPtr():curr(0){}
    StrBlobPtr(StrBlob &a,size_t sz =0):wptr(a.data),curr(sz){}
    string &deref() const;
    StrBlobPtr &incr();
    bool operator!=(StrBlobPtr& rhs){
        return wptr.lock() != rhs.wptr.lock() ||
               curr != rhs.curr;
    }
    string &operator[](size_t n);
    const string &operator[](size_t n) const;
    StrBlobPtr &operator++();
    StrBlobPtr operator++(int);
    StrBlobPtr &operator--();
    StrBlobPtr operator--(int);
    StrBlobPtr& operator+=(size_t n);
    StrBlobPtr operator+(size_t n);
    StrBlobPtr& operator-=(size_t n);
    StrBlobPtr operator-(size_t n);
    string& operator*() const
    {
        auto p = check(curr, "dereference past end");
        return (*p)[curr];
    }
    string * operator->() const 
    {
        return &this->operator*();
    }

private:
    shared_ptr<vector<string>> check(size_t, const string &) const;
    weak_ptr<vector<string>> wptr;
    size_t curr;
};

bool operator==(const StrBlob &lhs, const StrBlob &rhs)
{
    return *lhs.data == *rhs.data;
}

bool operator!=(const StrBlob &lhs, const StrBlob &rhs)
{
    return !(lhs == rhs);
}

bool operator<(const StrBlob &lhs, const StrBlob &rhs)
{
    return std::lexicographical_compare(lhs.data->begin(), lhs.data->end(), rhs.data->begin(), rhs.data->end());
}
string& StrBlob::operator[](size_t n)
{
    check(n, "out of range");
    return (*data)[n];
}
const string& StrBlob::operator[](size_t n) const
{
    check(n, "out of range");
    return (*data)[n];
}

bool operator==(const StrBlobPtr &lhs, const StrBlobPtr &rhs)
{
    return lhs.curr == rhs.curr;
}
bool operator!=(const StrBlobPtr &lhs, const StrBlobPtr &rhs)
{
    return !(lhs == rhs);
}

bool operator<(const StrBlobPtr &lhs, const StrBlobPtr &rhs)
{
    return lhs.curr < rhs.curr;
}
StrBlob::StrBlob(): data(make_shared<vector<string>>()){}
StrBlob::StrBlob(initializer_list<string> il):data(make_shared<vector<string>>(il)){}

void StrBlob::check(size_type i, const string &msg) const
{
    if(i >= data->size())
        throw out_of_range(msg);
}
string& StrBlob::front()
{
    check(0, "front on empty StrBlob");
    return data->front();
}
string& StrBlob::back()
{
    check(0, "back on empty StrBlob");
    return data->back();
}

void StrBlob::pop_back()
{
    check(0, "pop_back on empty StrBlob");
    data->pop_back();
}
const string& StrBlob::front() const
{
    check(0, "front on empty StrBlob");
    return data->front();
}
const string& StrBlob::back() const
{
    check(0, "back on empty StrBlob");
    return data->back();
}

shared_ptr<vector<string>> StrBlobPtr::check(size_t i, const string&msg) const
{
    auto ret = wptr.lock();
    if(!ret)
        throw runtime_error("unbound StrBlobPtr");
    if(i >= ret->size())
        throw out_of_range(msg);
    return ret;
}
string& StrBlobPtr::deref() const
{
    auto p = check(curr, "dereference past end");
    return (*p)[curr];
}

StrBlobPtr& StrBlobPtr::incr()
{
    check(curr, "increment past end of  StrblobPtr");
    ++curr;
    return *this;
}

StrBlobPtr StrBlob::begin() { return StrBlobPtr(*this); }
StrBlobPtr StrBlob::end(){
    auto ret = StrBlobPtr(*this, data->size());
    return ret;
}

string& StrBlobPtr::operator[](size_t n)
{
    auto data = check(n, "out of range");
    return (*data)[n];
}
const string& StrBlobPtr::operator[](size_t n) const
{
    auto data = check(n, "out of range");
    return (*data)[n];
}
StrBlobPtr & StrBlobPtr:: operator++()
{
    check(curr, "increment past end of StrBlobPtr");
    ++curr;
    return *this;
}
StrBlobPtr StrBlobPtr::operator++(int)
{
    StrBlobPtr ret = *this;
    ++*this;
    return ret;
}
StrBlobPtr &StrBlobPtr::operator--()
{
    --curr;
    check(curr, "decrement past begin of StrBlobPtr");
    return *this;
}
StrBlobPtr StrBlobPtr::operator--(int)
{
    StrBlobPtr ret = *this;
    --*this;
    return ret;
}

StrBlobPtr& StrBlobPtr::operator+=(size_t n)
{
    curr += n;
    check(curr, "increment past the end of StrBlobPtr");
    return *this;
}

StrBlobPtr& StrBlobPtr::operator-=(size_t n)
{
    curr -= n;
    check(curr, "decrement past the start of StrBlobPtr");
    return *this;
}
StrBlobPtr StrBlobPtr::operator+(size_t n)
{
    StrBlobPtr ret = *this;
    ret += n;
    return ret;
}
StrBlobPtr StrBlobPtr::operator-(size_t n)
{
    StrBlobPtr ret = *this;
    ret -= n;
    return ret;
}