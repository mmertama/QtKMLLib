#include "stylevisitor.h"

#ifdef ALLOW_QDEBUG_PRINT
#include <QDebug>
#endif

using namespace QtKml;

//constexpr char StyleParams::DEFAULT_ICON[];

StyleVisitor::StyleVisitor(StyleList& styles) : m_styles(styles){
}

StyleVisitor::~StyleVisitor(){
}

/*
 * const StyleParams& StyleVisitor::style() const {return m_styles.find(m_currentStyle).value();}
QString StyleVisitor::currentStyleId() const {
    return m_currentStyle;
}


void StyleVisitor::VisitFeature(const kmldom::FeaturePtr &element){
    if(element->has_styleurl()){
         const QString label = QString::fromStdString(element->get_styleurl());
         m_currentStyle = label.at(0) == '#' ? label.mid(1) : label;
    }
}
*/

QString StyleVisitor::currentStyleId(const kmldom::ElementPtr& element) const {
    if(element == nullptr)
        return QString("");
    if(element->IsA(kmldom::Type_Feature)){
        const kmldom::FeaturePtr feature = kmldom::AsFeature(element);
        if(feature->has_styleurl()){
            const QString label = QString::fromStdString(feature->get_styleurl());
            const QString id = label.at(0) == '#' ? label.mid(1) : label;
#ifdef ALLOW_QDEBUG_PRINT
            qDebug() << "ID is" << id.length() << id << "!!";
#endif
            return id;
        }   
    }
    return currentStyleId(element->GetParent());
}

void StyleVisitor::VisitStyle(const kmldom::StylePtr &element){
    if(element->has_id()){
        QString styleId = QString::fromStdString(element->get_id());
        StyleParams style;

        if(element->has_linestyle()){
            kmldom::LineStylePtr lp = element->get_linestyle();
            if(lp->has_color()){
                style.setLineColor (lp->get_color().get_color_argb());
            }
            if(lp->has_width()){
                style.setLineWidth (lp->get_width());
            }
        }
        if(element->has_polystyle()){
            kmldom::PolyStylePtr ps = element->get_polystyle();
            if(ps->has_color()){
                style.setFillColor (ps->get_color().get_color_argb());
            }
           // if(ps->has_fill()){ has_fill is FALSE even it shall not be, bug in kmllib
            style.setIsFill(ps->get_fill());
          //  }
        }
        if(element->has_iconstyle()){
            kmldom::IconStylePtr is = element->get_iconstyle();
            if(is->has_icon()){
                style.setIcon (QString::fromStdString(is->get_icon()->get_href()));
            }
        }
        m_styles.insert(styleId, style);
     //   qDebug() << styleId << "fill:" << style.fill;
    }
}
