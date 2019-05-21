#include "include/13_44_String.h"
#include <memory>
#include <cstring>
#include <iostream>
#include <algorithm>

std::allocator<char> String::alloc;

void String::push_back(char s)
{
    chk_n_alloc();
    alloc.construct(--first_free, s);
    alloc.construct(++first_free, '\0');
    first_free++;
}

std::pair<char*, char*>String::alloc_n_copy(const char* b, const char* e)
{
    auto data = alloc.allocate(e - b);
    return {data, std::uninitialized_copy(b, e, data)};
}
void String::range_initialize(const char* b, const char * e)
{
    auto newdata = alloc_n_copy(b, e);
    elements = newdata.first;
    first_free = cap = newdata.second; 
}

void String::free()
{
    if(elements)
    {
        std::for_each(elements, first_free, [this](char &c) { alloc.destroy(&c); });
        alloc.deallocate(elements, cap - elements);
    }
}

String::~String()
{
    free();
}
String::String(const char *ch)
{
    auto len = std::strlen(ch);
    range_initialize(ch, ch + len + 1);
}

String::String(const String & s)
{
    range_initialize(s.elements, s.first_free);
    std::cout << "copy constructor\n";
}

String& String::operator=(const String& rhs)
{
    auto data = alloc_n_copy(rhs.elements, rhs.first_free);
    free();
    elements = data.first;
    first_free = data.second;
    std::cout << "copy-assignment\n";
    return *this;
}

void String::reallocate()
{
    auto newcapacity = size() ? 2 * size() : 1;
    auto newdata = alloc.allocate(newcapacity + 1);

    auto dest = newdata;
    auto elem = elements;
    for (; elem != first_free; )
        alloc.construct(dest++, std::move(*elem++));
    free();

    elements = newdata;
    first_free = dest;
    cap = elements + newcapacity + 1;
}

