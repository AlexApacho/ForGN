#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>
#include <QSqlTableModel>
#include <QFileDialog>
#include <QBuffer>
#include <QtZlib/zlib.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , solverToPrepare(new Solver)
{
    ui->setupUi(this);
    connect(ui->btnStart,SIGNAL(clicked()),this,SLOT(slotPushStart()));
    connect(solverToPrepare,SIGNAL(sgnlGetData(QStringList)),this,SLOT(slotDateForWidget(QStringList)));
    connect(solverToPrepare,SIGNAL(sgnlGetNameLastDocument(QString)),this,SLOT(slotNameLastDocument(QString)));
    slotDateForWidget(QStringList());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotPushStart()
{

    solverToPrepare->run();
}

void MainWindow::slotDateForWidget(QStringList list)
{
    QSqlDatabase dataBase =  QSqlDatabase::addDatabase( "QSQLITE" );
    dataBase.setDatabaseName( "BaseForFNS.db" );
    if( !dataBase.open() )
        qDebug() <<"cannot open database!!!" <<dataBase.lastError().text();
    else
        qDebug() <<"open database!!!";
    QSqlTableModel *model = new QSqlTableModel(0, dataBase);
    model->setTable("ParserXML");          // Имя таблицы базы данных.
    model->select();
    ui->tableView->setModel(model);     // Устанавливаем модель на TableView
    ui->tableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
    // Разрешаем выделение строк
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    model->select(); // Делаем выборку данных из таблицы

}

void MainWindow::slotNameLastDocument(QString str)
{
    ui->label_6->clear();
    ui->label_6->setText(str);
}


void MainWindow::on_btnFindFile_clicked()
{
    ui->editDirFile->clear();
    QString str = QFileDialog::getExistingDirectory(0, "Выбор папки", "");
    ui->editDirFile->setText(str);
    solverToPrepare->SetDirectoriesFile(str);

}
