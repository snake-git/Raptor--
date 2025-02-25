/**
 *
 * Copyright (C) 2024 凉州刺史. All rights reserved.
 *
 * This file is part of Raptor.
 *
 * $RAPTOR_BEGIN_LICENSE$
 *
 * GNU General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl.html.
 *
 * $RAPTOR_END_LICENSE$
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include "RaptorUploadViewModel.h"

QVariant RaptorUploadViewModel::headerData(int qSection,
                                           Qt::Orientation qOrientation,
                                           int qRole) const
{
    if (qOrientation != Qt::Horizontal)
    {
        return QVariant();
    }

    switch (qRole)
    {
    case Qt::DisplayRole:
        if (qSection > 0 && qSection <= _Headers.length())
        {
            return _Headers[qSection - 1];
        }

        return QVariant();
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
    default:
        return QVariant();
    }
}

int RaptorUploadViewModel::rowCount(const QModelIndex& qIndex) const
{
    if (qIndex.isValid())
    {
        return 0;
    }

    return _Items.length();
}

int RaptorUploadViewModel::columnCount(const QModelIndex& qIndex) const
{
    if (qIndex.isValid())
    {
        return 0;
    }

    return _ColumnCount;
}

QVariant RaptorUploadViewModel::data(const QModelIndex& qIndex, int qRole) const
{
    if (!qIndex.isValid())
    {
        return QVariant();
    }

    const auto item = _Items[qIndex.row()];
    switch (qRole)
    {
    case Qt::UserRole:
        return QVariant::fromValue<RaptorTransferItem>(item);
    case Qt::DisplayRole:
        {
            switch (qIndex.column())
            {
            case 1:
                return item._Name;
            case 2:
                return QStringLiteral("%1/%2").arg(RaptorUtil::invokeStorageUnitConvert(item._Transferred), item._Size);
            case 3:
                return item._Created.split(' ')[0];
            default:
                return QVariant();
            }
        }
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
    default:
        return QVariant();
    }
}

bool RaptorUploadViewModel::setData(const QModelIndex& qIndex, const QVariant& qVariant, int qRole)
{
    if (!qIndex.isValid())
    {
        return QAbstractTableModel::setData(qIndex, qVariant, qRole);
    }

    if (qRole != Qt::EditRole)
    {
        return QAbstractTableModel::setData(qIndex, qVariant, qRole);
    }

    if (!qVariant.canConvert<RaptorTransferItem>())
    {
        return QAbstractTableModel::setData(qIndex, qVariant, qRole);
    }

    const auto item = qVariant.value<RaptorTransferItem>();
    _Items.replace(qIndex.row(), item);
    Q_EMIT dataChanged(qIndex, qIndex);
    return true;
}

void RaptorUploadViewModel::invokeHeaderSet(const QVector<QString>& qHeader)
{
    _Headers = qHeader;
}

void RaptorUploadViewModel::invokeColumnCountSet(const quint16& qCount)
{
    _ColumnCount = qCount;
}

bool RaptorUploadViewModel::removeRow(int qRow, const QModelIndex& qIndex)
{
    if (qRow < 0 || qRow > _Items.length())
    {
        return false;
    }

    beginRemoveRows(qIndex, qRow, qRow);
    _Items.removeAt(qRow);
    endRemoveRows();
    return true;
}

void RaptorUploadViewModel::invokeItemAppend(const RaptorTransferItem& item)
{
    beginInsertRows(QModelIndex(), _Items.length(), _Items.length());
    _Items << item;
    endInsertRows();
}

void RaptorUploadViewModel::invokeItemsAppend(const QVector<RaptorTransferItem>& items)
{
    if (items.isEmpty())
    {
        return;
    }

    beginInsertRows(QModelIndex(), _Items.length(), _Items.length() + items.length() - 1);
    _Items << items;
    endInsertRows();
}

void RaptorUploadViewModel::invokeItemsClear()
{
    beginResetModel();
    _Items.clear();
    endResetModel();
}

QVector<RaptorTransferItem> RaptorUploadViewModel::invokeItemsEject()
{
    return _Items;
}