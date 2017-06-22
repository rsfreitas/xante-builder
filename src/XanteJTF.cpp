
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

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QCryptographicHash>
#include <QByteArray>

#include "xante_builder.hpp"

/*
 *
 * XanteItem
 *
 */

void XanteItem::write(QJsonObject &root) const
{
    root["name"] = name;
    root["object_id"] = object_id;
    root["mode"] = mode;
}

XanteItem::XanteItem(QString application_name, QString menu_name, QString name)
    : application_name(application_name), menu_name(menu_name), name(name)
{
    object_id = XanteJTF::object_id_calc(application_name, menu_name, name);
    mode = XANTE_ACCESS_EDIT;
}

XanteItem::XanteItem(QString application_name, QString menu_name,
    QJsonObject item)
    : application_name(application_name), menu_name(menu_name)
{
    int tmp;

    /* Load item from JSON */
    name = item["name"].toString();
    object_id = item["object_id"].toString();
    tmp = item["mode"].toInt();
    mode = (enum xante_mode)tmp;
}

/*
 *
 * XanteMenu
 *
 */

XanteMenu::XanteMenu(QString application_name, QJsonObject menu)
    : application_name(application_name)
{
    int tmp;

    /* Load menu from JSON */
    name = menu["name"].toString();
    object_id = menu["object_id"].toString();
    tmp = menu["mode"].toInt();
    mode = (enum xante_mode)tmp;

    QJsonArray jitems = menu["items"].toArray();

    foreach(const QJsonValue &v, jitems) {
        XanteItem it(application_name, name, v.toObject());
        items.append(it);
    }
}

XanteMenu::XanteMenu(QString application_name, QString name)
    : application_name(application_name), name(name)
{
    object_id = XanteJTF::object_id_calc(application_name, name);
    mode = XANTE_ACCESS_EDIT;

    /* We always create an empty item for a new menu */
    XanteItem it = XanteItem::Builder(application_name, name)
                        .set_name(QString("Item"))
                        .build();

    items.append(it);
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

/*
 *
 * XanteJTF
 *
 */

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

XanteJTF::XanteJTF(QString filename)
{
    QFile file;
    QString data;

    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();

    QJsonDocument d = QJsonDocument::fromJson(data.toUtf8());
    jtf_root = d.object();
}

void XanteJTF::read_jtf_data(void)
{
    read_jtf_internal();
    read_jtf_general();
    read_jtf_ui();
}

void XanteJTF::read_jtf_internal(void)
{
    QJsonValue value = jtf_root.value(QString("internal"));
    QJsonObject internal = value.toObject();

    file_revision = internal["jtf_revision"].toInt();
    value = internal["application"];
    QJsonObject application = value.toObject();

    version = application["version"].toString();
    revision = application["revision"].toInt();
    build = application["build"].toInt();
    beta = application["beta"].toBool();
}

void XanteJTF::read_jtf_general(void)
{
    QJsonValue value = jtf_root.value(QString("general"));
    QJsonObject general = value.toObject();

    application_name = general["name"].toString();
    description = general["description"].toString();
    company = general["company"].toString();
    plugin = general["plugin"].toString();
    cfg_pathname = general["config_pathname"].toString();
    log_pathname = general["log_pathname"].toString();
}

void XanteJTF::read_jtf_ui(void)
{
    QJsonValue value = jtf_root.value(QString("ui"));
    QJsonObject ui = value.toObject();
    QJsonArray jmenus = ui["menus"].toArray();
    main_menu = ui["main_menu"].toString();

    foreach(const QJsonValue &v, jmenus) {
        XanteMenu m(application_name, v.toObject());
        menus.append(m);
    }
}

/*
 * Here we calculate an object's object_id property. It is composed of the
 * application name and the menu name, if we're talking about a menu. If it
 * is an item, the item's name is also used.
 */
QString XanteJTF::object_id_calc(QString application_name, QString menu_name,
    QString item_name)
{
    QCryptographicHash hash(QCryptographicHash::Sha1);

    hash.addData(application_name.toStdString().c_str(), menu_name.size());
    hash.addData(menu_name.toStdString().c_str(), menu_name.size());

    if (item_name != nullptr)
        hash.addData(item_name.toStdString().c_str(), item_name.size());

    QByteArray res = hash.result().toHex();

    return QString(res);
}

void XanteJTF::build_default_menu(void)
{
    XanteMenu m(application_name, QString("Main"));
    menus.append(m);
}

