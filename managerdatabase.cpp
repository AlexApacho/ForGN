#include "managerdatabase.h"

#include <QMessageBox>
#include <QFile>
#include <QDate>
#include <QList>
#include <QDebug>
#include <QMessageBox>
ManagerDataBase::ManagerDataBase(QObject *parent):
    QThread(parent)
  ,dataBase( QSqlDatabase::addDatabase( "QSQLITE" ))
{

}

void ManagerDataBase::SetName(QString &name)
{

    qDebug()<<"name"<<name;

}

void ManagerDataBase::Init()
{
    dataBase.setDatabaseName( "BaseForFNS.db" );
    if( !dataBase.open() )
        qDebug() <<"cannot open database!!!" <<dataBase.lastError().text();
    else
        qDebug() <<"open database!!!";
    QSqlQuery query(dataBase);
    QString   str  = "CREATE TABLE ParserXML ( "
            "id INTEGER PRIMARY KEY NOT NULL, "
            "versionNumber   INTEGER, "
            "purchaseNumber   INTEGER, "
            "purchaseObjectInfo    VARCHAR(255), "
            "lastName VARCHAR(255),"
            "firstName   VARCHAR(255),"
            "middleName   VARCHAR(255),"
            "startDT   VARCHAR(255),"
            "endDT   VARCHAR(255),"
            "place VARCHAR(255)"
            ");"
            ;
    query.exec(str);
}

void ManagerDataBase::SetData(QStringList &strList)
{
    QSqlQuery query(dataBase);
    QVariant nums;
    query.exec("SELECT COUNT(*) FROM ParserXML;");

    if ( ! query.isActive() )
    {

    }else
    {
        while ( query.next() )
        {
            nums    = query.value(0).toInt() + (int)1;
        }
    }
    QString numRecord;
    numRecord = nums.toString();

    QString addRecordCalc =
            "INSERT INTO  ParserXML (id, versionNumber, purchaseNumber, purchaseObjectInfo, lastName, firstName, middleName, startDT,endDT, place) "
        "VALUES('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10');";

    QString str = addRecordCalc.arg(numRecord)
            .arg(strList.at(0))
            .arg(strList.at(1))
            .arg(strList.at(2));
            //.arg(strList.at(3))
            //.arg(strList.at(4))
            //.arg(strList.at(5))
            //.arg(strList.at(6))
            //.arg(strList.at(7))
            //.arg(strList.at(8));
    query.exec(str);
    query.finish();


}

QStringList ManagerDataBase::GetCurentData()
{
    return QStringList() ;
}
QStringList ManagerDataBase::GetAllData()
{
return QStringList() ;
}
