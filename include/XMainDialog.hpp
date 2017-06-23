
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Sat Jun 10 11:00:17 2017
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

#ifndef _XMAINDIALOG_HPP
#define _XMAINDIALOG_HPP          1

#include <QWidget>

class XanteProject;
class XTreeView;
class XTreeModel;
class XDialogItem;
class XDialogMenu;

class XMainDialog : public QWidget
{
    Q_OBJECT

    public:
        XMainDialog(QWidget *parent = 0);
        ~XMainDialog() {}
        void set_current_project(XanteProject *project);
        void set_tree_content(XTreeModel *model, bool enable_menu);

    private slots:
        void dialog_item_selected();
        void dialog_menu_selected();
        void dialog_content_changed();

    private:
        XanteProject *project = nullptr;
        XDialogItem *dialog_item;
        XDialogMenu *dialog_menu;
        XTreeView *tree;
};

#endif

