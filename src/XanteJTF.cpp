
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

void XanteItem::name(QString name)
{
    m_name = name;
    m_objectId = XanteJTF::objectIdCalc(m_applicationName, m_menuName, name);
}

void XanteItem::writeOptions(QJsonObject &root) const
{
    if ((m_type == XanteItem::Type::RadioChecklist) ||
        (m_type == XanteItem::Type::Checklist))
    {
        QJsonArray options;

        for (int i = 0; i < m_options.size(); i++)
            options.append(m_options.at(i));

        root["options"] = options;
    } else
        root["options"] = m_fixedOption;
}

QJsonObject XanteItem::writeInputRanges(void) const
{
    QJsonObject inputRanges;

    if (m_type == XanteItem::Type::InputString)
        inputRanges["string_length"] = m_stringLength;
    else {
        if (m_type == XanteItem::Type::InputInt) {
            inputRanges["max"] = m_maxInputRange.toInt();
            inputRanges["min"] = m_minInputRange.toInt();
        } else if (m_type == XanteItem::Type::InputFloat) {
            inputRanges["max"] = m_maxInputRange.toFloat();
            inputRanges["min"] = m_minInputRange.toFloat();
        }
    }

    return inputRanges;
}

QJsonObject XanteItem::writeConfig(void) const
{
    QJsonObject config;

    config["block"] = m_configBlock;
    config["item"] = m_configItem;

    return config;
}

QJsonObject XanteItem::writeEvents(void) const
{
    QJsonObject events;
    QMap<QString, enum XanteItem::Event> evMap;

    evMap.insert("item-selected", XanteItem::Event::Selected);
    evMap.insert("item-exit", XanteItem::Event::Exit);
    evMap.insert("item-value-confirm", XanteItem::Event::ValueConfirmed);
    evMap.insert("item-value-updated", XanteItem::Event::ValueChanged);

    QMapIterator<QString, enum XanteItem::Event> i(evMap);

    while (i.hasNext()) {
        i.next();
        QString evValue = m_events.value(i.value(), QString(""));

        if (evValue.isEmpty() == false)
            events[i.key()] = evValue;
    }

    return events;
}

QJsonObject XanteItem::writeHelp(void) const
{
    QJsonObject help;

    if (m_briefHelp.isEmpty() == false)
        help["brief"] = m_briefHelp;

    if (m_descriptiveHelp.isEmpty() == false)
        help["description"] = m_descriptiveHelp;

    if (((m_type == XanteItem::Type::Checklist) ||
         (m_type == XanteItem::Type::RadioChecklist)) &&
        m_helpOptions.size() != 0)
    {
        QJsonArray options;
        int i, t = m_helpOptions.size();

        for (i = 0; i < t; i++)
            options.append(m_helpOptions.at(i));

        help["options"] = options;
    }

    return help;
}

/*
 * Writes the current content of a XanteItem object into a JSON object.
 */
void XanteItem::write(QJsonObject &root) const
{
    root["name"] = m_name;
    root["object_id"] = m_objectId;
    root["mode"] = m_mode;
    root["type"] = m_typeDescription.value(m_type);

    if (m_defaultValue.isEmpty() == false)
        root["default_value"] = m_defaultValue;

    if ((m_type == XanteItem::Type::Menu) ||
        (m_type == XanteItem::Type::DeleteDynamicMenu))
    {
        root["menu_id"] = m_menuReferenceId;
    }

    if (hasOptions())
        writeOptions(root);

    if (hasInputRanges())
        root["input_ranges"] = writeInputRanges();

    if (hasConfig())
        root["config"] = writeConfig();

    if (hasEvents())
        root["events"] = writeEvents();

    if (hasHelp())
        root["help"] = writeHelp();
}

void XanteItem::preLoad(void)
{
    m_typeDescription.insert(XanteItem::Type::Menu, QString("menu"));
    m_typeDescription.insert(XanteItem::Type::InputInt, QString("input-int"));
    m_typeDescription.insert(XanteItem::Type::InputFloat, QString("input-float"));
    m_typeDescription.insert(XanteItem::Type::InputDate, QString("input-date"));
    m_typeDescription.insert(XanteItem::Type::InputTime, QString("input-time"));
    m_typeDescription.insert(XanteItem::Type::InputString, QString("input-string"));
    m_typeDescription.insert(XanteItem::Type::InputPasswd, QString("input-passwd"));
    m_typeDescription.insert(XanteItem::Type::Calendar, QString("calendar"));
    m_typeDescription.insert(XanteItem::Type::Timebox, QString("timebox"));
    m_typeDescription.insert(XanteItem::Type::RadioChecklist, QString("radio-checklist"));
    m_typeDescription.insert(XanteItem::Type::Checklist, QString("checklist"));
    m_typeDescription.insert(XanteItem::Type::YesNo, QString("yesno"));
    m_typeDescription.insert(XanteItem::Type::DynamicMenu, QString("dynamic-menu"));
    m_typeDescription.insert(XanteItem::Type::DeleteDynamicMenu, QString("delete-dynamic-menu"));
    m_typeDescription.insert(XanteItem::Type::AddDynamicMenu, QString("add-dynamic-menu"));
}

