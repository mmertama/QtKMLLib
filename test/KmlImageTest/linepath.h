#ifndef FIELDPATH_H
#define FIELDPATH_H

#include <QQuickItem>
#include <QMutex>

#include "linecollection.h"

class QSGGeometryNode;

namespace LineDraw {

class LineDrawer : public QQuickItem, public LineCollection{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(qreal thickness READ thickness WRITE setThickness NOTIFY thicknessChanged)
    Q_PROPERTY(QStringList paths READ paths NOTIFY pathsChanged)
    Q_PROPERTY(Line::Style style READ style WRITE setStyle NOTIFY styleChanged)
/*public:
    Q_INVOKABLE Line* append(const QString& pathName);
    Q_INVOKABLE void remove(const QString& pathName);
    Q_INVOKABLE Line* path(const QString& pathName);
    Q_INVOKABLE const Line* path(const QString& pathName) const;
    Q_INVOKABLE void bringOnTop(const QString& pathName);*/
signals:
    void colorChanged(const QColor& pathColor);
    void thicknessChanged(qreal thickness);
    void pathsChanged();
    void styleChanged(Line::Style style);
public:
    LineDrawer(QQuickItem*  parent = nullptr);
    ~LineDrawer();
  //  void requestUpdate();
/*    QColor color() const;
    void setColor(const QColor& color);
    qreal thickness() const;
    void setThickness(qreal thickness);
    Line::Style style() const;
    void setStyle(Line::Style style);
    QStringList paths() const;
    QMutex* mutex() {return &m_mutex;}
public slots:
    void clear();
    void requestPaint();
    void restorePaint();
    void commit(); */
public slots:
    void requestPaint();
private:
    class LineData;
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* update);
    QSGGeometryNode* addLineNode(QSGNode* parent, const LinePrivate& line) const;
    Line* build();
private:
/*    QHash<QString, Line*> m_lines;
    QStringList m_keys;
    QColor m_pathColor = "#FFFFFF";
    Line::Style m_style = Line::STYLE_LINE;
    qreal m_volume = 5; //5 is a pretty number, even odd
    QMutex m_mutex; */
    bool m_enforce = false;
    friend class LineDraw::LinePrivate;
};
}




#endif // FIELDPATH_H
