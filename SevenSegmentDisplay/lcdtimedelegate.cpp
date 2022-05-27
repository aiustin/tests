#include "lcdtimedelegate.h"

#include "lcdwidget.h"

#include <QTime>
#include <QPainter>

LCDTimeDelegate::LCDTimeDelegate(QObject* parent /*= nullptr*/) :
    QStyledItemDelegate( parent )
,   widget_( new LCDWidget() )
{

}

LCDTimeDelegate::~LCDTimeDelegate()
{
    delete widget_;
}

void LCDTimeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.data().canConvert<QTime>()) {
        widget_->setTime(index.data().value<QTime>());

        painter->save();
        painter->translate(option.rect.topLeft());
        const QSize size = option.rect.size();
        widget_->resize(size);
        widget_->setFixedSize(size);
        widget_->render(painter);
        painter->restore();
    }
    else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize LCDTimeDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    return widget_->sizeHint();
}
