#ifndef LINEITEM_H
#define LINEITEM_H

#include <QQuickPaintedItem>
#include <QPainter>
#include "linecollection.h"

namespace LineDraw {

class LineItem : public QQuickPaintedItem, public LineCollection{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(qreal thickness READ thickness WRITE setThickness NOTIFY thicknessChanged)
    Q_PROPERTY(QStringList paths READ paths NOTIFY pathsChanged)
    Q_PROPERTY(Line::Style style READ style WRITE setStyle NOTIFY styleChanged)
public:
    LineItem(QQuickItem* parent = nullptr);
    void paint(QPainter* painter);
protected:
    Line* build();
signals:
    void colorChanged(const QColor& pathColor);
    void thicknessChanged(qreal thickness);
    void pathsChanged();
    void styleChanged(Line::Style style);
};

}

#endif // LINEITEM_H
