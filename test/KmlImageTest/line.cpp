#include "line.h"

#include <QColor>
#include <QPointF>
#include <QDebug>

#include "line_p.h"

using namespace LineDraw;

#define LOCK QMutexLocker o_locker(m_p->lock);

Line::Line(LinePrivate* p, QObject* parent) : QObject(parent), m_p(p){
    QObject::connect(this, &Line::colorChanged, this, &Line::lineChanged);
    QObject::connect(this, &Line::thicknessChanged, this, &Line::lineChanged);
    QObject::connect(this, &Line::styleChanged, this, &Line::lineChanged);
}

Line::Line(QObject* parent) : QObject(parent) {
}

Line::~Line(){
}

QColor Line::color() const {
    return m_p->color;
}

qreal Line::thickness() const {
    return m_p->volume;
}

Line::Style Line::style() const {
    return m_p->style;
}


int Line::len() const {
    return m_p->len;
}

void Line::setLen(int slen) {
    if(slen != m_p->len){
        m_p->len = slen;
        emit lenChanged(slen);
    }
}

int Line::offset() const {
    return m_p->offset;
}

void Line::setOffset(int soff) {
    if(soff != m_p->offset){
        m_p->offset = soff;
        emit offsetChanged(soff);
    }
}


void Line::clear(){
    m_p->vertices.clear();
    m_p->flags &= ~LinePrivate::VERTICES;
    emit lineChanged();
    emit verticesCountChanged(0);
}

int Line::verticesCount() const {
    return m_p->vertices.length();
}

void Line::setColor(const QColor& color){
    if(color != m_p->color){
        LOCK
        m_p->color = color;
        m_p->flags |= LinePrivate::MATERIAL;
        emit colorChanged(color);
    }
}

void Line::setThickness(qreal volume){
    if(volume != m_p->volume){
        LOCK
        m_p->volume = volume;
        m_p->flags |= LinePrivate::MATERIAL;
        emit thicknessChanged(volume);
    }
}

void Line::setStyle(Style style){
    if(style != m_p->style){
        LOCK
        m_p->style = style;
        m_p->flags |= LinePrivate::MATERIAL;
    }
}



void Line::set(const QVector<QPointF>& values){
    LOCK;
    if(values.length() > 0){
        m_p->flags |= LinePrivate::VISUAL;
    }
    else
        m_p->flags = 0;
    m_p->vertices.swap(const_cast<QVector<QPointF>&> (values));
    emit lineChanged();
    emit verticesCountChanged(m_p->vertices.length());
}


void Line::insert(int index, const QVector<QPointF>& values){
    if(values.length() > 0){
       LOCK
       auto start = qMin(values.length(), qMax(0, index));
       for(const auto& v : values){
           m_p->vertices.insert(start++, v);
       }
       m_p->flags |= LinePrivate::VISUAL;
       emit lineChanged();
       emit verticesCountChanged(m_p->vertices.length());
    }
}

void Line::append(const QVector<QPointF>& values){
    if(values.length() > 0){
       LOCK
       m_p->vertices.append(values);
       m_p->flags |= LinePrivate::VISUAL;
       emit lineChanged();
       emit verticesCountChanged(m_p->vertices.length());
    }
}

void Line::appendVertex(const QPointF& value){
    LOCK
 //   qDebug() << "add vertex" << value;
    m_p->vertices.append(value);
    m_p->flags |= LinePrivate::VISUAL;
}

void Line::commit(){
    emit lineChanged();
    emit verticesCountChanged(m_p->vertices.length());
}

Line::VertexPtr Line::vertices() const{
    return Line::VertexPtr(m_p->vertices.data(), m_p->lock);
}
