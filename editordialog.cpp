#include <QMessageBox>
#include <QSqlError>
#include <QSqlRelationalDelegate>

#include "moviedelegate.h"
#include "editordialog.h"

EditorDialog::EditorDialog(QWidget *parent) : QDialog(parent)
{
    setupModel();
    setupUi();
}

EditorDialog::~EditorDialog()
{
    delete _model;
}

void EditorDialog::addButton_Clicked()
{
    int row;

    if (_tableView->currentIndex().isValid())
    {
        row = _tableView->currentIndex().row();
    }
    else
    {
        row = _model->rowCount();
    }

    _model->insertRow(row);

    // Default values
    _model->setData(_model->index(row, 1), "untitled");
    _model->setData(_model->index(row, 6), 1);

    QModelIndex newIndex = _model->index(row, 1);
    _tableView->setCurrentIndex(newIndex);
    _tableView->edit(newIndex);
}

void EditorDialog::removeButton_Clicked()
{
    QModelIndex index = _tableView->currentIndex();

    if (index.isValid())
    {
        QModelIndex availableCellIndex = _model->index(index.row(), 7);

        if (_model->data(availableCellIndex) == 0)
        {
            _model->removeRow(index.row());
        }
        else
        {
            QMessageBox::warning(this, "Movie rented", "Selected movie is rented. You can't delete this item.");
        }
    }
    else
    {
        QMessageBox::warning(this, "No selection", "Select a row before clicking on delete");
    }
}

void EditorDialog::rentButton_Clicked()
{
    QModelIndex index = _tableView->currentIndex();

    if (index.isValid())
    {
        QModelIndex statusIndex = _model->index(index.row(), 6);
        QModelIndex availableIndex = _model->index(index.row(), 7);


        if (_model->data(statusIndex) == 0)
        {
            QMessageBox::warning(this, "Movie locked", "Selected movie is locked. You can't rent this item.");
            return;
        }

        if (_model->data(availableIndex) == 1)
        {
            QMessageBox::warning(this, "Movie rented", "Selected movie is rented. You can't rent this item.");
            return;
        }

        QModelIndex movieIdIndex = _model->index(index.row(), 0);
        int movieId = _model->data(movieIdIndex).toInt();

        _userDialog->setMovie(movieId);
        _userDialog->show();
    }
    else
    {
        QMessageBox::warning(this, "No selection", "Select a row before clicking on rent");
    }
}

void EditorDialog::returnButton_Clicked()
{
    QModelIndex index = _tableView->currentIndex();

    if (index.isValid())
    {
        QModelIndex availableIndex = _model->index(index.row(), 7);

        if (_model->data(availableIndex) == 0)
        {
            QMessageBox::warning(this, "Movie not rented", "Selected movie is not rented. You can't return this item.");
            return;
        }

        int movie = _model->data(_model->index(index.row(), 0)).toInt();
        returnMovie(movie);
    }
    else
    {
        QMessageBox::warning(this, "No selection", "Select a row before clicking on return");
    }
}

void EditorDialog::returnMovie(int movie)
{
    QSqlQuery query;

    query.prepare("select rent_id from rent where DATE(start_date) <= NOW() and end_date IS NULL and movie_id = :movie");
    query.bindValue(":movie", movie);
    query.exec();

    if (query.next())
    {
        int id = query.value(0).toInt();

        query.prepare("UPDATE rent SET end_date = NOW() WHERE rent_id = :id");
        query.bindValue(":id", id);
        query.exec();

        query.prepare("select start_date, end_date from rent WHERE rent_id = :id");
        query.bindValue(":id", id);
        query.exec();

        if (query.next())
        {
            QDateTime start = query.value(0).toDateTime();
            QDateTime end = query.value(1).toDateTime();

            seconds_to_DHMS(start.secsTo(end));
        }
    }
}

void EditorDialog::seconds_to_DHMS(quint32 duration)
{
    QString msg;
    QString res;

    int seconds = (int) (duration % 60);
    duration /= 60;

    int minutes = (int) (duration % 60);
    duration /= 60;

    int hours = (int) (duration % 24);
    int days = (int) (duration / 24);

    if (days >= 1)
    {
        msg = "Dept: ";
        res = res.sprintf("%dd %02d:%02d:%02d", days - 1, hours, minutes, seconds);
    }
    else
    {
        msg = "Rental period: ";
        res = res.sprintf("%dd %02d:%02d:%02d", days, hours, minutes, seconds);
    }

    QMessageBox::warning(this, "Movie returned", msg + res);
}

