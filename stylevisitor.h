#ifndef STYLEVISITOR_H
#define STYLEVISITOR_H


#include <QString>
#include <QHash>
#include <QColor>
#include "qtkml.h"
#include "kml/dom.h"
#include "kml/engine.h"

namespace QtKml{


/*
class StyleParams{
public:
    static constexpr qreal DEFAULT_LINE_WIDTH = 1.0;
    static constexpr quint32 DEFAULT_LINE_COLOR = 0xFF000000;
    static constexpr quint32 DEFAULT_FILL_COLOR = 0xFFFFFFFF;
    static constexpr bool DEFAULT_FILL = false;
    static constexpr char DEFAULT_ICON[] = "";
    static inline QColor fromARGB(quint32 color){
        return QColor(
                    (0xFF & (color >> 16)),
                    (0xFF & (color >> 8)),
                    (0xFF & color),
                    (0xFF & (color >> 24)));
    }
public:
    qreal lineWidth = DEFAULT_LINE_WIDTH;
    QColor lineColor = fromARGB(DEFAULT_LINE_COLOR);
    QColor fillColor = fromARGB(DEFAULT_FILL_COLOR);
    bool fill = DEFAULT_FILL;
    QString icon = DEFAULT_ICON;
};
*/

class StyleParams : public QVariantMap{
public:
    virtual ~StyleParams() = default;
    QString icon() const {return value(KmlElement::ICON , "").toString();}
    QColor lineColor() const {return value(KmlElement::LINE_COLOR, 0xFF000000).value<QColor>();}
    QColor fillColor() const {return value(KmlElement::FILL_COLOR, 0xFFFFFFFF).value<QColor>();}
    qreal lineWidth() const {return value(KmlElement::LINE_WIDTH, 1.0).toFloat();}
    bool isFill() const {return value(KmlElement::FILL, false).toBool();}
    void setIcon(const QString& name) {insert(KmlElement::ICON, name);}
    void setLineColor(const QColor& color) {insert(KmlElement::LINE_COLOR, QVariant::fromValue(color));}
    void setFillColor(const QColor& color) {insert(KmlElement::FILL_COLOR, QVariant::fromValue(color));}
    void setLineWidth(qreal width) {insert(KmlElement::LINE_WIDTH, width);}
    void setIsFill(bool fill) {insert(KmlElement::FILL, fill);}
};

class StyleVisitor : public kmldom::Visitor{
public:
    class StyleList : public QHash<QString, StyleParams>{
    public:
        StyleList(){insert("", StyleParams());}
        virtual ~StyleList() = default;
        const StyleParams& get(const QString& key) const {return (!key.isNull() && contains(key)) ? find(key).value() : find("").value();}
    };
    StyleVisitor(StyleList& styles);
    virtual ~StyleVisitor();
    QString currentStyleId(const kmldom::ElementPtr& element) const;
protected:
    virtual void VisitStyle(const kmldom::StylePtr &element);
private:
    StyleList* m_styles;
    QString m_currentStyle;
};

}

#endif // STYLEVISITOR_H
