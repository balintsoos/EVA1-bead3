#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("videorentdb");
    db.setUserName("root");
    db.setPassword("qweasd");

    if (db.open())
    {
        qDebug() << "lol...";
        db.close();

    }
    else
    {
        QMessageBox::critical(0, QObject::trUtf8("Database Error"), QObject::trUtf8("Connection error occured"));
    }

    return a.exec();
}
