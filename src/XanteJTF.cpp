
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

void XanteItem::setName(QString name)
{
    this->name = name;
    objectId = XanteJTF::objectIdCalc(applicationName, menuName, name);
}

void XanteItem::writeOptions(QJsonObject &root) const
{
    if ((type == XanteItem::Type::RadioChecklist) ||
        (type == XanteItem::Type::Checklist))
    {
        QJsonArray options;

        for (int i = 0; i < this->options.size(); i++)
            options.append(this->options.at(i));

        root["options"] = options;
    } else
        root["options"] = fixedOption;
}

QJsonObject XanteItem::writeInputRanges(void) const
{
    QJsonObject inputRanges;

    if (type == XanteItem::Type::InputString)
        inputRanges["string_length"] = stringLength;
    else {
        if (type == XanteItem::Type::InputInt) {
            inputRanges["max"] = maxInputRange.toInt();
            inputRanges["min"] = minInputRange.toInt();
        } else if (type == XanteItem::Type::InputFloat) {
            inputRanges["max"] = maxInputRange.toFloat();
            inputRanges["min"] = minInputRange.toFloat();
        }
    }

    return inputRanges;
}

QJsonObject XanteItem::writeConfig(void) const
{
    QJsonObject config;

    config["block"] = configBlock;
    config["item"] = configItem;

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
        QString evValue = this->events.value(i.value(), QString(""));

        if (evValue.isEmpty() == false)
            events[i.key()] = evValue;
    }

    return events;
}

QJsonObject XanteItem::writeHelp(void) const
{
    QJsonObject help;

    if (briefHelp.isEmpty() == false)
        help["brief"] = briefHelp;

    if (descriptiveHelp.isEmpty() == false)
        help["description"] = descriptiveHelp;

    if (((type == XanteItem::Type::Checklist) ||
         (type == XanteItem::Type::RadioChecklist)) &&
        helpOptions.size() != 0)
    {
        QJsonArray options;
        int i, t = helpOptions.size();

        for (i = 0; i < t; i++)
            options.append(helpOptions.at(i));

        help["options"] = options;
    }

    return help;
}

/*
 * Writes the current content of a XanteItem object into a JSON object.
 */
void XanteItem::write(QJsonObject &root) const
{
    root["name"] = name;
    root["object_id"] = objectId;
    root["mode"] = mode;
    root["type"] = typeDescription.value(type);

    if (defaultValue.isEmpty() == false)
        root["default_value"] = defaultValue;

    if ((type == XanteItem::Type::Menu) ||
        (type == XanteItem::Type::DeleteDynamicMenu))
    {
        root["menu_id"] = menuReferenceId;
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
    typeDescription.insert(XanteItem::Type::Menu, QString("menu"));
    typeDescription.insert(XanteItem::Type::InputInt, QString("input-int"));
    typeDescription.insert(XanteItem::Type::InputFloat, QString("input-float"));
    typeDescription.insert(XanteItem::Type::InputDate, QString("input-date"));
    typeDescription.insert(XanteItem::Type::InputTime, QString("input-time"));
    typeDescription.insert(XanteItem::Type::InputString, QString("input-string"));
    typeDescription.insert(XanteItem::Type::InputPasswd, QString("input-passwd"));
    typeDescription.insert(XanteItem::Type::Calendar, QString("calendar"));
    typeDescription.insert(XanteItem::Type::Timebox, QString("timebox"));
    typeDescription.insert(XanteItem::Type::RadioChecklist, QString("radio-checklist"));
    typeDescription.insert(XanteItem::Type::Checklist, QString("checklist"));
    typeDescription.insert(XanteItem::Type::YesNo, QString("yesno"));
    typeDescription.insert(XanteItem::Type::DynamicMenu, QString("dynamic-menu"));
    typeDescription.insert(XanteItem::Type::DeleteDynamicMenu, QString("delete-dynamic-menu"));
    typeDescription.insert(XanteItem::Type::AddDynamicMenu, QString("add-dynamic-menu"));
}

XanteItem::XanteItem(QString applicationName, QString menuName, QString name)
    : applicationName(applicationName), menuName(menuName), name(name)
{
    preLoad();
    setName(name);
    setMode(XanteMode::XanteAccessEdit);
    setType(XanteItem::Type::Menu);
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

    name = item["name"].toString();
    objectId = item["object_id"].toString();
    defaultValue = item["default_value"].toString();
    menuReferenceId = item["menuId"].toString();

    tmp = item["mode"].toInt();
    mode = (enum XanteMode)tmp;
    type = toXanteItem(item["type"].toString());

    QJsonValue value = item["options"];

    if (value.type() == QJsonValue::String)
        fixedOption = value.toString();
    else {
        QJsonArray options = value.toArray();

        foreach(const QJsonValue &v, options)
            this->options.append(v.toString());
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
                this->events.insert(i.value(), value.toString());
        }
    }
}

