#ifndef LINECOLLECTION_H
#define LINECOLLECTION_H

#include <QHash>
#include <QStringList>
#include <QQuickItem>
#include "line.h"


namespace LineDraw {

constexpr auto DEFAULT_COLOR = Qt::black;
constexpr auto DEFAULT_VOLUME = 5;

class LineInterface {
public:
    virtual Line* build() = 0;
    virtual void signalPathsChanged() = 0;
    virtual void signalColorChanged(const QColor& pathColor) = 0;
    virtual void signalThicknessChanged(qreal thickness) = 0;
    virtual void signalStyleChanged(Line::Style style) = 0;
protected:
    static LinePrivate* priv(Line* p) {return p->m_p.data();}
};

class LineCollection : public LinePainter {
    Q_GADGET
public:
    LineCollection(LineInterface* item);
    virtual ~LineCollection() = default;
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
    QRecursiveMutex* mutex() {return &m_mutex;}
    auto begin() {return m_lines.begin();}
    auto end() {return m_lines.end();}
    auto begin() const {return m_lines.begin();}
    auto end() const {return m_lines.end();}
    auto count() const {return m_lines.count();}
    const QStringList& keys() const {return m_keys;}
    const Line* const at(const QString& k) const {return m_lines[k]; }
    Line* const at(const QString& k) {return m_lines[k]; }
public:
    void clearLines();
    void commitVertices();
protected:
    Line* build();
    void pathsChanged();
    void colorChanged(const QColor& pathColor);
    void thicknessChanged(qreal thickness);
    void styleChanged(Line::Style style);
protected:
    LineInterface* m_lineItem;
    QRecursiveMutex m_mutex{};
    QHash<QString, Line*> m_lines{};
    QStringList m_keys{};
    QColor m_pathColor = DEFAULT_COLOR;
    Line::Style m_style = Line::STYLE_SOLID;
    qreal m_volume = DEFAULT_VOLUME; //5 is a pretty number, even odd
};

class LineDecorator  {
public:
    QColor color() const;
    void setColor(const QColor& color);
    qreal thickness() const;
    void setThickness(qreal thickness);
    Line::Style style() const;
    void setStyle(Line::Style style);
};

}
#endif // LINECOLLECTION_H
