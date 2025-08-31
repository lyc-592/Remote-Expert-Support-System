#ifndef BUTTONOBJECTHANDLER_H
#define BUTTONOBJECTHANDLER_H

// 在头文件中添加
#include <QTextObjectInterface>
#include <QTextDocument>
#include <QPushButton>

class ButtonObjectHandler : public QObject, public QTextObjectInterface
{
    Q_OBJECT
    Q_INTERFACES(QTextObjectInterface)

public:
    explicit ButtonObjectHandler(QPushButton *button, QObject *parent = nullptr)
        : QObject(parent), m_button(button) {}

    QSizeF intrinsicSize(QTextDocument *doc, int posInDocument,
                         const QTextFormat &format) override;
    void drawObject(QPainter *painter, const QRectF &rect, QTextDocument *doc,
                    int posInDocument, const QTextFormat &format) override;

private:
    QPushButton *m_button;
};

#endif // BUTTONOBJECTHANDLER_H
