#include <QMessageBox>
#include <QSqlError>
#include <QSqlRelationalDelegate>

#include "editordialog.h"

EditorDialog::EditorDialog(QWidget *parent) : QDialog(parent)
{
    setupModel();
    setupUi();

    setWindowTitle("Movies");
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

    QModelIndex newIndex = _model->index(row, 0);
    _tableView->setCurrentIndex(newIndex);
    _tableView->edit(newIndex);
}

void EditorDialog::removeButton_Clicked()
{
    QModelIndex index = _tableView->currentIndex();

    if (index.isValid())
    {
        _model->removeRow(index.row());
        _tableView->setCurrentIndex(_model->index(index.row() - 1, 0));
    }
    else
    {
        QMessageBox::warning(this, "No selection", "Select a row before clicking on delete");
    }
}

void EditorDialog::submitButton_Clicked()
{
    _model->database().transaction();

    if (_model->submitAll())
    {
        _model->database().commit();
    }
    else
    {
        _model->database().rollback();
        QMessageBox::warning(this, "Database Error", trUtf8("Database error: %1").arg(_model->lastError().text()));
    }
}

void EditorDialog::setupModel()
{
    _model = new QSqlRelationalTableModel(this);
    _model->setTable("movie");
    _model->setSort(1, Qt::AscendingOrder);
    _model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //_model->setRelation(2, QSqlRelation("city", "id", "name"));
    _model->select();
}

void EditorDialog::setupUi()
{
    // Buttons
    _addButton = new QPushButton("Insert");
    _removeButton = new QPushButton("Remove");
    _submitButton = new QPushButton("Submit");
    _revertButton = new QPushButton("Revert");

    _addButton->setFocusPolicy(Qt::NoFocus);
    _removeButton->setFocusPolicy(Qt::NoFocus);
    _submitButton->setFocusPolicy(Qt::NoFocus);
    _revertButton->setFocusPolicy(Qt::NoFocus);

    // Button Connect
    connect(_addButton, SIGNAL(clicked()), this, SLOT(addButton_Clicked()));
    connect(_removeButton, SIGNAL(clicked()), this, SLOT(removeButton_Clicked()));
    connect(_submitButton, SIGNAL(clicked()), this, SLOT(submitButton_Clicked()));
    connect(_revertButton, SIGNAL(clicked()), _model, SLOT(revertAll()));

    // Button Box
    _buttonBox = new QDialogButtonBox(Qt::Horizontal);
    _buttonBox->addButton(_addButton, QDialogButtonBox::ActionRole);
    _buttonBox->addButton(_removeButton, QDialogButtonBox::ActionRole);
    _buttonBox->addButton(_submitButton, QDialogButtonBox::ActionRole);
    _buttonBox->addButton(_revertButton, QDialogButtonBox::ActionRole);

    _tableView = new QTableView(this);
    _tableView->setModel(_model);
    _tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    _tableView->resizeColumnsToContents();
    _tableView->setItemDelegate(new QSqlRelationalDelegate());

    // Layout
    _mainLayout = new QVBoxLayout;
    _mainLayout->addWidget(_tableView);
    _mainLayout->addWidget(_buttonBox);
    setLayout(_mainLayout);
}
