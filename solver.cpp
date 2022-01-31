#include "solver.h"
#include <QDebug>
#include <QFile>
#include <QtZlib/zlib.h>
#include <QtCore>
#include <QDataStream>
#include <QByteArray>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
Solver::Solver(QObject *parent) :
    QThread(parent)
  ,dataManager(new ManagerDataBase())
  ,timer(new QTimer(this))

{
    dataManager->Init();
    timer->setInterval(1000);
    //connect(timer,SIGNAL(timeout()),this,SLOT(updateTime()));
    connect(this,SIGNAL(sgnlParser()), this,SLOT(slotParser()));
}
enum class TypeTag {
    purchaseObjectInfo, url
};



void Solver::run(){
    //timer->start();
    tempDir = new QTemporaryDir("tmp");
    updateTime();
}



void Solver::PrepareToWriteDatabase()
{
    QStringList strListData;

    for(int i = 0; i<dir.entryList().size();i++){
        strListData.clear();
        dataForTW.clear();
        for(int i = 0;i<15;i++){
            strListData<<"Null";
        }
        QDomDocument document;
        QString fileName("1/"+dir.entryList().at(i));

        nameDocumentLast = dir.entryList().at(i);
        emit sgnlGetNameLastDocument(nameDocumentLast);

        qDebug()<<"Name file = "<<fileName;
        QFile xmlFile(fileName);
        if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly)) {
            qDebug() << "Check your file"<<dir.entryList().at(i);
            //return;
        }
        QDomDocument domDocument;
        domDocument.setContent(&xmlFile);
        QDomElement topElement = domDocument.documentElement();
        QDomNode domNode = topElement.firstChild();
        while (!domNode.isNull()) {
            QDomElement domElement = domNode.toElement();
            if (!domElement.isNull()) {
                qDebug()<<"Name tag = "<<domElement.tagName();
                if (domElement.tagName() == domElement.tagName()) {
                    QDomElement node = domElement.firstChildElement();
                    while (!node.isNull()) {


                        if(node.nodeName() == "ns9:versionNumber"){
                            qDebug()<<"versionNumber:"<<node.text();
                            strListData.insert(0,node.text());
                        }
                        if(node.nodeName() == "ns9:commonInfo" ){
                            QDomElement element = node.firstChildElement();
                            while(!element.isNull()){
                                const QString tagName(element.tagName());

                                if (tagName == "ns9:purchaseNumber" || tagName == "purchaseNumber" ) {
                                    qDebug() << "purchaseNumber:" << element.text();
                                    strListData.insert(1,element.text());
                                } else if (tagName == "ns9:purchaseObjectInfo") {
                                    qDebug() << "purchaseObjectInfo:" << element.text();
                                    strListData.insert(2,element.text());
                                }
                                if(element.tagName() == "ns9:placingWay"){
                                    QDomElement node3 = element.firstChildElement();
                                    while(!node3.isNull()){

                                        if(node3.nodeName() == "ns4:code"){
                                            qDebug()<<"Code = "<<node3.text();

                                        }
                                        //qDebug() << "ELEEEEEMENT is:" << node3.nodeName();
                                        node3 = node3.nextSiblingElement();
                                    }
                                }
                                //qDebug() << "Href is:" << element.tagName();
                                element = element.nextSiblingElement();
                            }
                        }
                        if(node.nodeName() == "ns9:purchaseResponsibleInfo"){
                            QDomElement element = node.firstChildElement();
                            while(!element.isNull()){
                                const QString tagName(element.tagName());

                                if(element.tagName() == "ns9:responsibleInfo"){
                                    QDomElement elementInside = element.firstChildElement();
                                    while(!elementInside.isNull()){
                                        if(elementInside.tagName() == "ns9:contactPersonInfo"){
                                            QDomElement elementInsIns = elementInside.firstChildElement();
                                            while(!elementInsIns.isNull()){
                                                if(elementInsIns.tagName() == "ns3:lastName"){
                                                    qDebug()<<"LastName = "<<elementInsIns.text();
                                                    strListData.insert(3,elementInsIns.text());
                                                }else if(elementInsIns.tagName() == "ns3:firstName"){
                                                    qDebug()<<"FirstName = "<<elementInsIns.text();
                                                    strListData.insert(4,elementInsIns.text());
                                                }else if(elementInsIns.tagName() == "ns3:middleName"){
                                                    qDebug()<<"MiddleName = "<<elementInsIns.text();
                                                    strListData.insert(5,elementInsIns.text());
                                                }
                                                elementInsIns = elementInsIns.nextSiblingElement();
                                            }
                                        }
                                        //qDebug() << "Href ========= " << elementInside.tagName();
                                        elementInside = elementInside.nextSiblingElement();
                                    }
                                }
                                element = element.nextSiblingElement();
                            }

                        }
                        if(node.nodeName() == "ns9:notificationInfo"){
                            QDomElement element = node.firstChildElement();
                            while(!element.isNull()){
                                if(element.tagName() == "ns9:procedureInfo"){
                                    QDomElement elementIns = element.firstChildElement();
                                    while(!elementIns.isNull()){
                                        if(elementIns.tagName() == "ns9:collectingInfo")
                                        {
                                            QDomElement elementInsIns = elementIns.firstChildElement();
                                            while(!elementInsIns.isNull()){
                                                if(elementInsIns.tagName() == "ns9:startDT"){
                                                    qDebug()<<"startDT = "<<elementInsIns.text();
                                                    strListData.insert(6,elementInsIns.text());
                                                }else if(elementInsIns.tagName() == "ns9:endDT"){
                                                    strListData.insert(7,elementInsIns.text());
                                                    qDebug()<<"endDT = "<<elementInsIns.text();
                                                }else if(elementInsIns.tagName() == "ns9:place"){
                                                    strListData.insert(8,elementInsIns.text());
                                                    qDebug()<<"place = "<<elementInsIns.text();
                                                }
                                                elementInsIns = elementInsIns.nextSiblingElement();
                                            }
                                        }
                                        //qDebug()<<"Taaaaaag = "<<elementIns.tagName();
                                        elementIns = elementIns.nextSiblingElement();

                                    }
                                }
                                //qDebug()<<"Taaaaaag = "<<element.tagName();
                                element = element.nextSiblingElement();
                            }

                        }

                        //qDebug() << "Node = "<<node.nodeName();
                        node = node.nextSiblingElement();
                    }
                }
            }
            domNode = domNode.nextSibling();
        }

        xmlFile.close();

        dataManager->SetData(strListData);

        emit sgnlGetData(dataManager->GetAllData());
    }

}

