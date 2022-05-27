#ifndef TIMEZONESMODEL_H
#define TIMEZONESMODEL_H

#include <QAbstractListModel>

#include "timezone.h"
#include "clock.h"

class TimezonesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TimezonesModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addTimeZone(TimeZone&& timeZone);
    void removeTimeZone(const QModelIndex& index);


private slots:
    void onTimeChanged();

private:
    TimeZones timeZones_;
    Clock* clock_ = nullptr;
};

#endif // TIMEZONESMODEL_H
