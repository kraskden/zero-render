#ifndef GLPAINTER_TBUFFER_H
#define GLPAINTER_TBUFFER_H

#include "../obj/ObjModel.h"

class TBuffIterator : public std::iterator_traits<Face**>
{
    volatile Face** p;
    volatile Face** end;
public:
    TBuffIterator(volatile Face** start, volatile Face** end) : p(start), end(end) {}

    bool operator==(const TBuffIterator& other) const {return p == other.p;}
    bool operator!=(const TBuffIterator& other) const {return p != other.p; }
    volatile Face* operator*() const {return *p; }
    TBuffIterator& operator++() {
        while (++p < end) {
            if (*p) {break;}
        }
        return *this;
    }
    TBuffIterator& operator--() {return *this;}
    TBuffIterator& operator-(const TBuffIterator& other) {return *this;}
    TBuffIterator& operator+(const TBuffIterator& other) {return *this;}
};

#endif //GLPAINTER_TBUFFER_H
