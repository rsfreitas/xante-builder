
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Sat Jun 10 10:51:30 2017
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

#include <QWidget>
#include <QSplitter>
#include <QHBoxLayout>

#include "xante_builder.hpp"

XMainDialog::XMainDialog(QWidget *parent)
    : QWidget(parent)
{
    dialog_item = new XDialogItem(this);
    dialog_menu = new XDialogMenu(this);
    tree = new XTreeView(this);

    QSplitter *splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Horizontal);
    splitter->addWidget(tree);
    splitter->addWidget(dialog_menu);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(splitter);
    setLayout(layout);
}

void XMainDialog::set_current_project(XanteProject *project)
{
    this->project = project;
}

/*void XMainDialog::set_xante_menu_property_objects(void)
{
}

void XMainDialog::set_xante_item_property_objects(void)
{
}*/

