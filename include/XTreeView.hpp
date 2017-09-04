
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

class XanteProject;

class XTreeView : public QTreeView
{
    Q_OBJECT

    public:
        int current_selected_menu = 0;
        int current_selected_item = 0;

        XTreeView(QWidget *parent = 0);
        ~XTreeView();
        void set_current_project(XanteProject *project) { this->project = project; }
        void control_actions(bool enable);

    protected:
        void mousePressEvent(QMouseEvent *event);
        void mouseDoubleClickEvent(QMouseEvent *event);

    private slots:
        void add_menu();
        void rename_menu();
        void add_item();
        void copy_item();
        void paste_item();
        void cut_item();
        void remove_item();
        void change_items_position();

    signals:
        void item_selected();
        void menu_selected();
        void content_changed();

    private:
        XanteProject *project = nullptr;
        bool selected_line = false;
        QMenu *menu;
        QAction *ac_add_menu, *ac_rename_menu, *ac_add_item, *ac_copy,
                *ac_paste, *ac_cut, *ac_remove, *ac_change;

        void create_menu(void);
        void display_selected_item(QModelIndex index);
};

#endif

