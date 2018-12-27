#include "linecollection.h"

using namespace LineDraw;

LineCollection::LineCollection() : m_mutex(QMutex::Recursive) {
}


void LineCollection::remove(const QString& name){
    PLOCK
    if(m_lines.contains(name)){
        auto line = m_lines.take(name);
        delete line;
        m_keys.removeOne(name);
        emit pathsChanged();
    }
}


QStringList LineCollection::paths() const{
    return m_lines.keys();
}



void LineCollection::clear(){
    PLOCK
    for(const auto& s : m_lines)
            s->clear();
    emit pathsChanged();
}


Line* LineCollection::append(const QString& pathName){
    PLOCK
    if(m_lines.contains(pathName))
        return nullptr;
    m_lines.insert(pathName, build());
    auto p = path(pathName);
    m_keys.append(pathName);
    emit pathsChanged();
    return p;
}


Line* LineCollection::path(const QString& pathName){
    if(!m_lines.contains(pathName))
        return nullptr;
    return m_lines[pathName];
}


void LineCollection::commit(){
    emit pathsChanged();
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
        emit colorChanged(color);
    }
}


void LineCollection::setThickness(qreal volume){
    if(m_volume != volume){
        m_volume = volume;
        emit thicknessChanged(volume);
    }
}


void LineCollection::setStyle(Line::Style style){
    if(m_style != style){
        m_style = style;
        emit styleChanged(style);
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
