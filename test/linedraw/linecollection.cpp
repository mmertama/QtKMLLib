#include "linecollection.h"
#include "lineitem.h"

using namespace LineDraw;


#define PLOCK QMutexLocker o_locker(&m_mutex);

LineCollection::LineCollection(LineInterface* lineItem) : m_lineItem{lineItem}, m_mutex{} {
}


void LineCollection::remove(const QString& name){
    PLOCK
    if(m_lines.contains(name)){
        m_lines.take(name);
        m_keys.removeOne(name);
        pathsChanged();
    }
}


QStringList LineCollection::paths() const{
    return m_lines.keys();
}



void LineCollection::clearLines(){
    PLOCK
    for(const auto& s : qAsConst(m_lines))
        s->clearLine();
    m_keys.clear();
    m_pathColor = DEFAULT_COLOR;
    m_style = Line::STYLE_SOLID;
    m_volume = DEFAULT_VOLUME;
    pathsChanged();
}


Line* LineCollection::append(const QString& pathName){
    PLOCK
    if(m_lines.contains(pathName))
        return nullptr;
    m_lines.insert(pathName, build());
    auto p = path(pathName);
    m_keys.append(pathName);
    pathsChanged();
    return p;
}


Line* LineCollection::path(const QString& pathName){
    if(!m_lines.contains(pathName))
        return nullptr;
    return m_lines[pathName];
}


void LineCollection::commitVertices() {
    pathsChanged();
}


QColor LineCollection::color() const{
    return m_pathColor;
}


qreal LineCollection::thickness() const {
    return m_volume;
}


Line::Style LineCollection::style() const{
    return m_style;
}


void LineCollection::setColor(const QColor& color){
    if(m_pathColor != color){
        m_pathColor = color;
        colorChanged(color);
    }
}


void LineCollection::setThickness(qreal volume){
    if(m_volume != volume){
        m_volume = volume;
        thicknessChanged(volume);
    }
}


void LineCollection::setStyle(Line::Style style){
    if(m_style != style){
        m_style = style;
        styleChanged(style);
    }
}


void LineCollection::bringOnTop(const QString& pathName){
    if(m_keys.last() != pathName){
       PLOCK
       const auto p = m_keys.indexOf(pathName);
       m_keys.append(m_keys.takeAt(p));
    }
}

const Line* LineCollection::path(const QString& pathName) const{
    return m_lines[pathName];
}


Line* LineCollection::build() {return m_lineItem->build();}
void LineCollection::pathsChanged() {m_lineItem->signalPathsChanged();}
void LineCollection::colorChanged(const QColor& pathColor) {m_lineItem->signalColorChanged(pathColor);}
void LineCollection::thicknessChanged(qreal thickness) {m_lineItem->signalThicknessChanged(thickness);}
void LineCollection::styleChanged(Line::Style style) {m_lineItem->signalStyleChanged(style);}
