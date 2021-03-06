
#pragma once

#include <memory>
#include <iostream>
using std::ostream;

class String
{
  friend bool operator==(const String &lhs, const String &rhs);
  friend bool operator!=(const String &lhs, const String &rhs);
  friend bool operator<(const String &lhs, const String &rhs);

public:
  typedef size_t size_type;
  String() : String("") {}
  String(const char *);
  String(const String &);
  String(String &&) noexcept;
  String &operator=(const String &);
  String &operator=(String &&);
  ~String();

  void push_back(char);

  const char *c_str() const { return elements; }
  size_type size() const { return first_free - elements - 1; }
  size_type length() const { return size(); }
  size_type capacity() const { return cap - elements - 1; }
  char *begin() const { return elements; }
  char *end() const { return first_free - 1; }
  friend ostream &operator<<(ostream &os, const String &s)
  {
    os << s.begin();
    return os;
  }
  char &operator[](size_t n);
  const char &operator[](size_t n) const;

private:
  std::pair<char *, char *> alloc_n_copy(const char *, const char *);
  void range_initialize(const char *, const char *);
  void chk_n_alloc()
  {
    if (size() == capacity())
      reallocate();
  }
  void free();
  void reallocate();

private:
  char *elements;
  char *first_free;
  char *cap;
  static std::allocator<char> alloc;
};