#include <QSqlQuery>
#include <QMessageBox>

#include "userdialog.h"

UserDialog::UserDialog(QWidget *parent) : QDialog(parent)
{
    setupModel();
    setupUi();

    _movie = -1;
}

void UserDialog::setMovie(int movieId)
{
    if (movieId >= 0)
    {
        _movie = movieId;
    }
}

void UserDialog::selectButton_Clicked()
{
    QModelIndex index = _listView->currentIndex();

    if (_movie == -1)
    {
        QMessageBox::warning(this, "Invalid movie ID", "Invalid Movie ID");
        return;
    }

    if (index.isValid())
    {
        int member = _model->data(_model->index(index.row(), 0)).toInt();

        QSqlQuery query;

        query.prepare("insert into rent(member_id, movie_id, start_date, end_date) values(:member, :movie, NOW(), null)");

        query.bindValue(":member", member);
        query.bindValue(":movie", _movie);

        query.exec();
    }
    else
    {
        QMessageBox::warning(this, "No selection", "Select a member");
    }

    close();
}

void UserDialog::setupModel()
{
    _model = new QSqlTableModel(this);

    _model->setTable("member");
    _model->setSort(1, Qt::AscendingOrder);
    _model->select();
}

void UserDialog::setupUi()
{
    _selectButton = new QPushButton("Rent");

    connect(_selectButton, SIGNAL(clicked()), this, SLOT(selectButton_Clicked()));

    _buttonBox = new QDialogButtonBox(Qt::Vertical);
    _buttonBox->addButton(_selectButton, QDialogButtonBox::ActionRole);

    _listView = new QListView(this);
    _listView->setModel(_model);
    _listView->setModelColumn(1);

    _mainLayout = new QHBoxLayout;
    _mainLayout->addWidget(_listView);
    _mainLayout->addWidget(_buttonBox);

    setLayout(_mainLayout);

    setWindowTitle("Select Member");
    setFixedSize(300, 300);
}