QStringList Solver::ParserXML(QString fileName,QString& tagName1)
{
    QFile xmlFile(fileName);
    QStringList strTagValue;
    QDomDocument domDocument;
    domDocument.setContent(&xmlFile);
    QDomElement topElement = domDocument.documentElement();
    QDomNode domNode = topElement.firstChild();

    qDebug()<<"TAG"<<domNode.nodeName();

    while (!domNode.isNull()) {
        QDomElement domElement = domNode.toElement();
        QString str123;
        str123 = domNode.attributes().item(0).nodeValue();
        qDebug()<<"Tags = "<<domNode.nodeName()<<domNode.attributes().item(0).nodeName()<<domNode.attributes().item(0).nodeValue();

        qDebug()<<"Tagname234"<<tagName1;

        QDomElement node = domElement.firstChildElement();
        while (!node.isNull()) {
            if(tagName1 == node.tagName()){
                qDebug()<<node.tagName()<<node.text();
                strTagValue<<node.text();
            }
            //qDebug()<<"Node"<<node.tagName();
            QDomElement element = node.firstChildElement();
            while (!element.isNull()) {
                //qDebug()<<"Element = "<<element.tagName();
                if(element.isNull()){
                    qDebug()<<"Element = "<<element.tagName();

                }
                element = element.nextSiblingElement();
            }
            node = node.nextSiblingElement();
        }
        domNode = domNode.nextSibling();
    }
    xmlFile.close();
    if(strTagValue.isEmpty())
        strTagValue<<"NULL";
    return strTagValue;

}
QStringList Solver::ParserXML(QString fileName,QString& tagName1,QString& tagName2)
{
    QFile xmlFile(fileName);
    QStringList strTagValue;
    QDomDocument domDocument;
    domDocument.setContent(&xmlFile);
    QDomElement topElement = domDocument.documentElement();
    QDomNode domNode = topElement.firstChild();
    while (!domNode.isNull()) {
        QDomElement domElement = domNode.toElement();
        //qDebug()<<"Tags = "<<domNode.nodeName();
        QDomElement node = domElement.firstChildElement();
        while (!node.isNull()) {
            //qDebug()<<node.tagName()<<node.text();

            if(tagName1 == node.tagName()){
                //qDebug()<<node.tagName()<<node.text();
                QDomElement element = node.firstChildElement();
                while (!element.isNull()) {
                    if(tagName2 == element.tagName()){
                        qDebug()<<element.tagName()<<element.text();
                        strTagValue<<element.text();
                    }
                    element = element.nextSiblingElement();
                }
            }
            node = node.nextSiblingElement();
        }
        domNode = domNode.nextSibling();
    }
    xmlFile.close();
    if(strTagValue.isEmpty())
        strTagValue<<"NULL";
    return strTagValue;
}
void Solver::ParserXML(QString fileName,QString& tagName1,QString& tagName2,QString& tagName3)
{
    QFile xmlFile(fileName);
    QDomDocument domDocument;
    domDocument.setContent(&xmlFile);
    QDomElement topElement = domDocument.documentElement();
    QDomNode domNode = topElement.firstChild();
    while (!domNode.isNull()) {
        QDomElement domElement = domNode.toElement();
        //qDebug()<<"Tags = "<<domNode.nodeName();
        QDomElement node = domElement.firstChildElement();
        while (!node.isNull()) {
            //qDebug()<<node.tagName()<<node.text();
            if(tagName1 == node.tagName()){
                //qDebug()<<node.tagName()<<node.text();
                QDomElement element = node.firstChildElement();
                while (!element.isNull()) {
                    if(tagName2 == element.tagName()){
                        QDomElement elementIns = element.firstChildElement();
                        while (!elementIns.isNull()) {
                            if(tagName3 == elementIns.tagName())
                                qDebug()<<elementIns.tagName()<<elementIns.text();
                            elementIns = elementIns.nextSiblingElement();
                        }
                    }
                    element = element.nextSiblingElement();
                }
            }
            node = node.nextSiblingElement();
        }
        domNode = domNode.nextSibling();
    }
    xmlFile.close();
}
void Solver::ParserXML(QString fileName,QString& tagName1,QString& tagName2,QString& tagName3,QString& tagName4)
{
    QFile xmlFile(fileName);
    QDomDocument domDocument;
    domDocument.setContent(&xmlFile);
    QDomElement topElement = domDocument.documentElement();
    QDomNode domNode = topElement.firstChild();
    while (!domNode.isNull()) {
        QDomElement domElement = domNode.toElement();
        //qDebug()<<"Tags = "<<domNode.nodeName();
        QDomElement node = domElement.firstChildElement();
        while (!node.isNull()) {
            //qDebug()<<node.tagName()<<node.text();
            if(tagName1 == node.tagName()){
                //qDebug()<<node.tagName()<<node.text();
                QDomElement element = node.firstChildElement();
                while (!element.isNull()) {
                    if(tagName2 == element.tagName()){
                        QDomElement elementIns = element.firstChildElement();
                        while (!elementIns.isNull()) {
                            if(tagName3 == elementIns.tagName()){
                                QDomElement elementInsIns = elementIns.firstChildElement();
                                while (!elementInsIns.isNull()) {
                                    if(tagName4 == elementInsIns.tagName()){
                                        qDebug()<<elementInsIns.tagName()<<elementInsIns.text();
                                    }
                                    elementInsIns = elementInsIns.nextSiblingElement();
                                }
                            }

                            elementIns = elementIns.nextSiblingElement();
                        }
                    }
                    element = element.nextSiblingElement();
                }
            }
            node = node.nextSiblingElement();
        }
        domNode = domNode.nextSibling();
    }
    xmlFile.close();
}
QStringList Solver::ParserXML(QString fileName,QString& tagName1,QString& tagName2,QString& tagName3,QString& tagName4,QString& tagName5)
{
    QFile xmlFile(fileName);
    QDomDocument domDocument;
    QStringList strTagValue;
    domDocument.setContent(&xmlFile);
    QDomElement topElement = domDocument.documentElement();
    QDomNode domNode = topElement.firstChild();
    while (!domNode.isNull()) {
        QDomElement domElement = domNode.toElement();
        //qDebug()<<"Tags = "<<domNode.nodeName();
        QDomElement node = domElement.firstChildElement();
        while (!node.isNull()) {
            //qDebug()<<node.tagName()<<node.text();
            if(tagName1 == node.tagName()){
                //qDebug()<<node.tagName()<<node.text();
                QDomElement element = node.firstChildElement();
                while (!element.isNull()) {
                    if(tagName2 == element.tagName()){
                        QDomElement elementIns = element.firstChildElement();
                        while (!elementIns.isNull()) {
                            if(tagName3 == elementIns.tagName()){
                                QDomElement elementInsIns = elementIns.firstChildElement();
                                while (!elementInsIns.isNull()) {
                                    if(tagName4 == elementInsIns.tagName()){
                                        QDomElement elementInsInsIns = elementInsIns.firstChildElement();
                                        while (!elementInsInsIns.isNull()) {
                                            if(tagName5 == elementInsInsIns.tagName()){
                                                qDebug()<<elementInsInsIns.tagName()<<elementInsInsIns.text();
                                                strTagValue<<elementInsInsIns.text();
                                            }
                                            elementInsInsIns = elementInsInsIns.nextSiblingElement();
                                        }
                                    }
                                    elementInsIns = elementInsIns.nextSiblingElement();
                                }
                            }

                            elementIns = elementIns.nextSiblingElement();
                        }
                    }
                    element = element.nextSiblingElement();
                }
            }
            node = node.nextSiblingElement();
        }
        domNode = domNode.nextSibling();
    }
    xmlFile.close();
    if(strTagValue.isEmpty())
        strTagValue<<"NULL";
    return strTagValue;
}
void Solver::ParserXML(QString fileName,QString& tagName1,QString& tagName2,QString& tagName3,QString& tagName4,QString& tagName5,QString& tagName6)
{
    QFile xmlFile(fileName);
    QDomDocument domDocument;
    QStringList strTagValue;
    domDocument.setContent(&xmlFile);
    QDomElement topElement = domDocument.documentElement();
    QDomNode domNode = topElement.firstChild();
    while (!domNode.isNull()) {
        QDomElement domElement = domNode.toElement();
        //qDebug()<<"Tags = "<<domNode.nodeName();
        QDomElement node = domElement.firstChildElement();
        while (!node.isNull()) {
            //qDebug()<<node.tagName()<<node.text();
            if(tagName1 == node.tagName()){
                //qDebug()<<node.tagName()<<node.text();
                QDomElement element = node.firstChildElement();
                while (!element.isNull()) {
                    if(tagName2 == element.tagName()){
                        QDomElement elementIns = element.firstChildElement();
                        while (!elementIns.isNull()) {
                            if(tagName3 == elementIns.tagName()){
                                QDomElement elementInsIns = elementIns.firstChildElement();
                                while (!elementInsIns.isNull()) {
                                    if(tagName4 == elementInsIns.tagName()){
                                        QDomElement elementInsInsIns = elementInsIns.firstChildElement();
                                        while (!elementInsInsIns.isNull()) {
                                            if(tagName5 == elementInsInsIns.tagName()){
                                                QDomElement elementInsInsInsIns = elementInsInsIns.firstChildElement();
                                                while (!elementInsInsInsIns.isNull()) {
                                                    if(tagName6 == elementInsInsInsIns.tagName()){
                                                        qDebug()<<elementInsInsInsIns.tagName()<<elementInsInsInsIns.text();
                                                        strTagValue<<elementInsInsInsIns.text();
                                                    }
                                                    elementInsInsInsIns = elementInsInsInsIns.nextSiblingElement();
                                                }
                                            }
                                            elementInsInsIns = elementInsInsIns.nextSiblingElement();
                                        }
                                    }
                                    elementInsIns = elementInsIns.nextSiblingElement();
                                }
                            }

                            elementIns = elementIns.nextSiblingElement();
                        }
                    }
                    element = element.nextSiblingElement();
                }
            }
            node = node.nextSiblingElement();
        }
        domNode = domNode.nextSibling();
    }
    xmlFile.close();


}


