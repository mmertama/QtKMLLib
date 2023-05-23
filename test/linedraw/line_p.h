#ifndef LINE_P_H
#define LINE_P_H

#include <QColor>
#include <QVector>
#include "line.h"


class LineDraw::LinePrivate{
public:
    enum {NONE, VERTICES = 1, MATERIAL = 2, VISUAL = (VERTICES|MATERIAL), RESET = 4};
public:
    LinePrivate(LinePainter* d) : color(d->color()),
        volume(d->thickness()),
        style(d->style()),
        lock((d->mutex())) {}
    QVector<QPointF> vertices;
    QColor color;
    qreal volume;

    Line::Style style;
    int offset = 0;
    int len = std::numeric_limits<int>::max();
    mutable int flags = 0;
    mutable QRecursiveMutex* lock;
};

#endif // LINE_P_H
