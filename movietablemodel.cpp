#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

#include "movietablemodel.h"

MovieTableModel::MovieTableModel(QObject *parent) :
    QSqlRelationalTableModel(parent)
{
    setTable("movie");
    setSort(1, Qt::AscendingOrder);
    setEditStrategy(QSqlTableModel::OnManualSubmit);

    setHeaderData(0, Qt::Horizontal, trUtf8("ID"));
    setHeaderData(1, Qt::Horizontal, trUtf8("Title"));
    setHeaderData(2, Qt::Horizontal, trUtf8("Main characters"));
    setHeaderData(3, Qt::Horizontal, trUtf8("Director"));
    setHeaderData(4, Qt::Horizontal, trUtf8("Distributor"));
    setHeaderData(5, Qt::Horizontal, trUtf8("Release date"));
    setHeaderData(6, Qt::Horizontal, trUtf8("Status"));
    setHeaderData(7, Qt::Horizontal, trUtf8("Availability"));
}

void MovieTableModel::insertRow(int row)
{
    QSqlRelationalTableModel::insertRow(row);

    // default values
    setData(index(row, 1), "untitled");;
    setData(index(row, 6), 1);
}

QVariant MovieTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (index.column() == 7 && role == Qt::TextAlignmentRole)
    {
        return QVariant(Qt::AlignRight | Qt::AlignVCenter);
    }

    if (index.column() == 7 && (role == Qt::DisplayRole || role == Qt::EditRole))
    {
        QSqlQuery query;

        QString movie_id = this->data(this->index(index.row(), 0)).toString();

        query.exec("select count(*) from rent where DATE(start_date) <= NOW() and end_date IS NULL and movie_id = " + movie_id);

        if (query.next())
        {
            int result = query.value(0).toInt();

            if (result == 0 || result == 1)
            {
                return QVariant(result);
            }
            else
            {
                QMessageBox::critical(0, "Query Error", "Query result error: result is bigger than 1");
            }
        }
        else
        {
            QMessageBox::critical(0, "Query Error", "Query result error: no lines");
        }
    }
    else
    {
        return QSqlRelationalTableModel::data(index, role);
    }
}

int MovieTableModel::columnCount(const QModelIndex& parent) const
{
    // increase the number of columns by one
    return QSqlRelationalTableModel::columnCount(parent) + 1;
}
