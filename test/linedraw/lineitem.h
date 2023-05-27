#ifndef LINEITEM_H
#define LINEITEM_H

#include <QQuickPaintedItem>
#include <QPainter>
#include "linecollection.h"

namespace LineDraw {

class LineItem : public QQuickPaintedItem, public LineCollection, public LineInterface {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(qreal thickness READ thickness WRITE setThickness NOTIFY thicknessChanged)
    Q_PROPERTY(QStringList lineNames READ paths NOTIFY pathsChanged)
    Q_PROPERTY(LineDraw::Line::Style style READ style WRITE setStyle NOTIFY styleChanged)
public:
    LineItem(QQuickItem* parent = nullptr);
    void paint(QPainter* painter);
protected:
    Line* build();
    void signalPathsChanged() {emit pathsChanged();}
    void signalColorChanged(const QColor& pathColor) {emit colorChanged(pathColor);}
    void signalThicknessChanged(qreal thickness) {emit thicknessChanged(thickness);}
    void signalStyleChanged(Line::Style style) {emit styleChanged(style);}
signals:
    void colorChanged(const QColor& pathColor);
    void thicknessChanged(qreal thickness);
    void pathsChanged();
    void styleChanged(Line::Style style);
};

}

#endif // LINEITEM_H
