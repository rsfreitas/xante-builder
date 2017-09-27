
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
#include <QVector>
#include <QMap>

#include "xante_builder.hpp"

/*
 *
 * XanteItem
 *
 */

void XanteItem::set_name(QString name)
{
    this->name = name;
    object_id = XanteJTF::object_id_calc(application_name, menu_name, name);
}

void XanteItem::write_options(QJsonObject &root) const
{
    if ((type == XanteItem::Type::RadioChecklist) ||
        (type == XanteItem::Type::Checklist))
    {
        QJsonArray options;

        for (int i = 0; i < this->options.size(); i++)
            options.append(this->options.at(i));

        root["options"] = options;
    } else
        root["options"] = fixed_option;
}

void XanteItem::write_input_ranges(QJsonObject &input_ranges) const
{
    if (type == XanteItem::Type::InputString)
        input_ranges["string_length"] = string_length;
    else {
        if (type == XanteItem::Type::InputInt) {
            input_ranges["max"] = max_input_range.toInt();
            input_ranges["min"] = min_input_range.toInt();
        } else if (type == XanteItem::Type::InputFloat) {
            input_ranges["max"] = max_input_range.toFloat();
            input_ranges["min"] = min_input_range.toFloat();
        }
    }
}

void XanteItem::write_config(QJsonObject &config) const
{
    config["block"] = config_block;
    config["item"] = config_item;
}

void XanteItem::write_events(QJsonObject &events) const
{
    QMap<QString, enum XanteItem::Event> ev_map;

    ev_map.insert("selected", XanteItem::Event::Selected);
    ev_map.insert("exit", XanteItem::Event::Exit);
    ev_map.insert("value_confirmed", XanteItem::Event::ValueConfirmed);
    ev_map.insert("value_changed", XanteItem::Event::ValueChanged);

    QMapIterator<QString, enum XanteItem::Event> i(ev_map);

    while (i.hasNext()) {
        i.next();
        QString ev_value = this->events.value(i.value(), QString(""));

        if (ev_value.isEmpty() == false)
            events[i.key()] = ev_value;
    }
}

void XanteItem::write(QJsonObject &root) const
{
    root["name"] = name;
    root["object_id"] = object_id;
    root["mode"] = mode;
    root["type"] = type_description.value(type);

    if (default_value.isEmpty() == false)
        root["default_value"] = default_value;

    if ((type == XanteItem::Type::Menu) ||
        (type == XanteItem::Type::DeleteDynamicMenu))
    {
        root["menu_id"] = menu_reference_id;
    }

    if (has_options())
        write_options(root);

    if (has_input_ranges()) {
        QJsonObject input_ranges;

        write_input_ranges(input_ranges);
        root["input_ranges"] = input_ranges;
    }

    if (has_config()) {
        QJsonObject config;

        write_config(config);
        root["config"] = config;
    }

    if (has_events()) {
        QJsonObject events;

        write_events(events);
        root["events"] = events;
    }
}

void XanteItem::pre_load(void)
{
    type_description.insert(XanteItem::Type::Menu, QString("menu"));
    type_description.insert(XanteItem::Type::InputInt, QString("input-int"));
    type_description.insert(XanteItem::Type::InputFloat, QString("input-float"));
    type_description.insert(XanteItem::Type::InputDate, QString("input-date"));
    type_description.insert(XanteItem::Type::InputTime, QString("input-time"));
    type_description.insert(XanteItem::Type::InputString, QString("input-string"));
    type_description.insert(XanteItem::Type::InputPasswd, QString("input-passwd"));
    type_description.insert(XanteItem::Type::Calendar, QString("calendar"));
    type_description.insert(XanteItem::Type::Timebox, QString("timebox"));
    type_description.insert(XanteItem::Type::RadioChecklist, QString("radio-checklist"));
    type_description.insert(XanteItem::Type::Checklist, QString("checklist"));
    type_description.insert(XanteItem::Type::YesNo, QString("yesno"));
    type_description.insert(XanteItem::Type::DynamicMenu, QString("dynamic-menu"));
    type_description.insert(XanteItem::Type::DeleteDynamicMenu, QString("delete-dynamic-menu"));
    type_description.insert(XanteItem::Type::AddDynamicMenu, QString("add-dynamic-menu"));
}

XanteItem::XanteItem(QString application_name, QString menu_name, QString name)
    : application_name(application_name), menu_name(menu_name), name(name)
{
    pre_load();
    set_name(name);
    set_mode(XanteMode::XanteAccessEdit);
    set_type(XanteItem::Type::Menu);
}

