#ifndef LCDTIMEDELEGATE_H
#define LCDTIMEDELEGATE_H

#include <QStyledItemDelegate>

class LCDWidget;

class LCDTimeDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    LCDTimeDelegate(QObject* parent = nullptr);
    virtual ~LCDTimeDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index ) const;

private:
    LCDWidget* widget_ = nullptr;
};

#endif // LCDTIMEDELEGATE_H
