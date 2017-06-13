
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

/*
 * TODO:
 *
 * - Criar classes diferentes para tratar quando estiver editando um menu ou
 *   um item.
 *
 */

#include "xante_builder.hpp"

static const char *cb_menu_event_name[] = {
    "selected",
    "exit"
};

static const char *cb_item_event_name[] = {
    "selected",
    "value_confirmed",
    "value_changed",
    "exit"
};

static const char *cb_access_mode_name[] = {
    "hidden",
    "view",
    "edit"
};

static const char *cb_item_type_name[] = {
    NULL,
    "menu",
    "input-int",
    "input-float",
    "input-date",
    "input-string",
    "input-passwd",
    "input-time",
    "calendar",
    "timebox",
    "radio-checklist",
    "checklist",
    "yesno",
    "dynamic-menu",
    "delete-dynamic-menu",
    "add-dynamic-menu"
};

static const char *cb_menu_type_name[] = {
};

XMainDialog::XMainDialog(QWidget *parent)
    : QWidget(parent)
{
    /* Splitter horizontal */
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