enum XanteItem::Type XanteItem::toXanteItem(const QString &type)
{
    if (type == "menu")
        return XanteItem::Type::Menu;
    else if (type == "input-int")
        return XanteItem::Type::InputInt;
    else if (type == "input-float")
        return XanteItem::Type::InputFloat;
    else if (type == "input-date")
        return XanteItem::Type::InputDate;
    else if (type == "input-time")
        return XanteItem::Type::InputTime;
    else if (type == "input-string")
        return XanteItem::Type::InputString;
    else if (type == "input-passwd")
        return XanteItem::Type::InputPasswd;
    else if (type == "calendar")
        return XanteItem::Type::Calendar;
    else if (type == "timebox")
        return XanteItem::Type::Timebox;
    else if (type == "radio-checklist")
        return XanteItem::Type::RadioChecklist;
    else if (type == "checklist")
        return XanteItem::Type::Checklist;
    else if (type == "yesno")
        return XanteItem::Type::YesNo;
    else if (type == "dynamic-menu")
        return XanteItem::Type::DynamicMenu;
    else if (type == "delete-dynamic-menu")
        return XanteItem::Type::DeleteDynamicMenu;
    else if (type == "add-dynamic-menu")
        return XanteItem::Type::AddDynamicMenu;

    return XanteItem::Type::Unknown;
}

void XanteItem::parse_common_data(QJsonObject item)
{
    int tmp;

    /* Load item from JSON */

    name = item["name"].toString();
    object_id = item["object_id"].toString();
    default_value = item["default_value"].toString();
    menu_reference_id = item["menu_id"].toString();

    tmp = item["mode"].toInt();
    mode = (enum XanteMode)tmp;
    type = toXanteItem(item["type"].toString());

    QJsonValue value = item["options"];

    if (value.type() == QJsonValue::String)
        fixed_option = value.toString();
    else {
        QJsonArray options = value.toArray();

        foreach(const QJsonValue &v, options)
            this->options.append(v.toString());
    }
}

void XanteItem::parse_events_data(QJsonObject item)
{
    QJsonValue value = item["events"];

    if ((value.type() != QJsonValue::Undefined) &&
        (value.type() != QJsonValue::Null))
    {
        QJsonObject events = value.toObject();
        QMap<QString, enum XanteItem::Event> ev_map;

        ev_map.insert("item-selected", XanteItem::Event::Selected);
        ev_map.insert("item-exit", XanteItem::Event::Exit);
        ev_map.insert("item-value-confirm", XanteItem::Event::ValueConfirmed);
        ev_map.insert("item-value-updated", XanteItem::Event::ValueChanged);

        QMapIterator<QString, enum XanteItem::Event> i(ev_map);

        while (i.hasNext()) {
            i.next();
            value = events[i.key()];

            if (value.type() == QJsonValue::String)
                this->events.insert(i.value(), value.toString());
        }
    }
}

void XanteItem::parse_config_data(QJsonObject item)
{
    QJsonValue value = item["config"];

    if (value.type() != QJsonValue::Object)
        return;

    QJsonObject config = value.toObject();

    config_block = config["block"].toString();
    config_item = config["item"].toString();
}

void XanteItem::parse_ranges_data(QJsonObject item)
{
    QJsonValue value = item["input_ranges"];

    if (value.type() != QJsonValue::Object)
        return;

    QJsonObject input_ranges = value.toObject();

    string_length = input_ranges["string_length"].toInt();
    min_input_range = QVariant(input_ranges["min"].toDouble());
    max_input_range = QVariant(input_ranges["max"].toDouble());
}

void XanteItem::parse_help_data(QJsonObject item)
{
    QJsonValue value = item["help"];

    if (value.type() != QJsonValue::Object)
        return;

    QJsonObject help = value.toObject();

    brief_help = help["brief"].toString();
    descriptive_help = help["description"].toString();

    QJsonArray options = help["options"].toArray();

    foreach(const QJsonValue &v, options)
        help_options.append(v.toString());
}

void XanteItem::parse(QJsonObject item)
{
    parse_common_data(item);
    parse_events_data(item);
    parse_config_data(item);
    parse_ranges_data(item);
    parse_help_data(item);
}

XanteItem::XanteItem(QString application_name, QString menu_name,
    QJsonObject item)
    : application_name(application_name), menu_name(menu_name)
{
    pre_load();
    parse(item);
}

/*
 *
 * XanteMenu
 *
 */

void XanteMenu::set_name(QString name)
{
    this->name = name;
    object_id = XanteJTF::object_id_calc(application_name, name);
}

void XanteMenu::pre_load(void)
{
    type_description.insert(XanteMenu::Type::Dynamic, QString("dynamic"));
    type_description.insert(XanteMenu::Type::Default, QString("default"));
}

void XanteMenu::parse_common_data(QJsonObject menu)
{
    int tmp;

    name = menu["name"].toString();
    object_id = menu["object_id"].toString();
    tmp = menu["mode"].toInt();
    mode = (enum XanteMode)tmp;
}

void XanteMenu::parse_events_data(QJsonObject menu)
{
    QJsonValue value = menu["events"];

    if ((value.type() != QJsonValue::Undefined) &&
        (value.type() != QJsonValue::Null))
    {
        QJsonObject events = value.toObject();
        QMap<QString, enum XanteMenu::Event> ev_map;

        ev_map.insert("selected", XanteMenu::Event::Selected);
        ev_map.insert("exit", XanteMenu::Event::Exit);

        QMapIterator<QString, enum XanteMenu::Event> i(ev_map);

        while (i.hasNext()) {
            i.next();
            value = events[i.key()];

            if (value.type() == QJsonValue::String)
                this->events.insert(i.value(), value.toString());
        }
    }
}

