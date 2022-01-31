#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "solver.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSql>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlIndex>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlResult>
#include <QSqlTableModel>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Solver *solverToPrepare;
    QSqlTableModel *model;
public slots:
    void slotPushStart();
    void slotDateForWidget(QStringList list);
    void slotNameLastDocument(QString str);

private slots:
    void on_btnFindFile_clicked();
};
#endif // MAINWINDOW_H
