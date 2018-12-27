#ifndef LINECOLLECTION_H
#define LINECOLLECTION_H

#include <QHash>
#include <QStringList>
#include <QQuickItem>
#include "line.h"


namespace LineDraw {

#define PLOCK QMutexLocker o_locker(&m_mutex);

class LineCollection : public LinePainter  {
    Q_GADGET
public:
    LineCollection();
    virtual ~LineCollection(){}
    Q_INVOKABLE Line* append(const QString& pathName);
    Q_INVOKABLE void remove(const QString& pathName);
    Q_INVOKABLE Line* path(const QString& pathName);
    Q_INVOKABLE const Line* path(const QString& pathName) const;
    Q_INVOKABLE void bringOnTop(const QString& pathName);
public:
    QColor color() const;
    void setColor(const QColor& color);
    qreal thickness() const;
    void setThickness(qreal thickness);
    Line::Style style() const;
    void setStyle(Line::Style style);
    QStringList paths() const;
    QMutex* mutex() {return &m_mutex;}
public:
    virtual void colorChanged(const QColor& pathColor){}
    virtual void thicknessChanged(qreal thickness){}
    virtual void pathsChanged(){}
    virtual void styleChanged(Line::Style style){}
    virtual void clear();
    virtual void commit();
protected:
    virtual Line* build() = 0;
protected:
    QMutex m_mutex;
    QHash<QString, Line*> m_lines;
    QStringList m_keys;
    QColor m_pathColor = "#FFFFFF";
    Line::Style m_style = Line::STYLE_SOLID;
    qreal m_volume = 5; //5 is a pretty number, even odd
};



}

#endif // LINECOLLECTION_H
