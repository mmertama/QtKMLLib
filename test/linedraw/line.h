#ifndef LINE_H
#define LINE_H

#include <QColor>
#include <QObject>
#include <QPointF>
#include <QMutex>

namespace LineDraw{

class LinePrivate;
class LinePainter;

void registerTypes();



class Line : public QObject {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(qreal thickness READ thickness WRITE setThickness NOTIFY thicknessChanged)
    Q_PROPERTY(Line::Style style READ style WRITE setStyle NOTIFY styleChanged)
    Q_PROPERTY(int len READ len WRITE setLen NOTIFY lenChanged)
    Q_PROPERTY(int offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(int verticesCount READ verticesCount NOTIFY verticesCountChanged)
public:
    enum  Style{STYLE_SOLID, STYLE_DASH, STYLE_DOTS};
    enum  Constants {MAXLEN = 0xFFFFFFF};
    Q_ENUM(Constants)
    Q_ENUM(Style)
public:
    Line(QObject* parent = nullptr);
    Line(LinePrivate* priv, QObject* parent);
    ~Line();
signals:
    void colorChanged(const QColor& color);
    void thicknessChanged(qreal thickness);
    void lineChanged();
    void styleChanged(Style style);
    void lenChanged(int len);
    void offsetChanged(int offset);
    void verticesCountChanged(int verticesCount);
public slots:
    void set(const QVector<QPointF>& values);
    void insert(int index, const QVector<QPointF>& values);
    void append(const QVector<QPointF>& values);
    void appendVertex(const QPointF& value);
    void clearLine();
    void commitVertices();
public:
    QColor color() const;
    void setColor(const QColor& color);
    qreal thickness() const;
    void setThickness(qreal thickness);
    Style style() const;
    void setStyle(Style style);
    int offset() const;
    void setOffset(int offset);
    int len() const;
    void setLen(int len);
    int verticesCount() const;
    class VertexPtr{
    public:
        VertexPtr(const QPointF* ptr, QRecursiveMutex* lock) : m_ptr(ptr), m_lock(lock){m_lock->lock();}
        ~VertexPtr(){m_lock->unlock();}
        const QPointF* operator->() {return m_ptr;}
        const QPointF* data() const {return m_ptr;}
        VertexPtr(VertexPtr&&) = default;
    private:
        const QPointF* m_ptr;
        QRecursiveMutex* m_lock;
    };
    VertexPtr vertices() const;
protected:
    QScopedPointer<LinePrivate> m_p;
    friend class LineInterface;
};

class LinePainter{
public:
    virtual QColor color() const = 0;
    virtual qreal thickness() const = 0;
    virtual Line::Style style() const = 0;
    virtual QRecursiveMutex* mutex() = 0;
    virtual Line* build() = 0;
    virtual ~LinePainter() = default;
};


}

Q_DECLARE_METATYPE(const LineDraw::Line*)
Q_DECLARE_METATYPE(LineDraw::Line*)
Q_DECLARE_INTERFACE(LineDraw::Line, "LineDraw/1.0")

#endif // LINE_H