void XanteItem::parseConfigData(QJsonObject item)
{
    QJsonValue value = item["config"];

    if (value.type() != QJsonValue::Object)
        return;

    QJsonObject config = value.toObject();

    configBlock = config["block"].toString();
    configItem = config["item"].toString();
}

void XanteItem::parseRangesData(QJsonObject item)
{
    QJsonValue value = item["input_ranges"];

    if (value.type() != QJsonValue::Object)
        return;

    QJsonObject inputRanges = value.toObject();

    stringLength = inputRanges["string_length"].toInt();
    minInputRange = QVariant(inputRanges["min"].toDouble());
    maxInputRange = QVariant(inputRanges["max"].toDouble());
}

void XanteItem::parseHelpData(QJsonObject item)
{
    QJsonValue value = item["help"];

    if (value.type() != QJsonValue::Object)
        return;

    QJsonObject help = value.toObject();

    briefHelp = help["brief"].toString();
    descriptiveHelp = help["description"].toString();

    QJsonArray options = help["options"].toArray();

    foreach(const QJsonValue &v, options)
        helpOptions.append(v.toString());
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
    : applicationName(applicationName), menuName(menuName)
{
    preLoad();
    parse(item);
}

/*
 *
 * XanteMenu
 *
 */

void XanteMenu::setName(QString name)
{
    this->name = name;
    objectId = XanteJTF::objectIdCalc(applicationName, name);
}

void XanteMenu::preLoad(void)
{
    typeDescription.insert(XanteMenu::Type::Dynamic, QString("dynamic"));
    typeDescription.insert(XanteMenu::Type::Default, QString("default"));
}

void XanteMenu::parseCommonData(QJsonObject menu)
{
    int tmp;

    name = menu["name"].toString();
    objectId = menu["object_id"].toString();
    tmp = menu["mode"].toInt();
    mode = (enum XanteMode)tmp;
}

void XanteMenu::parseEventsData(QJsonObject menu)
{
    QJsonValue value = menu["events"];

    if ((value.type() != QJsonValue::Undefined) &&
        (value.type() != QJsonValue::Null))
    {
        QJsonObject events = value.toObject();
        QMap<QString, enum XanteMenu::Event> evMap;

        evMap.insert("selected", XanteMenu::Event::Selected);
        evMap.insert("exit", XanteMenu::Event::Exit);

        QMapIterator<QString, enum XanteMenu::Event> i(evMap);

        while (i.hasNext()) {
            i.next();
            value = events[i.key()];

            if (value.type() == QJsonValue::String)
                this->events.insert(i.value(), value.toString());
        }
    }
}

void XanteMenu::parseDynamicData(QJsonObject menu)
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
                dynamicType = XanteMenu::FixedSize;
                dynamicCopies = value.toInt();
            } else if (value.type() == QJsonValue::Array) {
                QJsonArray copies = value.toArray();
                dynamicType = XanteMenu::FixedOptions;

                foreach(const QJsonValue &v, copies)
                    this->copies.append(v.toString());
            } else {
                QJsonObject origin = dynamic["origin"].toObject();

                dynamicType = XanteMenu::DynamicOptions;
                dynamicOriginBlock = origin["block"].toString();
                dynamicOriginItem = origin["item"].toString();
                dynamicBlockPrefix = dynamic["block_prefix"].toString();
            }
        } else
            type = XanteMenu::Type::Default;
    }
}

