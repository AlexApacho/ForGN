#ifndef SOLVER_H
#define SOLVER_H

#include <QThread>
#include "managerdatabase.h"
#include <QTimer>
#include<QString>
#include <QDir>
#include<QtXml/QDomDocument>
#include <QTemporaryDir>

class Solver:public QThread
{
    Q_OBJECT
public:
    explicit Solver(QObject *parent = 0);
    void run();
    void PrepareToWriteDatabase();
    QStringList ParserXML(QString fileName,QString &tagName1);
    QStringList ParserXML(QString fileName,QString &tagName1,QString &tagName2);
    void ParserXML(QString fileName,QString &tagName1,QString &tagName2,QString &tagName3);
    void ParserXML(QString fileName,QString &tagName1,QString &tagName2,QString &tagName3,QString &tagName4);
    QStringList ParserXML(QString fileName,QString &tagName1,QString &tagName2,QString &tagName3,QString &tagName4,QString &tagName5);
    void ParserXML(QString fileName,QString& tagName1,QString& tagName2,QString& tagName3,QString& tagName4,QString& tagName5,QString& tagName6);
    void SetDirectoriesFile(QString &dir);
    QStringList GetDataForTableWidget();
    QStringList GetAllDataForTable();
    QString GetNameLastDocument();
    bool re_zlib(QString& FileName);
    void parseXML(const QDomElement& root, const QString& baseName, std::vector<QString>& v);
 private:
    ManagerDataBase *dataManager;
    QTimer *timer;
    QStringList dataForTW;
    QStringList dataForTable;
    QDir dir;
    QString strDirectories;
    QString nameDocumentLast;
    QTemporaryDir *tempDir;
public slots:
    void updateTime();
    void slotParser();
signals:
    void sgnlGetData(QStringList list);
    void sgnlGetNameLastDocument(QString str);
    void sgnlParser();

};
class ParserXmlDom{
public:
    void parse(QString &fileName);
    QDomElement getTag();

};

class SleeperThread : public QThread
{
public:
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};
#endif // SOLVER_H
