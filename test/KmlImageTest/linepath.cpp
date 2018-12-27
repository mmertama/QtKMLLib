
#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgflatcolormaterial.h>
#include <QQuickWindow>
#include <QMutexLocker>

#include "linepath.h"
#include "line_p.h"

using namespace LineDraw;

static const QHash<Line::Style, int> StyleMap({
                                                           {Line::STYLE_DASH, GL_LINES},
                                                           {Line::STYLE_SOLID, GL_LINE_STRIP},
                                                           {Line::STYLE_DOTS, GL_POINTS}
                                                       });


#define PLOCK QMutexLocker o_locker(&m_mutex);

class LineDrawerPrivate : public LinePrivate{
public:
    LineDrawerPrivate(LinePainter* p) : LinePrivate(p){}
    QSGGeometryNode* node = nullptr;
};




Line* LineDrawer::build(){
    auto p  = new Line(new LineDrawerPrivate(this), this);
    QObject::connect(p, &Line::lineChanged, this, &LineDrawer::pathsChanged);
    QObject::connect(this, &LineDrawer::styleChanged, p, &Line::setStyle);
    QObject::connect(this, &LineDrawer::colorChanged, p, &Line::setColor);
    QObject::connect(this, &LineDrawer::thicknessChanged, p, &Line::setThickness);
    return p;
}


LineDrawer::LineDrawer(QQuickItem* parent) : QQuickItem(parent){
    qRegisterMetaType<QVector<QVector<QPointF>>>("QVector<QVector<QPointF>>");
    qRegisterMetaType<QVector<QPointF>>("QVector<QPointF>");
    QObject::connect(this, &LineDrawer::pathsChanged, [this](){
        m_enforce = true;
        requestPaint();
    });

    setAntialiasing(true);


    QObject::connect(this, &LineDrawer::windowChanged, [](QQuickWindow *win){
        if(win){
            auto format = win->format();
            format.setSamples(4);
            win->setFormat(format);
            win->setColor(Qt::transparent);
        }
    });
}

 LineDrawer::~LineDrawer(){
     PLOCK
 }


QSGGeometryNode* LineDrawer::addLineNode(QSGNode* node, const LinePrivate& line) const{
    auto chld = new QSGGeometryNode;
    auto geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), line.vertices.length());
    const auto style = StyleMap[line.style];
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

QSGNode* LineDrawer::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data){
    Q_UNUSED(data);
    PLOCK
    auto* node = oldNode;

    Q_ASSERT(window()->format().samples() >= 4);
//   if(node == nullptr){
//        node = new QSGNode();
//    }

    //TODO: remore true when Qt (?) Works
    if(m_enforce || node == nullptr || node->childCount() != m_lines.count()){
        delete node;
        node = new QSGNode();
        node->setFlag(QSGNode::OwnedByParent);
        m_enforce = false;
        for(const auto& k : m_keys){
            const auto line = m_lines[k];
            if(line->len() > 0){
                auto n = static_cast<LineDrawerPrivate*>(p(line));
                n->node = addLineNode(node, *n);
            }
        }
      //  qDebug() << "n DirtyNodeAdded";
        node->markDirty(QSGNode::DirtyNodeAdded);
    }

    int dirty = 0;
  //  auto c = node->firstChild();
    for(const auto& k : m_keys){
        const auto& line = static_cast<LineDrawerPrivate*>(p(m_lines[k]));
     //   Q_ASSERT(c->type() == QSGNode::GeometryNodeType);
    //    auto chld = static_cast<QSGGeometryNode*>(c);
        auto chld = line->node;

        if(line->flags & LinePrivate::MATERIAL){
            auto material = static_cast<QSGFlatColorMaterial*>(chld->material());
            auto geometry = chld->geometry();
            material->setColor(line->color);
            geometry->setLineWidth(static_cast<float>(line->volume));
            geometry->setDrawingMode(StyleMap[line->style]);
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



void LineDrawer::requestPaint(){
    //m_enforce = true;
    const bool hasLines = !m_lines.isEmpty();
    if(hasLines){
        setFlag(QQuickItem::ItemHasContents, true);
        update();
    }
}


