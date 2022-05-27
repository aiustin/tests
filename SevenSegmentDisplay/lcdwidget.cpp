#include "lcdwidget.h"

#include "sevensegmentdigitwidget.h"

#include <QLayout>
#include <QTime>

static const QString TimeFormat = QStringLiteral("HHmm");

LCDWidget::LCDWidget(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(h0Widget_ = new SevenSegmentDigitWidget());
    layout->addWidget(h1Widget_ = new SevenSegmentDigitWidget());
    layout->addWidget(m0Widget_ = new SevenSegmentDigitWidget());
    layout->addWidget(m1Widget_ = new SevenSegmentDigitWidget());
    layout->setSpacing(1);
    layout->setContentsMargins(1, 1, 1, 1);
    setLayout(layout);

    setAttribute(Qt::WA_NoSystemBackground);
}


void LCDWidget::setTime(const QTime& time)
{
    const auto formatted_time = time.toString(TimeFormat);
    Q_ASSERT(formatted_time.size() == 4);
    h0Widget_->setDigit(formatted_time.at(0).toLatin1() - '0');
    h1Widget_->setDigit(formatted_time.at(1).toLatin1() - '0');
    m0Widget_->setDigit(formatted_time.at(2).toLatin1() - '0');
    m1Widget_->setDigit(formatted_time.at(3).toLatin1() - '0');

    update();
}