void Solver::SetDirectoriesFile(QString &dir)
{
    strDirectories = dir;
    return;
}

QStringList Solver::GetDataForTableWidget()
{
    return dataManager->GetCurentData();
}

QStringList Solver::GetAllDataForTable()
{
    return dataManager->GetAllData();
}

QString Solver::GetNameLastDocument()
{
    return nameDocumentLast;
}

void Solver::updateTime()
{
    dir.cd(strDirectories);
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Time | QDir::Reversed);
    QStringList dirArchive = dir.entryList(QStringList() << "*.zip", QDir::Files);
    qDebug()<<dirArchive.size()<<dir.entryList().size();
    for(int j = 0;j<dirArchive.size();j++){
        QProcess *processUnZip = new QProcess(this);
        QString pathToTemp,commPath;
        commPath = "-o";
        pathToTemp = tempDir->path();
        processUnZip->start("7z.exe",QStringList()<<"x"<<strDirectories+"/"+dirArchive.at(j)<<commPath+pathToTemp);
        qDebug()<<"PROCESS END = "<<processUnZip->atEnd();
    }
    SleeperThread::msleep(2000);
    emit sgnlParser();
    //timer->stop();
}

void Solver::slotParser()
{

    QDir dirCurrent;
    dirCurrent.setFilter(QDir::Files);
    //dirCurrent.setSorting(QDir::Time | QDir::Reversed);

    QString str1 = "purchaseObjectInfo";

    QString str3 = "printForm";
    QString str4 = "url";

    QString str5 = "purchaseResponsible";
    QString str6 = "responsibleOrg";
    QString str7 = "regNum";

    QString str8 = "purchaseResponsible";
    QString str9 = "responsibleInfo";
    QString str10 = "contactPerson";
    QString str11 = "lastName";

    QString str12 = "lot";
    QString str13 = "purchaseObjects";
    QString str14 = "purchaseObject";
    QString str15 = "OKPD";
    QString str16 = "code";

    dirCurrent.cd(tempDir->path());

    qDebug()<<"FILE PATH = "<<tempDir->path()<<"SIZE = "<<dirCurrent.entryList().size();

    if (tempDir->isValid()) {
        for(int k = 0;k<dirCurrent.entryList().size();k++){


            dataForTable.clear();

            QString fileName(dirCurrent.filePath(dirCurrent.entryList().at(k)));
            QString typeFile = dirCurrent.entryList().at(k).left(dirCurrent.entryList().at(k).indexOf('_'));

             qDebug()<<"TyPE File = "<< typeFile;
            if(typeFile == "contractProcedure")
                continue;
            //qDebug()<<"FILE PATH = "<<dirCurrent.filePath(dirCurrent.entryList().at(k));

            qDebug()<<"Parser1:" <<fileName<<ParserXML(fileName,str1);
            dataForTable<<ParserXML(fileName,str1);
            dataForTable<<ParserXML(fileName,str3,str4);
            dataForTable<<ParserXML(fileName,str12,str13,str14,str15,str16);
//            qDebug()<<"Parser2:"<<fileName;
//            ParserXML(fileName,str3,str4);
//            qDebug()<<"Parser3:"<<fileName;
//            ParserXML(fileName,str5,str6,str7);
//            qDebug()<<"Parser4:"<<fileName;
//            ParserXML(fileName,str8,str9,str10,str11);
//            qDebug()<<"Parser5:"<<fileName;

            //qDebug()<<"asdasd = "<<dataForTable.size();

            for(int i = 0;i<dataForTable.size();i++){
                if(dataForTable.at(i).isEmpty())
                    dataForTable<<"Null";
            }
            dataManager->SetData(dataForTable);
        }
        qDebug()<<"ReMOVE"<<tempDir->autoRemove();

    }
    tempDir->~QTemporaryDir();


}

