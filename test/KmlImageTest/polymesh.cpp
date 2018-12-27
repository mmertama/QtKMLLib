#include "polymesh.h"
#include <QSGBasicGeometryNode>
#include <QSGFlatColorMaterial>
#include <QQuickWindow>

Polymesh::Polymesh(QQuickItem *parent) : QQuickItem (parent){
    setAntialiasing(true);
    QObject::connect(this, &Polymesh::polygonChanged,[this](){
       setFlag(ItemHasContents, !m_polygon.isEmpty());
       update();
    });
}

QPolygonF Polymesh::asPolygon() const{
    return m_polygon;
}

void Polymesh::setAsPolygon(const QPolygonF& polygon){
    if(m_polygon != polygon){
        m_polygon = polygon;
        emit polygonChanged();
    }
}

QPolygonF& Polymesh::polygon(){
    return m_polygon;
}

const QPolygonF& Polymesh::polygon() const{
    return m_polygon;
}

QSGGeometryNode* Polymesh::createNode(){
    auto node = new QSGGeometryNode;
    auto geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), m_polygon.count());
    geometry->setLineWidth(2);
    geometry->setDrawingMode(QSGGeometry::DrawTriangles);
    node->setGeometry(geometry);
    node->setFlag(QSGNode::OwnsGeometry);

    auto *material = new QSGFlatColorMaterial;
    material->setColor(QColor(255, 0, 0));
    node->setMaterial(material);
    node->setFlag(QSGNode::OwnsMaterial);

    return node;
}

QSGNode* Polymesh::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* update){
    auto node = oldNode ? static_cast<QSGGeometryNode*>(oldNode) : createNode();
    auto *geometry = node->geometry();
    if(geometry->vertexCount() != m_polygon.count()){
        geometry->allocate(m_polygon.count());
    }
    auto i = 0;
    auto *vertices = geometry->vertexDataAsPoint2D();
    for(const auto& v : m_polygon){
        vertices[i++].set(v.x(), v.y());
    }
    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}
