#ifndef MANAGERDATABASE_H
#define MANAGERDATABASE_H

#include <QThread>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSql>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlIndex>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlResult>
class ManagerDataBase :public QThread
{
    Q_OBJECT
public:
    explicit ManagerDataBase(QObject *parent = 0);
    void SetName(QString &name);
    void Init();
    void SetData(QStringList &strList);
    QStringList GetCurentData();
    QStringList GetAllData();
private:
    QString strings;
    QSqlDatabase dataBase ;
    QStringList dataListForTW;
};

#endif // MANAGERDATABASE_H
