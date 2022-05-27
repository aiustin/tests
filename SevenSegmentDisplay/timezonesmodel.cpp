#include "timezonesmodel.h"

static const int ColumnCount = 2;


TimezonesModel::TimezonesModel(QObject *parent) :
    QAbstractListModel(parent)
,   clock_(new Clock(Clock::Resolution::Minute, this))
{
    connect(clock_, &Clock::timeChanged, this, &TimezonesModel::onTimeChanged);
}

QVariant TimezonesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section >= 0 && section < ColumnCount)
    {
        return !section ? tr("Time zone") : tr("Current time");
    }
    else if (role == Qt::BackgroundRole) {
        return QVariant::fromValue(Qt::red);
    }
    return QVariant();
}

int TimezonesModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : timeZones_.size();
}

int TimezonesModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : ColumnCount;
}

QVariant TimezonesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch(role)
    {
    case Qt::DisplayRole:
    {
        const auto& timeZone = timeZones_[index.row()];
        return index.column() ? QVariant::fromValue<QTime>(timeZone.currentDateTime().time()):
                                QVariant::fromValue<QString>(timeZone.name());
    }
    default:
        break;
    }

    return QVariant();
}

void TimezonesModel::addTimeZone(TimeZone&& timeZone)
{
    beginInsertRows(QModelIndex(), timeZones_.size(), timeZones_.size());
    timeZones_.push_back(std::move(timeZone));
    endInsertRows();
}

void TimezonesModel::removeTimeZone(const QModelIndex &index)
{
    if ( ! index.isValid( ) ) {
        return;
    }
    Q_ASSERT(index.row() < timeZones_.size());
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    timeZones_.erase(timeZones_.begin() + index.row());
    endRemoveRows();
}

void TimezonesModel::onTimeChanged()
{
    if (!timeZones_.empty()) {
        emit dataChanged(index(0, 1), index(timeZones_.size()-1, 1));
    }
}
