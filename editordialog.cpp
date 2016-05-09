#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
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

        // Rent the movie
        QModelIndex idIndex = _model->index(index.row(), 0);
        QString movie_id = _model->data(idIndex).toString();
        QSqlQuery query;
        query.prepare("insert into rent(member_id, movie_id, start_date, end_date) values(:member, :movie, NOW(), null)");
        query.bindValue(":member", 1);
        query.bindValue(":movie", movie_id);
        query.exec();
    }
    else
    {
        QMessageBox::warning(this, "No selection", "Select a row before clicking on rent");
    }
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
    _submitButton = new QPushButton("Submit");
    _revertButton = new QPushButton("Revert");

    _addButton->setFocusPolicy(Qt::NoFocus);
    _removeButton->setFocusPolicy(Qt::NoFocus);
    _rentButton->setFocusPolicy(Qt::NoFocus);
    _submitButton->setFocusPolicy(Qt::NoFocus);
    _revertButton->setFocusPolicy(Qt::NoFocus);

    // Button Connect
    connect(_addButton, SIGNAL(clicked()), this, SLOT(addButton_Clicked()));
    connect(_removeButton, SIGNAL(clicked()), this, SLOT(removeButton_Clicked()));
    connect(_rentButton, SIGNAL(clicked()), this, SLOT(rentButton_Clicked()));
    connect(_submitButton, SIGNAL(clicked()), this, SLOT(submitButton_Clicked()));
    connect(_revertButton, SIGNAL(clicked()), _model, SLOT(revertAll()));

    // Button Box
    _buttonBox = new QDialogButtonBox(Qt::Horizontal);
    _buttonBox->addButton(_rentButton, QDialogButtonBox::ActionRole);
    _buttonBox->addButton(_addButton, QDialogButtonBox::ActionRole);
    _buttonBox->addButton(_removeButton, QDialogButtonBox::ActionRole);
    _buttonBox->addButton(_submitButton, QDialogButtonBox::ActionRole);
    _buttonBox->addButton(_revertButton, QDialogButtonBox::ActionRole);

    // View
    _tableView = new QTableView(this);
    _tableView->setModel(_model);
    _tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _tableView->resizeColumnsToContents();
    _tableView->setItemDelegate(new MovieDelegate());
    _tableView->setColumnWidth(6, 85);
    _tableView->setColumnHidden(0, true);
    _tableView->setCurrentIndex(_model->index(0, 1));

    // Layout
    _mainLayout = new QVBoxLayout;
    _mainLayout->addWidget(_tableView);
    _mainLayout->addWidget(_buttonBox);
    setLayout(_mainLayout);

    // Window
    setWindowTitle("Movies");
    setFixedSize(900, 300);
}
