#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <QOpenGLBuffer>
#include <QQuickWindow>
#include <QMutexLocker>

#include "linepath.h"
#include "line_p.h"

using namespace LineDraw;

#define PLOCK QMutexLocker o_locker(&m_mutex);

const QHash<Line::Style, int>& gl_style()
{
    static const QHash<Line::Style, int> sm({
        {Line::STYLE_DASH, GL_LINES},
        {Line::STYLE_SOLID, GL_LINE_STRIP},
        {Line::STYLE_DOTS, GL_POINTS}
    });
    return sm;
}

class LineDrawerPrivate : public LinePrivate {
public:
    LineDrawerPrivate(LinePainter* p) : LinePrivate(p){}
    QSGGeometryNode* node = nullptr;
};




Line* LinePath::build(){
    auto p  = new Line(new LineDrawerPrivate(this), this);
    QObject::connect(p, &Line::lineChanged, this, &LinePath::pathsChanged);
    QObject::connect(this, &LinePath::styleChanged, p, &Line::setStyle);
    QObject::connect(this, &LinePath::colorChanged, p, &Line::setColor);
    QObject::connect(this, &LinePath::thicknessChanged, p, &Line::setThickness);
    return p;
}


LinePath::LinePath(QQuickItem* parent) : QQuickItem(parent), LineCollection(this) {
    qRegisterMetaType<QVector<QVector<QPointF>>>("QVector<QVector<QPointF>>");
    qRegisterMetaType<QVector<QPointF>>("QVector<QPointF>");
    QObject::connect(this, &LinePath::pathsChanged, [this](){
        m_enforce = true;
        requestPaint();
    });

    setAntialiasing(true);
    
    
    QObject::connect(this, &LinePath::windowChanged, this, [](QQuickWindow *win){
        if(win){
            auto format = win->format();
            format.setSamples(4);
            win->setFormat(format);
            win->setColor(Qt::transparent);
        }
    });
}

LinePath::~LinePath(){
     PLOCK
 }


QSGGeometryNode* LinePath::addLineNode(QSGNode* node, const LinePrivate& line) const{
    auto chld = new QSGGeometryNode;
    auto geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), line.vertices.length());
    const auto style = gl_style()[line.style];
    geometry->setDrawingMode(style);
    geometry->setLineWidth(static_cast<float>(line.volume));
    geometry->setVertexDataPattern(QSGGeometry::AlwaysUploadPattern);
    QSGFlatColorMaterial* material = new QSGFlatColorMaterial;
    material->setColor(line.color);

    chld->setGeometry(geometry);
    chld->setMaterial(material);

    chld->setFlag(QSGNode::OwnsGeometry);
    chld->setFlag(QSGNode::OwnedByParent);
    chld->setFlag(QSGNode::OwnsMaterial);

    node->appendChildNode(chld);
    return chld;
}

QSGNode* LinePath::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data){
    Q_UNUSED(data);
    PLOCK
    auto* node = oldNode;

    Q_ASSERT(window()->format().samples() >= 4);
//   if(node == nullptr){
//        node = new QSGNode();
//    }

    //TODO: remore true when Qt (?) Works
    if(m_enforce || node == nullptr || node->childCount() != count()) {
        delete node;
        node = new QSGNode();
        node->setFlag(QSGNode::OwnedByParent);
        m_enforce = false;
        for(const auto& k : keys()){
            const auto line = at(k);
            if(line->len() > 0){
                auto n = static_cast<LineDrawerPrivate*>(priv(line));
                n->node = addLineNode(node, *n);
            }
        }
      //  qDebug() << "n DirtyNodeAdded";
        node->markDirty(QSGNode::DirtyNodeAdded);
    }

    int dirty = 0;
  //  auto c = node->firstChild();
    for(const auto& k : keys()){
        const auto& line = static_cast<LineDrawerPrivate*>(priv(at(k)));
     //   Q_ASSERT(c->type() == QSGNode::GeometryNodeType);
    //    auto chld = static_cast<QSGGeometryNode*>(c);
        auto chld = line->node;

        if(line->flags & LinePrivate::MATERIAL){
            auto material = static_cast<QSGFlatColorMaterial*>(chld->material());
            auto geometry = chld->geometry();
            material->setColor(line->color);
            geometry->setLineWidth(static_cast<float>(line->volume));
            geometry->setDrawingMode(gl_style()[line->style]);
            material->setColor(line->color);
            dirty |= LinePrivate::MATERIAL;
        }
        if(line->flags & LinePrivate::VERTICES){
           auto geometry = chld->geometry();
           auto count = line->vertices.length();


           if(line->offset > 0){
               count -= line->offset;
           }

           if(line->len >= 0 && line->len < count){
               count = line->len;
           }


           if(geometry->vertexCount() < count){
                geometry->allocate(count);
            }


            auto* vertices = geometry->vertexDataAsPoint2D();
            for(auto i = line->offset; i < count; i++){
                const auto& v = line->vertices.at(i);
                vertices->set(static_cast<float>(v.x()), static_cast<float>(v.y()));
                ++vertices;
            }
            geometry->markVertexDataDirty();
            geometry->markIndexDataDirty();
        //    qDebug() << "c DirtyGeometry" << count;
            chld->markDirty(QSGNode::DirtyGeometry);
            dirty |= LinePrivate::VERTICES;
        }

        line->flags = LinePrivate::NONE;
       // c = c->nextSibling();
    }
    if(dirty & LinePrivate::VERTICES){
      //  qDebug() << "n DirtyGeometry";
        node->markDirty(QSGNode::DirtyGeometry);
    }
    if(dirty & LinePrivate::MATERIAL){
        node->markDirty(QSGNode::DirtyMaterial);
        node->markDirty(QSGNode::DirtyGeometry);
      //  qDebug() << "n DirtyMaterial";
      //  qDebug() << "n DirtyMaterial";
    }
   // qDebug() << "exit";
  //  node->markDirty(QSGNode::DirtyForceUpdate);
    return node;
}



void LinePath::requestPaint(){
    //m_enforce = true;
    const bool hasLines = std::distance(begin(), end());
    if(hasLines){
        setFlag(QQuickItem::ItemHasContents, true);
        update();
    }
}