void EditorDialog::submitButton_Clicked()
{
    _model->database().transaction();

    if (_model->submitAll())
    {
        _model->database().commit();
        _model->select();
    }
    else
    {
        _model->database().rollback();
        QMessageBox::warning(this, "Database Error", trUtf8("Database error: %1").arg(_model->lastError().text()));
    }
}

void EditorDialog::filterAll()
{
    _model->setFilter("");
}

void EditorDialog::filterRent()
{
    _model->setFilter("id in (select movie_id from rent where DATE(start_date) <= NOW() and end_date IS NULL)");
}

void EditorDialog::filterStock()
{
    _model->setFilter("id not in (select movie_id from rent where DATE(start_date) <= NOW() and end_date IS NULL)");
}

void EditorDialog::setupModel()
{
    _model = new MovieTableModel();
    _model->select();
}

void EditorDialog::setupUi()
{
    // Buttons
    _addButton = new QPushButton("Add");
    _removeButton = new QPushButton("Remove");
    _rentButton = new QPushButton("Rent");
    _returnButton = new QPushButton("Return");
    _submitButton = new QPushButton("Submit");
    _revertButton = new QPushButton("Revert");

    _filterAllButton = new QPushButton("All");
    _filterRentButton = new QPushButton("Rented");
    _filterStockButton = new QPushButton("In stock");

    _addButton->setFocusPolicy(Qt::NoFocus);
    _removeButton->setFocusPolicy(Qt::NoFocus);
    _rentButton->setFocusPolicy(Qt::NoFocus);
    _returnButton->setFocusPolicy(Qt::NoFocus);
    _submitButton->setFocusPolicy(Qt::NoFocus);
    _revertButton->setFocusPolicy(Qt::NoFocus);

    _filterAllButton->setFocusPolicy(Qt::NoFocus);
    _filterRentButton->setFocusPolicy(Qt::NoFocus);
    _filterStockButton->setFocusPolicy(Qt::NoFocus);

    // Button Connect
    connect(_addButton, SIGNAL(clicked()), this, SLOT(addButton_Clicked()));
    connect(_removeButton, SIGNAL(clicked()), this, SLOT(removeButton_Clicked()));
    connect(_rentButton, SIGNAL(clicked()), this, SLOT(rentButton_Clicked()));
    connect(_returnButton, SIGNAL(clicked()), this, SLOT(returnButton_Clicked()));
    connect(_submitButton, SIGNAL(clicked()), this, SLOT(submitButton_Clicked()));
    connect(_revertButton, SIGNAL(clicked()), _model, SLOT(revertAll()));

    connect(_filterAllButton, SIGNAL(clicked()), this, SLOT(filterAll()));
    connect(_filterRentButton, SIGNAL(clicked()), this, SLOT(filterRent()));
    connect(_filterStockButton, SIGNAL(clicked()), this, SLOT(filterStock()));

    // Button Box
    _buttonBox = new QDialogButtonBox(Qt::Horizontal);
    _buttonBox->addButton(_rentButton, QDialogButtonBox::ActionRole);
    _buttonBox->addButton(_returnButton, QDialogButtonBox::ActionRole);
    _buttonBox->addButton(_addButton, QDialogButtonBox::ActionRole);
    _buttonBox->addButton(_removeButton, QDialogButtonBox::ActionRole);
    _buttonBox->addButton(_submitButton, QDialogButtonBox::ActionRole);
    _buttonBox->addButton(_revertButton, QDialogButtonBox::ActionRole);

    _filterBox = new QDialogButtonBox(Qt::Horizontal);
    _filterBox->addButton(_filterAllButton, QDialogButtonBox::ActionRole);
    _filterBox->addButton(_filterRentButton, QDialogButtonBox::ActionRole);
    _filterBox->addButton(_filterStockButton, QDialogButtonBox::ActionRole);

    // View
    _tableView = new QTableView(this);
    _tableView->setModel(_model);
    _tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _tableView->resizeColumnsToContents();
    _tableView->setItemDelegate(new MovieDelegate());
    _tableView->setColumnWidth(6, 85);
    _tableView->setColumnHidden(0, true);
    _tableView->setCurrentIndex(_model->index(0, 1));

    // User dialog
    _userDialog = new UserDialog(this);

    // Layout
    _mainLayout = new QVBoxLayout;
    _mainLayout->addWidget(_filterBox);
    _mainLayout->addWidget(_tableView);
    _mainLayout->addWidget(_buttonBox);
    setLayout(_mainLayout);

    // Window
    setWindowTitle("Movies");
    setFixedSize(900, 300);
}