XanteItem::XanteItem(QString applicationName, QString menuName, QString name)
    : m_applicationName(applicationName), m_menuName(menuName), m_name(name)
{
    preLoad();
    this->name(name);
    mode(XanteMode::XanteAccessEdit);
    type(XanteItem::Type::Menu);
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

void XanteItem::parseCommonData(QJsonObject item)
{
    int tmp;

    /* Load item from JSON */

    m_name = item["name"].toString();
    m_objectId = item["object_id"].toString();
    m_defaultValue = item["default_value"].toString();
    m_menuReferenceId = item["menuId"].toString();

    tmp = item["mode"].toInt();
    m_mode = (enum XanteMode)tmp;
    m_type = toXanteItem(item["type"].toString());

    QJsonValue value = item["options"];

    if (value.type() == QJsonValue::String)
        m_fixedOption = value.toString();
    else {
        QJsonArray options = value.toArray();

        foreach(const QJsonValue &v, options)
            m_options.append(v.toString());
    }
}

void XanteItem::parseEventsData(QJsonObject item)
{
    QJsonValue value = item["events"];

    if ((value.type() != QJsonValue::Undefined) &&
        (value.type() != QJsonValue::Null))
    {
        QJsonObject events = value.toObject();
        QMap<QString, enum XanteItem::Event> evMap;

        evMap.insert("item-selected", XanteItem::Event::Selected);
        evMap.insert("item-exit", XanteItem::Event::Exit);
        evMap.insert("item-value-confirm", XanteItem::Event::ValueConfirmed);
        evMap.insert("item-value-updated", XanteItem::Event::ValueChanged);

        QMapIterator<QString, enum XanteItem::Event> i(evMap);

        while (i.hasNext()) {
            i.next();
            value = events[i.key()];

            if (value.type() == QJsonValue::String)
                m_events.insert(i.value(), value.toString());
        }
    }
}

void XanteItem::parseConfigData(QJsonObject item)
{
    QJsonValue value = item["config"];

    if (value.type() != QJsonValue::Object)
        return;

    QJsonObject config = value.toObject();

    m_configBlock = config["block"].toString();
    m_configItem = config["item"].toString();
}

void XanteItem::parseRangesData(QJsonObject item)
{
    QJsonValue value = item["input_ranges"];

    if (value.type() != QJsonValue::Object)
        return;

    QJsonObject inputRanges = value.toObject();

    m_stringLength = inputRanges["string_length"].toInt();
    m_minInputRange = QVariant(inputRanges["min"].toDouble());
    m_maxInputRange = QVariant(inputRanges["max"].toDouble());
}

void XanteItem::parseHelpData(QJsonObject item)
{
    QJsonValue value = item["help"];

    if (value.type() != QJsonValue::Object)
        return;

    QJsonObject help = value.toObject();

    m_briefHelp = help["brief"].toString();
    m_descriptiveHelp = help["description"].toString();

    QJsonArray options = help["options"].toArray();

    foreach(const QJsonValue &v, options)
        m_helpOptions.append(v.toString());
}

void XanteItem::parse(QJsonObject item)
{
    parseCommonData(item);
    parseEventsData(item);
    parseConfigData(item);
    parseRangesData(item);
    parseHelpData(item);
}

XanteItem::XanteItem(QString applicationName, QString menuName,
    QJsonObject item)
    : m_applicationName(applicationName), m_menuName(menuName)
{
    preLoad();
    parse(item);
}

/*
 *
 * XanteMenu
 *
 */

void XanteMenu::name(QString name)
{
    m_name = name;
    m_objectId = XanteJTF::objectIdCalc(m_applicationName, name);
}

void XanteMenu::preLoad(void)
{
    m_typeDescription.insert(XanteMenu::Type::Dynamic, QString("dynamic"));
    m_typeDescription.insert(XanteMenu::Type::Default, QString("default"));
}

void XanteMenu::parseCommonData(QJsonObject menu)
{
    int tmp;

    m_name = menu["name"].toString();
    m_objectId = menu["object_id"].toString();
    tmp = menu["mode"].toInt();
    m_mode = (enum XanteMode)tmp;
}

void XanteMenu::parseEventsData(QJsonObject menu)
{
    QJsonValue value = menu["events"];

    if ((value.type() != QJsonValue::Undefined) &&
        (value.type() != QJsonValue::Null))
    {
        QJsonObject events = value.toObject();
        QMap<QString, enum XanteMenu::Event> evMap;

        evMap.insert("item-selected", XanteMenu::Event::Selected);
        evMap.insert("menu-exit", XanteMenu::Event::Exit);

        QMapIterator<QString, enum XanteMenu::Event> i(evMap);

        while (i.hasNext()) {
            i.next();
            value = events[i.key()];

            if (value.type() == QJsonValue::String)
                m_events.insert(i.value(), value.toString());
        }
    }
}

void XanteMenu::parseDynamicData(QJsonObject menu)
{
    QJsonValue value = menu["type"];

    if ((value.type() == QJsonValue::Undefined) ||
        (value.type() == QJsonValue::Null))
    {
        m_type = XanteMenu::Type::Default;
    } else {
        QString tmp = value.toString();

        if (tmp == "dynamic") {
            QJsonObject dynamic = menu["dynamic"].toObject();

            m_type = XanteMenu::Type::Dynamic;
            value = dynamic["copies"];

            if (value.type() == QJsonValue::Double) {
                m_dynamicType = XanteMenu::FixedSize;
                m_dynamicCopies = value.toInt();
            } else if (value.type() == QJsonValue::Array) {
                QJsonArray copies = value.toArray();
                m_dynamicType = XanteMenu::FixedOptions;

                foreach(const QJsonValue &v, copies)
                    m_copies.append(v.toString());
            } else {
                QJsonObject origin = dynamic["origin"].toObject();

                m_dynamicType = XanteMenu::DynamicOptions;
                m_dynamicOriginBlock = origin["block"].toString();
                m_dynamicOriginItem = origin["item"].toString();
                m_dynamicBlockPrefix = dynamic["block_prefix"].toString();
            }
        } else
            m_type = XanteMenu::Type::Default;
    }
}

void XanteMenu::parseItems(QJsonObject menu)
{
    QJsonArray jitems = menu["items"].toArray();

    foreach(const QJsonValue &v, jitems) {
        XanteItem it(m_applicationName, m_name, v.toObject());
        m_items.append(it);
    }
}

void XanteMenu::parse(QJsonObject menu)
{
    parseCommonData(menu);
    parseEventsData(menu);
    parseDynamicData(menu);
    parseItems(menu);
}

XanteMenu::XanteMenu(QString applicationName, QJsonObject menu)
    : m_applicationName(applicationName)
{
    preLoad();
    parse(menu);
}

XanteMenu::XanteMenu(QString applicationName, QString name)
    : m_applicationName(applicationName), m_name(name)
{
    preLoad();
    mode(XanteMode::XanteAccessEdit);
    this->name(name);
    type(XanteMenu::Type::Default);

    /* We always create an empty item for a new menu */
    XanteItem it(applicationName, name, QString("Item"));
    m_items.append(it);
}

QJsonObject XanteMenu::writeEvents(void) const
{
    QJsonObject events;
    QMap<QString, enum XanteMenu::Event> evMap;

    evMap.insert("selected", XanteMenu::Event::Selected);
    evMap.insert("exit", XanteMenu::Event::Exit);

    QMapIterator<QString, enum XanteMenu::Event> i(evMap);

    while (i.hasNext()) {
        i.next();
        QString evValue = m_events.value(i.value(), QString(""));

        if (evValue.isEmpty() == false)
            events[i.key()] = evValue;
    }

    return events;
}

QJsonObject XanteMenu::writeDynamic(void) const
{
    QJsonArray optArray;
    QJsonObject origin, dynamic;

    switch (m_dynamicType) {
        case XanteMenu::DynamicType::FixedSize:
            dynamic["copies"] = m_dynamicCopies;
            break;

        case XanteMenu::DynamicType::FixedOptions:
            for (int i = 0; i < m_copies.size(); i++)
                optArray.append(m_copies.at(i));

            dynamic["copies"] = optArray;
            break;

        case XanteMenu::DynamicType::DynamicOptions:
            origin["block"] = m_dynamicOriginBlock;
            origin["item"] = m_dynamicOriginItem;
            dynamic["block_prefix"] = m_dynamicBlockPrefix;
            dynamic["origin"] = origin;
            break;

        default:
            break;
    }

    return dynamic;
}

void XanteMenu::write(QJsonObject &root) const
{
    QJsonArray jitems;

    foreach(const XanteItem item, m_items) {
        QJsonObject it;
        item.write(it);
        jitems.append(it);
    }

    root["mode"] = m_mode;
    root["name"] = m_name;
    root["object_id"] = m_objectId;
    root["type"] = m_typeDescription.value(m_type);

    if (hasEvents())
        root["events"] = writeEvents();

    if (m_type == XanteMenu::Type::Dynamic)
        root["dynamic"] = writeDynamic();

    root["items"] = jitems;
}

/*
 *
 * XanteJTF
 *
 */

void XanteJTF::writeJtfInternal(QJsonObject &root)
{
    QJsonObject japplication;

    japplication["version"] = m_version;
    japplication["revision"] = m_revision;
    japplication["build"] = m_build;
    japplication["beta"] = m_beta;

    root["jtf_revision"] = m_fileRevision;
    root["language"] = "en-us";
    root["application"] = japplication;
}

void XanteJTF::writeJtfGeneral(QJsonObject &root)
{
    root["name"] = m_applicationName;
    root["description"] = m_description;
    root["module"] = m_plugin;
    root["config_pathname"] = m_cfgPathname;
    root["log_pathname"] = m_logPathname;
    root["log_level"] = "info";
    root["company"] = m_company;
}

void XanteJTF::writeJtfUi(QJsonObject &root)
{
    QJsonArray jmenus;
    XanteMenu menu;

    foreach (const XanteMenu menu, m_menus) {
        QJsonObject m;
        menu.write(m);
        jmenus.append(m);
    }

    root["main_menu"] = m_mainMenu;
    root["menus"] = jmenus;
}

void XanteJTF::writeJtfData(QJsonObject &root)
{
    QJsonObject jinternal, jui, jgeneral;

    writeJtfInternal(jinternal);
    writeJtfUi(jui);
    writeJtfGeneral(jgeneral);

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

    QJsonObject jtfObject;
    writeJtfData(jtfObject);
    QJsonDocument doc(jtfObject);
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
    m_jtfRoot = d.object();
    loadJtfFromFile();

    /* We're not empty anymore */
    m_empty = false;
}

void XanteJTF::loadJtfFromFile(void)
{
    loadJtfInternal();
    loadJtfGeneral();
    loadJtfUi();
}

void XanteJTF::loadJtfInternal(void)
{
    QJsonValue value = m_jtfRoot.value(QString("internal"));
    QJsonObject internal = value.toObject();

    m_fileRevision = internal["jtf_revision"].toInt();
    value = internal["application"];
    QJsonObject application = value.toObject();

    m_version = application["version"].toString();
    m_revision = application["revision"].toInt();
    m_build = application["build"].toInt();
    m_beta = application["beta"].toBool();
}

void XanteJTF::loadJtfGeneral(void)
{
    QJsonValue value = m_jtfRoot.value(QString("general"));
    QJsonObject general = value.toObject();

    m_applicationName = general["name"].toString();
    m_description = general["description"].toString();
    m_company = general["company"].toString();
    m_plugin = general["module"].toString();
    m_cfgPathname = general["config_pathname"].toString();
    m_logPathname = general["log_pathname"].toString();
}

void XanteJTF::loadJtfUi(void)
{
    QJsonValue value = m_jtfRoot.value(QString("ui"));
    QJsonObject ui = value.toObject();
    QJsonArray jmenus = ui["menus"].toArray();
    m_mainMenu = ui["main_menu"].toString();

    foreach(const QJsonValue &v, jmenus) {
        XanteMenu m(m_applicationName, v.toObject());
        m_menus.append(m);
    }
}

/*
 * Here we calculate an object's objectId property. It is composed of the
 * application name and the menu name, if we're talking about a menu. If it
 * is an item, the item's name is also used.
 */
QString XanteJTF::objectIdCalc(QString applicationName, QString menuName,
    QString itemName)
{
    QCryptographicHash hash(QCryptographicHash::Sha1);

    hash.addData(applicationName.toStdString().c_str(), menuName.size());
    hash.addData(menuName.toStdString().c_str(), menuName.size());

    if (itemName != nullptr)
        hash.addData(itemName.toStdString().c_str(), itemName.size());

    QByteArray res = hash.result().toHex();

    return QString(res);
}

void XanteJTF::buildDefaultMenu(void)
{
    XanteMenu m(m_applicationName, QString("Main"));
    m_menus.append(m);
}

XanteMenu &XanteJTF::getMenu(QString objectId)
{
    int index = m_menus.indexOf(objectId);

    return menuAt(index);
}

XanteMenu &XanteJTF::menuAt(int index)
{
    if ((index < 0) || (index > m_menus.size()))
        throw std::out_of_range("Menu not found.");

    return m_menus[index];
}

void XanteJTF::clear(void)
{
    m_empty = true;
}