void XanteMenu::parse_dynamic_data(QJsonObject menu)
{
    QJsonValue value = menu["type"];

    if ((value.type() == QJsonValue::Undefined) ||
        (value.type() == QJsonValue::Null))
    {
        type = XanteMenu::Type::Default;
    } else {
        QString tmp = value.toString();

        if (tmp == "dynamic") {
            QJsonObject dynamic = menu["dynamic"].toObject();

            type = XanteMenu::Type::Dynamic;
            value = dynamic["copies"];

            if (value.type() == QJsonValue::Double) {
                dynamic_type = XanteMenu::FixedSize;
                dynamic_copies = value.toInt();
            } else if (value.type() == QJsonValue::Array) {
                QJsonArray copies = value.toArray();
                dynamic_type = XanteMenu::FixedOptions;

                foreach(const QJsonValue &v, copies)
                    this->copies.append(v.toString());
            } else {
                QJsonObject origin = dynamic["origin"].toObject();

                dynamic_type = XanteMenu::DynamicOptions;
                dynamic_origin_block = origin["block"].toString();
                dynamic_origin_item = origin["item"].toString();
                dynamic_block_prefix = dynamic["block_prefix"].toString();
            }
        } else
            type = XanteMenu::Type::Default;
    }
}

void XanteMenu::parse_items(QJsonObject menu)
{
    QJsonArray jitems = menu["items"].toArray();

    foreach(const QJsonValue &v, jitems) {
        XanteItem it(application_name, name, v.toObject());
        items.append(it);
    }
}

void XanteMenu::parse(QJsonObject menu)
{
    parse_common_data(menu);
    parse_events_data(menu);
    parse_dynamic_data(menu);
    parse_items(menu);
}

XanteMenu::XanteMenu(QString application_name, QJsonObject menu)
    : application_name(application_name)
{
    pre_load();
    parse(menu);
}

XanteMenu::XanteMenu(QString application_name, QString name)
    : application_name(application_name), name(name)
{
    pre_load();
    set_mode(XanteMode::XanteAccessEdit);
    set_name(name);
    set_type(XanteMenu::Type::Default);

    /* We always create an empty item for a new menu */
    XanteItem it(application_name, name, QString("Item"));
    items.append(it);
}

void XanteMenu::write_events(QJsonObject &events) const
{
    QMap<QString, enum XanteMenu::Event> ev_map;

    ev_map.insert("selected", XanteMenu::Event::Selected);
    ev_map.insert("exit", XanteMenu::Event::Exit);

    QMapIterator<QString, enum XanteMenu::Event> i(ev_map);

    while (i.hasNext()) {
        i.next();
        QString ev_value = this->events.value(i.value(), QString(""));

        if (ev_value.isEmpty() == false)
            events[i.key()] = ev_value;
    }
}

void XanteMenu::write_dynamic(QJsonObject &dynamic) const
{
    QJsonArray opt_array;
    QJsonObject origin;

    switch (dynamic_type) {
        case XanteMenu::DynamicType::FixedSize:
            dynamic["copies"] = dynamic_copies;
            break;

        case XanteMenu::DynamicType::FixedOptions:
            for (int i = 0; i < copies.size(); i++)
                opt_array.append(copies.at(i));

            dynamic["copies"] = opt_array;
            break;

        case XanteMenu::DynamicType::DynamicOptions:
            origin["block"] = dynamic_origin_block;
            origin["item"] = dynamic_origin_item;
            dynamic["origin"] = origin;
            break;

        default:
            break;
    }
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
    root["type"] = type_description.value(type);

    if (has_events()) {
        QJsonObject events;

        write_events(events);
        root["events"] = events;
    }

    if (type == XanteMenu::Type::Dynamic) {
        QJsonObject dynamic;

        write_dynamic(dynamic);
        root["dynamic"] = dynamic;
    }

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

void XanteJTF::load(const QString &filename)
{
    QFile file;
    QString data;

    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();

    QJsonDocument d = QJsonDocument::fromJson(data.toUtf8());
    jtf_root = d.object();
    load_jtf_from_file();

    /* We're not empty anymore */
    empty = false;
}

void XanteJTF::load_jtf_from_file(void)
{
    load_jtf_internal();
    load_jtf_general();
    load_jtf_ui();
}

void XanteJTF::load_jtf_internal(void)
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

void XanteJTF::load_jtf_general(void)
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

void XanteJTF::load_jtf_ui(void)
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

XanteMenu &XanteJTF::get_menu(QString object_id)
{
    int index = menus.indexOf(object_id);

    return menu_at(index);
}

XanteMenu &XanteJTF::menu_at(int index)
{
    if ((index < 0) || (index > menus.size()))
        throw std::out_of_range("Menu not found.");

    return menus[index];
}

void XanteJTF::clear(void)
{
    empty = true;
}

