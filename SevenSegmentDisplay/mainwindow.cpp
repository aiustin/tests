#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "timezonesmodel.h"
#include "lcdtimedelegate.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timeZonesModel_(new TimezonesModel(this))
{
    ui->setupUi(this);

    initialize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialize()
{
    for(const auto& tz : TimeZone::allAvailableTimeZones()) {
        ui->timezonesCB->addItem(tz.name(), QVariant::fromValue<TimeZone>(tz));
    }

    ui->clocksTableView->setStyleSheet("QHeaderView { background-color:white }  QHeaderView::section { background-color:white; border-width: 1; }");
    ui->clocksTableView->horizontalHeader()->setStretchLastSection(false);
    ui->clocksTableView->setItemDelegateForColumn(1, new LCDTimeDelegate(this));
    ui->clocksTableView->setModel(timeZonesModel_);

    if (!ui->timezonesCB->count()) {
        ui->addTimezoneButton->setEnabled(false);
    }

    connect(ui->addTimezoneButton, &QPushButton::clicked, this, &MainWindow::onAddTimeZoneButtonClicked);
    connect(ui->clocksTableView, &QTableView::doubleClicked, this, &MainWindow::onTreeViewDoubleClicked);
}


void MainWindow::onAddTimeZoneButtonClicked()
{
    Q_ASSERT(timeZonesModel_);
    if (ui->timezonesCB->currentIndex() < 0) {
        return;
    }

    timeZonesModel_->addTimeZone(ui->timezonesCB->itemData(ui->timezonesCB->currentIndex()).value<TimeZone>());
}

void MainWindow::onTreeViewDoubleClicked(const QModelIndex& index)
{
    Q_ASSERT(timeZonesModel_);
    timeZonesModel_->removeTimeZone( index );
}
