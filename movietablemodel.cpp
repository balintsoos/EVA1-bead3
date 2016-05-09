#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>

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
    setHeaderData(6, Qt::Horizontal, trUtf8("Availability"));
    setHeaderData(7, Qt::Horizontal, trUtf8("Rented"));
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

        query.exec("select count(*) from rent where movie_id = " + this->data(this->index(index.row(), 0)).toString());

        if (query.next())
        {
            return QVariant(query.value(0).toInt());
        }
        else
        {
            return QVariant(0);
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
