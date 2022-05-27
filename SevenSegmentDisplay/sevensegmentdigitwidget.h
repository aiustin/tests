#ifndef SEVENSEGMENTDIGITWIDGET_H
#define SEVENSEGMENTDIGITWIDGET_H

#include <QWidget>

class SevenSegmentDigitWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SevenSegmentDigitWidget(quint8 digit = 0, QWidget *parent = nullptr);

    virtual QSize sizeHint() const override;

    quint8 digit() const { return digit_; }
    void setDigit(quint8 d);

signals:

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    uint8_t digit_;
};

#endif // SEVENSEGMENTDIGITWIDGET_H
