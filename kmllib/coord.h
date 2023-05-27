#ifndef COORD_H
#define COORD_H

#include <QPair>

namespace QtKml{

class Coord : public QPair<qreal, qreal>{
public:
    Coord() : QPair<qreal, qreal>(){}
    Coord(qreal lat, qreal lon) : QPair<qreal, qreal>(lat, lon){}
    Coord(const Coord& other) = default;
    Coord& operator=(const Coord& other)= default;
    qreal lat() const {return first;}
    qreal lon() const {return second;}
};
}

#endif // COORD_H

