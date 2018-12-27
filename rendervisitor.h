#ifndef RENDERVISITOR_H
#define RENDERVISITOR_H

#include <QHash>
#include "graphicsvisitor.h"

class QPainter;

namespace QtKml{

class UrlDataProvider{
public:
    virtual const QByteArray* urlData(QString url) const = 0;
};

class CustomStyleProvider{
public:
    virtual StyleParams customStyle(const StyleParams& defaultStyle, const QString& styleId) const = 0;
};

class RenderVisitor : public GraphicsVisitor{
public:
    static RenderVisitor* create(const UrlDataProvider& urls, const CustomStyleProvider& styles);
    void render(QPainter& painter) const;
    void calculateProjection(const QSizeF size, const QPointF& center, qreal zoomFactor);
    ~RenderVisitor();
    typedef QVector<QPointF> PointList;
    const StyleList& styleList() const {Q_ASSERT(m_ptrStyles); return *(m_ptrStyles.get());}
private:
    void drawPolygon(QPainter& painter, const Graphics::GraphicsPtr& g, const PointList& p) const;
    void drawPoint(QPainter& painter, const StyleParams& s, const QPointF& p) const;
    void insertPoints(const Graphics::GraphicsPtr& g, const QSizeF size, const QPointF& center, qreal zoomFactor);
protected:
    RenderVisitor(Graphics::GraphicsList* graphics, StyleList* styles, const UrlDataProvider& urls, const CustomStyleProvider& styleProvider);
    const UrlDataProvider& m_urls;
    const CustomStyleProvider& m_styles;
    qreal m_zoomFactor;
    std::unique_ptr<Graphics::GraphicsList> m_ptrGraphics = nullptr;
    std::unique_ptr<StyleList> m_ptrStyles = nullptr;
    QHash<Graphics*, PointList> m_polygons;
};

} //namespace

#endif // RENDERVISITOR_H