void XanteMenu::parseItems(QJsonObject menu)
{
    QJsonArray jitems = menu["items"].toArray();

    foreach(const QJsonValue &v, jitems) {
        XanteItem it(applicationName, name, v.toObject());
        items.append(it);
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
    : applicationName(applicationName)
{
    preLoad();
    parse(menu);
}

XanteMenu::XanteMenu(QString applicationName, QString name)
    : applicationName(applicationName), name(name)
{
    preLoad();
    setMode(XanteMode::XanteAccessEdit);
    setName(name);
    setType(XanteMenu::Type::Default);

    /* We always create an empty item for a new menu */
    XanteItem it(applicationName, name, QString("Item"));
    items.append(it);
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
        QString evValue = this->events.value(i.value(), QString(""));

        if (evValue.isEmpty() == false)
            events[i.key()] = evValue;
    }

    return events;
}

QJsonObject XanteMenu::writeDynamic(void) const
{
    QJsonArray optArray;
    QJsonObject origin, dynamic;

    switch (dynamicType) {
        case XanteMenu::DynamicType::FixedSize:
            dynamic["copies"] = dynamicCopies;
            break;

        case XanteMenu::DynamicType::FixedOptions:
            for (int i = 0; i < copies.size(); i++)
                optArray.append(copies.at(i));

            dynamic["copies"] = optArray;
            break;

        case XanteMenu::DynamicType::DynamicOptions:
            origin["block"] = dynamicOriginBlock;
            origin["item"] = dynamicOriginItem;
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

    foreach(const XanteItem item, items) {
        QJsonObject it;
        item.write(it);
        jitems.append(it);
    }

    root["mode"] = mode;
    root["name"] = name;
    root["object_id"] = objectId;
    root["type"] = typeDescription.value(type);

    if (hasEvents())
        root["events"] = writeEvents();

    if (type == XanteMenu::Type::Dynamic)
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

    japplication["version"] = version;
    japplication["revision"] = revision;
    japplication["build"] = build;
    japplication["beta"] = beta;

    root["jtf_revision"] = fileRevision;
    root["language"] = "en-us";
    root["application"] = japplication;
}

void XanteJTF::writeJtfGeneral(QJsonObject &root)
{
    root["name"] = applicationName;
    root["description"] = description;
    root["plugin"] = plugin;
    root["config_pathname"] = cfgPathname;
    root["log_pathname"] = logPathname;
    root["log_level"] = "info";
    root["company"] = company;
}

void XanteJTF::writeJtfUi(QJsonObject &root)
{
    QJsonArray jmenus;
    XanteMenu menu;

    foreach (const XanteMenu menu, menus) {
        QJsonObject m;
        menu.write(m);
        jmenus.append(m);
    }

    root["main_menu"] = mainMenu;
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
    jtfRoot = d.object();
    loadJtfFromFile();

    /* We're not empty anymore */
    empty = false;
}

void XanteJTF::loadJtfFromFile(void)
{
    loadJtfInternal();
    loadJtfGeneral();
    loadJtfUi();
}

void XanteJTF::loadJtfInternal(void)
{
    QJsonValue value = jtfRoot.value(QString("internal"));
    QJsonObject internal = value.toObject();

    fileRevision = internal["jtf_revision"].toInt();
    value = internal["application"];
    QJsonObject application = value.toObject();

    version = application["version"].toString();
    revision = application["revision"].toInt();
    build = application["build"].toInt();
    beta = application["beta"].toBool();
}

void XanteJTF::loadJtfGeneral(void)
{
    QJsonValue value = jtfRoot.value(QString("general"));
    QJsonObject general = value.toObject();

    applicationName = general["name"].toString();
    description = general["description"].toString();
    company = general["company"].toString();
    plugin = general["plugin"].toString();
    cfgPathname = general["config_pathname"].toString();
    logPathname = general["log_pathname"].toString();
}

void XanteJTF::loadJtfUi(void)
{
    QJsonValue value = jtfRoot.value(QString("ui"));
    QJsonObject ui = value.toObject();
    QJsonArray jmenus = ui["menus"].toArray();
    mainMenu = ui["main_menu"].toString();

    foreach(const QJsonValue &v, jmenus) {
        XanteMenu m(applicationName, v.toObject());
        menus.append(m);
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
    XanteMenu m(applicationName, QString("Main"));
    menus.append(m);
}

XanteMenu &XanteJTF::getMenu(QString objectId)
{
    int index = menus.indexOf(objectId);

    return menuAt(index);
}

XanteMenu &XanteJTF::menuAt(int index)
{
    if ((index < 0) || (index > menus.size()))
        throw std::out_of_range("Menu not found.");

    return menus[index];
}

void XanteJTF::clear(void)
{
    empty = true;
}

