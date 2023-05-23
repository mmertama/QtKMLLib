#ifndef FIELDPATH_H
#define FIELDPATH_H

#include <QQuickItem>
#include <QRecursiveMutex>

#include "linecollection.h"

class QSGGeometryNode;

namespace LineDraw {

class LineDrawer : public QQuickItem, public LineCollection{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(qreal thickness READ thickness WRITE setThickness NOTIFY thicknessChanged)
    Q_PROPERTY(QStringList paths READ paths NOTIFY pathsChanged)
    Q_PROPERTY(Line::Style style READ style WRITE setStyle NOTIFY styleChanged)
signals:
    void colorChanged(const QColor& pathColor);
    void thicknessChanged(qreal thickness);
    void pathsChanged();
    void styleChanged(Line::Style style);
public:
    LineDrawer(QQuickItem*  parent = nullptr);
    ~LineDrawer();
public slots:
    void requestPaint();
private:
    class LineData;
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* update);
    QSGGeometryNode* addLineNode(QSGNode* parent, const LinePrivate& line) const;
    Line* build();
private:
    bool m_enforce = false;
    friend class LineDraw::LinePrivate;
};
}




#endif // FIELDPATH_H
