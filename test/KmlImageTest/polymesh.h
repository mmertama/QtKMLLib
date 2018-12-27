#ifndef POLYMESH_H
#define POLYMESH_H

#include <QQuickItem>
#include <QPolygonF>

class QSGGeometryNode;

class Polymesh : public QQuickItem{
    Q_OBJECT
    Q_PROPERTY(QPolygonF polygon READ asPolygon WRITE setAsPolygon NOTIFY polygonChanged);
public:
    explicit Polymesh(QQuickItem *parent = nullptr);
    QPolygonF asPolygon() const;
    void setAsPolygon(const QPolygonF& polygon);
    QPolygonF& polygon();
    const QPolygonF& polygon() const;
    Polymesh& operator=(const QVector<QPointF>& poly) {m_polygon = poly; emit polygonChanged(); return *this;}
signals:
    void polygonChanged();
private:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* update);
    QSGGeometryNode* createNode();
private:
    QPolygonF m_polygon;
};

Q_DECLARE_METATYPE(Polymesh*)
Q_DECLARE_METATYPE(QPolygon)
Q_DECLARE_INTERFACE(Polymesh, "com.corp.mars/1.0")

#endif // POLYMESH_H
