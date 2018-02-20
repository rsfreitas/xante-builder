
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Sat Jun 10 14:49:07 2017
 * Project: xante-builder
 *
 * Copyright (C) 2017 Rodrigo Freitas
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _XTREEMODEL_HPP
#define _XTREEMODEL_HPP          1

#include <QTreeView>
#include <QPixmap>

class XanteProject;

class XTreeItem
{
    public:
        ~XTreeItem();
        XTreeItem(const QList<QVariant> &data, int itemType = -1,
                  XTreeItem *parent = 0);

        void appendChild(XTreeItem *child);
        XTreeItem *child(int row);
        int childCount() const;
        int columnCount() const;
        QVariant data(int column) const;
        int row() const;
        XTreeItem *parent();
        QPixmap icon() const;

    private:
        QList<XTreeItem *> m_childItems;
        QList<QVariant> m_itemData;
        QMap<int, QString> m_iconName;
        QPixmap m_icon;
        QString m_path;
        XTreeItem *m_parentItem;
};

class XTreeModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        XTreeModel(bool enable, QWidget *parent = 0);
        ~XTreeModel();

        QVariant data(const QModelIndex &index, int role) const;
        QModelIndex index(int row, int column,
                          const QModelIndex &parent = QModelIndex()) const;

        QModelIndex parent(const QModelIndex &index) const;
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant headerData(int section, Qt::Orientation orientation,
                            int rotel = Qt::DisplayRole) const;

        void setupModelData(XTreeItem *parent);

    private:
        XTreeItem *rootItem;
};

#endif

