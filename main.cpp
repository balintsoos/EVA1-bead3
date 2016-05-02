#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>
#include <QMessageBox>

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
        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery("select * from movie");

        QTableView *view = new QTableView;
        view->setModel(model);
        view->setWindowTitle(QObject::trUtf8("Movies"));
        view->setMinimumSize(800,300);
        view->show();

        db.close();
    }
    else
    {
        QMessageBox::critical(0, QObject::trUtf8("Database Error"), QObject::trUtf8("Connection error occured"));
    }

    return a.exec();
}
