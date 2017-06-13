
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Sat Jun 10 11:05:50 2017
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

#ifndef _XTREEVIEW_HPP
#define _XTREEVIEW_HPP          1

#include <QTreeView>

class XTreeView : public QTreeView
{
    Q_OBJECT

    public:
        enum Events {
            XTreeViewItemDblClicked = 1001,
            XTreeModelMenuSelected,
            XTreeViewMenuInserted,
            XTreeViewItemInserted,
            XTreeViewItemRemoved,
            XTreeViewItemPaste,
            XTreeViewItemCut,
            XTreeViewItemSelected,
            XTreeViewMenuRenamed
        };

        XTreeView(QWidget *parent = 0);
        ~XTreeView();
};

#endif

