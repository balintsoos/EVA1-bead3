#include <QApplication>
#include <QSqlDatabase>
#include <QMessageBox>
#include "editordialog.h"

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
        db.close();

        EditorDialog* editor = new EditorDialog();
        editor->show();
    }
    else
    {
        QMessageBox::critical(0, "Database Error", "Connection error occured");
    }

    return a.exec();
}
