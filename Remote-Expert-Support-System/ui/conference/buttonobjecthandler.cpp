#include "buttonobjecthandler.h"

#include <QPainter>
// 在实现文件中添加
QSizeF ButtonObjectHandler::intrinsicSize(QTextDocument *doc, int posInDocument,
                                          const QTextFormat &format)
{
    Q_UNUSED(doc)
    Q_UNUSED(posInDocument)
    Q_UNUSED(format)
    return m_button->sizeHint();
}

void ButtonObjectHandler::drawObject(QPainter *painter, const QRectF &rect,
                                     QTextDocument *doc, int posInDocument,
                                     const QTextFormat &format)
{
    Q_UNUSED(doc)
    Q_UNUSED(posInDocument)
    Q_UNUSED(format)

    m_button->setGeometry(rect.toRect());
    painter->save();
    painter->translate(rect.topLeft());
    m_button->render(painter);
    painter->restore();
}
