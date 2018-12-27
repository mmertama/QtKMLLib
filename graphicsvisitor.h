#ifndef GRAPHICSVISITOR_H
#define GRAPHICSVISITOR_H


#include <QVector>
#include <QGeoCoordinate>
#include "stylevisitor.h"


namespace QtKml{

class Graphics {
  public:
    enum Type{Unknown, PolyLine, Point, Polygon, Hole};
    typedef QVector<QGeoCoordinate> CoordinateList;
    typedef std::shared_ptr<Graphics> GraphicsPtr;
    typedef QVector<GraphicsPtr> GraphicsList;
    Graphics(QString styleId, Type type, kmldom::GeometryPtr geo) : m_styleId(styleId), m_type(type), m_ptr(geo){}
   // Graphics(const Graphics& graphs) : m_styleId(graphs.m_styleId), m_type(graphs.m_type), m_subgraphics(graphs.m_subgraphics){}
   // bool setStyles(const StyleParams& styles);
    //const StyleParams& styles(const StyleVisitor::StyleList* defaultStyles) const;
    const CoordinateList& vertices() const {return m_vertices;}
    const GraphicsList& subgraphics() const {return m_subgraphics;}
    Type type() const {return m_type;}
    QString styleId() const {return m_styleId;}
    const kmldom::GeometryPtr domObject() const {return m_ptr;}
    ~Graphics();
private:
    CoordinateList m_vertices;
    const QString m_styleId;
    //StyleParams m_styles;
    const Type m_type;
    const kmldom::GeometryPtr m_ptr;
    GraphicsList m_subgraphics;
    friend class GraphicsVisitor;
};



class GraphicsVisitor : public StyleVisitor{
public:
    GraphicsVisitor(Graphics::GraphicsList& graphics, StyleVisitor::StyleList& styles);
protected:
    void VisitPlacemark(const kmldom::PlacemarkPtr& element);
protected:
    Graphics::GraphicsList& m_graphics;
private:
    void addVertices(const kmldom::CoordinatesGeometryCommon* const element, Graphics* g) const;
    void visitInternal(const kmldom::PolygonPtr &element);
    void visitInternal(const kmldom::LinearRingPtr &element);
    void visitInternal(const kmldom::LineStringPtr &element);
    void visitInternal(const kmldom::PointPtr &element);
};

} //namespace
#endif // GRAPHICSVISITOR_H
