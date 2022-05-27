#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class TimezonesModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddTimeZoneButtonClicked();
    void onTreeViewDoubleClicked(const QModelIndex& index);

private:
    void initialize();

private:
    Ui::MainWindow *ui;
    TimezonesModel* timeZonesModel_ = nullptr;
};
#endif // MAINWINDOW_H
