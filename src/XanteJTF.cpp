
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Sat Jun 10 11:09:09 2017
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

#include "xante_builder.hpp"

XanteItem::XanteItem()
{
}

XanteItem::~XanteItem()
{
}

void XanteItem::write(QJsonObject &root) const
{
    root["name"] = name;
    root["object_id"] = object_id;
    root["mode"] = mode;
}

XanteMenu::XanteMenu()
{
}

XanteMenu::~XanteMenu()
{
}

void XanteMenu::write(QJsonObject &root) const
{
    QJsonArray jitems;

    foreach(const XanteItem item, items) {
        QJsonObject it;
        item.write(it);
        jitems.append(it);
    }

    root["mode"] = mode;
    root["name"] = name;
    root["object_id"] = object_id;
    root["items"] = jitems;
}

void XanteJTF::write_jtf_internal(QJsonObject &root)
{
    QJsonObject japplication;

    japplication["version"] = version;
    japplication["revision"] = revision;
    japplication["build"] = build;
    japplication["beta"] = beta;

    root["jtf_revision"] = file_revision;
    root["language"] = "en-us";
    root["application"] = japplication;
}

void XanteJTF::write_jtf_general(QJsonObject &root)
{
    root["name"] = application_name;
    root["description"] = description;
    root["plugin"] = plugin;
    root["config_pathname"] = cfg_pathname;
    root["log_pathname"] = log_pathname;
    root["log_level"] = "info";
    root["company"] = company;
}

void XanteJTF::write_jtf_ui(QJsonObject &root)
{
    QJsonArray jmenus;
    XanteMenu menu;

    foreach (const XanteMenu menu, menus) {
        QJsonObject m;
        menu.write(m);
        jmenus.append(m);
    }

    root["main_menu"] = main_menu;
    root["menus"] = jmenus;
}

void XanteJTF::write_jtf_data(QJsonObject &root)
{
    QJsonObject jinternal, jui, jgeneral;
    write_jtf_internal(jinternal);
    write_jtf_ui(jui);
    write_jtf_general(jgeneral);

    root["internal"] = jinternal;
    root["general"] = jgeneral;
    root["ui"] = jui;
}

bool XanteJTF::save(QString filename)
{
    QFile file(filename);

    if (file.open(QIODevice::WriteOnly) == false) {
        return false;
    }

    QJsonObject jtf_object;
    write_jtf_data(jtf_object);
    QJsonDocument doc(jtf_object);
    file.write(doc.toJson(QJsonDocument::Compact));

    return true;
}

void XanteJTF::add_menu(XanteMenu menu)
{
    menus.append(menu);
}

void XanteJTF::set_main_menu(QString menu_name)
{
    main_menu = menu_name;
}

