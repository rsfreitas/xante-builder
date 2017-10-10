
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Sat Jun 10 11:09:52 2017
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

#ifndef _XANTEJTF_HPP
#define _XANTEJTF_HPP   1

#include <QList>
#include <QJsonObject>
#include <QVector>
#include <QMap>

class QVariant;

enum XanteMode {
    XanteAccessHidden   = 0,
    XanteAccessView     = (1 << 0),
    XanteAccessEdit     = (1 << 1)
};

class XanteItem
{
    public:
        enum Type {
            Unknown = -1,
            Menu,
            InputInt,
            InputFloat,
            InputDate,
            InputTime,
            InputString,
            InputPasswd,
            Calendar,
            Timebox,
            RadioChecklist,
            Checklist,
            YesNo,
            DynamicMenu,
            DeleteDynamicMenu,
            AddDynamicMenu
        };

        enum Event {
            Selected,
            Exit,
            ValueConfirmed,
            ValueChanged,

            MaxEvents
        };

        enum OptionType {
            Fixed,
            List
        };

        XanteItem() {}
        XanteItem(QString applicationName, QString menuName, QJsonObject item);
        XanteItem(QString applicationName, QString menuName, QString name);

        void write(QJsonObject &root) const;

        /* Item properties */
        const QString &getName(void) const { return name; }
        const QString &getObjectId(void) const { return objectId; }
        const QString getBriefHelp(void) const { return briefHelp; }
        const QString getDescriptiveHelp(void) const { return descriptiveHelp; }
        const QString getConfigBlock(void) const { return configBlock; }
        const QString getConfigItem(void) const { return configItem; }
        enum XanteItem::OptionType getOptionType(void) const { return optionType; }
        int getTotalOptions(void) const { return options.size(); }
        int getTotalHelpOptions(void) const { return helpOptions.size(); }
        const QString getOption(void) const { return fixedOption; }
        const QString getOption(int index) const { return options.at(index); }
        const QString getHelpOption(int index) const { return helpOptions.at(index); }
        const QString getDefaultValue(void) const { return defaultValue; }
        const QString getReferencedMenu(void) const { return menuReferenceId; }
        const QVariant getMin(void) const { return minInputRange; }
        const QVariant getMax(void) const { return maxInputRange; }
        enum XanteItem::Type getType(void) const { return type; }
        enum XanteMode getMode(void) const { return mode; }
        int getStringLength(void) const { return stringLength; }
        const QString getEvent(enum XanteItem::Event event) const {
            QMap<enum XanteItem::Event, QString>::const_iterator it;

            for (it = events.constBegin(); it != events.constEnd(); it++)
                if (it.key() == event)
                    return it.value();

            return QString("");
        }

        void setName(QString name);
        void setMode(enum XanteMode mode) { this->mode = mode; }
        void setType(enum XanteItem::Type type) { this->type = type; }
        void setConfigBlock(QString configBlock) { this->configBlock = configBlock; }
        void setConfigItem(QString configItem) { this->configItem = configItem; }
        void setOption(QString option) {
            if ((type == XanteItem::Type::RadioChecklist) ||
                (type == XanteItem::Type::Checklist))
            {
                options.append(option);
                optionType = XanteItem::OptionType::List;
            } else {
                fixedOption = option;
                optionType = XanteItem::OptionType::Fixed;
            }
        }

        void setDefaultValue(QString value) { defaultValue = value; }
        void setEvent(QString event, enum XanteItem::Event eventId) {
            events[eventId] = event;
        }

        void setStringLength(int length) { stringLength = length; }
        void setBriefHelp(QString help) { briefHelp = help; }
        void setDescriptiveHelp(QString help) { descriptiveHelp = help; }

        bool hasEvents(void) const { return events.size() != 0; }
        bool hasOptions(void) const { return (type >= XanteItem::Type::InputInt) &&
                                              (type <= XanteItem::Type::DeleteDynamicMenu); }
        bool hasInputRanges(void) const { return (type >= XanteItem::Type::InputInt) &&
                                                   (type <= XanteItem::Type::InputPasswd); }
        bool hasConfig(void) const { return (type >= XanteItem::Type::InputInt) &&
                                             (type <= XanteItem::Type::YesNo); }

        bool hasHelp(void) const {
            return (briefHelp.isEmpty() == false) ||
                   (descriptiveHelp.isEmpty() == false) ||
                   (((type == XanteItem::Type::Checklist) ||
                     (type == XanteItem::Type::RadioChecklist)) &&
                    helpOptions.size() != 0);
        }

    private:
        QString applicationName, menuName, name, objectId, briefHelp,
                descriptiveHelp, configBlock, configItem, fixedOption,
                defaultValue, menuReferenceId;

        QVariant minInputRange, maxInputRange;
        QList<QString> options, helpOptions;
        QMap<enum XanteItem::Type, QString> typeDescription;
        QMap<enum XanteItem::Event, QString> events;
        enum XanteMode mode;
        enum XanteItem::Type type;
        enum XanteItem::OptionType optionType;
        int stringLength;

        void preLoad(void);
        void parse(QJsonObject item);
        void parseCommonData(QJsonObject item);
        void parseEventsData(QJsonObject item);
        void parseConfigData(QJsonObject item);
        void parseRangesData(QJsonObject item);
        void parseHelpData(QJsonObject item);

        void writeOptions(QJsonObject &root) const;
        QJsonObject writeInputRanges(void) const;
        QJsonObject writeConfig(void) const;
        QJsonObject writeEvents(void) const;
        QJsonObject writeHelp(void) const;

        enum XanteItem::Type toXanteItem(const QString &type);
};

class XanteMenu
{
    public:
        enum Type {
            Default,
            Dynamic
        };

        enum Event {
            Selected,
            Exit,

            MaxEvents
        };

        enum DynamicType {
            FixedSize,
            FixedOptions,
            DynamicOptions,

            MaxDynamicMenuType
        };

        XanteMenu() {}
        XanteMenu(QString applicationName, QJsonObject menu);
        XanteMenu(QString applicationName, QString name);
        XanteMenu(QString objectId) : objectId(objectId) {}

        void write(QJsonObject &root) const;
        int totalItems(void) { return items.size(); }
        XanteItem &itemAt(int index) { return items[index]; }

        /* Menu properties */
        const QString getName(void) const { return name; }
        const QString getObjectId(void) const { return objectId; }
        enum XanteMenu::Type getType(void) const { return type; }
        enum XanteMenu::DynamicType getDynamicType(void) const { return dynamicType; }
        enum XanteMode getMode(void) const { return mode; }
        int getDynamicCopies(void) const { return dynamicCopies; }
        const QStringList getDynamicOptions(void) const { return copies; }
        const QString getDynamicOriginBlock(void) const { return dynamicOriginBlock; }
        const QString getDynamicOriginItem(void) const { return dynamicOriginItem; }
        const QString getBlockPrefix(void) const { return dynamicBlockPrefix; }
        const QString getEvent(enum XanteMenu::Event event) const {
            QMap<enum XanteMenu::Event, QString>::const_iterator it;

            for (it = events.constBegin(); it != events.constEnd(); it++)
                if (it.key() == event)
                    return it.value();

            return QString("");
        }

        bool hasEvents(void) const { return events.size() != 0; }

        void setName(QString name);
        void setType(enum XanteMenu::Type type) { this->type = type; }
        void setMode(enum XanteMode mode) { this->mode = mode; }
        void setDynamic(int dynamicCopies) {
            this->dynamicCopies = dynamicCopies;
            dynamicType = XanteMenu::DynamicType::FixedSize;
        }

        void setDynamic(QString originBlock, QString originItem) {
            dynamicOriginBlock = originBlock;
            dynamicOriginItem = originItem;
            dynamicType = XanteMenu::DynamicType::DynamicOptions;
        }

        void setDynamic(QString copy) {
            copies.append(copy);
            dynamicType = XanteMenu::DynamicType::FixedOptions;
        }

        void setEvent(QString event, enum XanteMenu::Event eventId) {
            events.insert(eventId, event);
        }

        bool operator ==(const XanteMenu &other) const {
            return objectId.compare(other.getObjectId()) == 0;
        }

        bool operator ==(const QString &objectId) const {
            return this->objectId.compare(objectId) == 0;
        }

    private:
        QString applicationName, name, objectId, dynamicBlockPrefix,
                dynamicOriginBlock, dynamicOriginItem;

        enum XanteMode mode;
        enum XanteMenu::Type type;
        enum XanteMenu::DynamicType dynamicType;
        int dynamicCopies;
        QStringList copies;
        QList<XanteItem> items;
        QMap<enum XanteMenu::Type, QString> typeDescription;
        QMap<enum XanteMenu::Event, QString> events;

        void preLoad(void);
        void parse(QJsonObject menu);
        void parseCommonData(QJsonObject menu);
        void parseEventsData(QJsonObject menu);
        void parseDynamicData(QJsonObject menu);
        void parseItems(QJsonObject menu);

        QJsonObject writeEvents(void) const;
        QJsonObject writeDynamic(void) const;
};

class XanteJTF
{
    public:
        class Builder;
        XanteJTF() {}
        XanteJTF(const QString applicationName, const QString description,
                 const QString company, const QString plugin,
                 const QString cfgPathname, const QString logPathname,
                 const QString version, const int revision, const int build,
                 const bool beta)
            :
                applicationName(applicationName),
                description(description),
                company(company),
                plugin(plugin),
                cfgPathname(cfgPathname),
                logPathname(logPathname),
                version(version),
                revision(revision),
                build(build),
                beta(beta)
        {
            buildDefaultMenu();

            /*
             * Sets the main menu of a JTF as the first one inside our list.
             */
            setMainMenu(menus.at(0).getObjectId());

            /* We're not empty anymore */
            empty = false;
        }

        static QString objectIdCalc(QString applicationName,
                                      QString menuName,
                                      QString itemName = nullptr);

        bool save(QString filename);
        bool isEmpty(void) const { return empty; }
        void clear(void);
        void load(const QString &filename);
        void addMenu(XanteMenu menu) { menus.append(menu); }
        void setMainMenu(QString menuName) { mainMenu = menuName; }

        /* JTF information */
        const QString getMainMenu(void) { return mainMenu; }
        const QString getApplicationName(void) const { return applicationName; }
        const QString getDescription(void) const { return description; }
        const QString getCompany(void) const { return company; }
        const QString getPlugin(void) const { return plugin; }
        const QString getCfgPathname(void) const { return cfgPathname; }
        const QString getLogPathname(void) const { return logPathname; }
        const QString getVersion(void) const { return version; }
        int getRevision(void) const { return revision; }
        int getBuild(void) const { return build; }
        bool getBeta(void) const { return beta; }

        int totalMenus(void) const { return menus.size(); }
        XanteMenu &menuAt(int index);
        XanteMenu &getMenu(QString objectId);

    private:
        QString applicationName, description, company, plugin, cfgPathname,
                logPathname, version, mainMenu;

        int revision, build, fileRevision = 1;
        bool beta, empty = true;
        QList<XanteMenu> menus;
        QJsonObject jtfRoot;

        void writeJtfData(QJsonObject &root);
        void writeJtfUi(QJsonObject &root);
        void writeJtfGeneral(QJsonObject &root);
        void writeJtfInternal(QJsonObject &root);
        void loadJtfFromFile(void);
        void loadJtfInternal(void);
        void loadJtfGeneral(void);
        void loadJtfUi(void);
        void buildDefaultMenu(void);
};

class XanteJTF::Builder
{
    public:
        Builder()
            :
                version(QString("0.1")),
                revision(1),
                build_(1),
                beta(false){}

        Builder &setApplicationName(const QString applicationName) {
            this->applicationName = applicationName;
            return *this;
        }

        Builder &setDescription(const QString description) {
            this->description = description;
            return *this;
        }

        Builder &setCompany(const QString company) {
            this->company= company;
            return *this;
        }

        Builder &setPlugin(const QString plugin) {
            this->plugin = plugin;
            return *this;
        }

        Builder &setCfgPathname(const QString cfgPathname) {
            this->cfgPathname = cfgPathname;
            return *this;
        }

        Builder &setLogPathname(const QString logPathname) {
            this->logPathname = logPathname;
            return *this;
        }

        Builder &setVersion(const QString version) {
            this->version = version;
            return *this;
        }

        Builder &setRevision(const int revision) {
            this->revision = revision;
            return *this;
        }

        Builder &setBuild(const int build_) {
            this->build_ = build_;
            return *this;
        }

        Builder &setBeta(const bool beta) {
            this->beta = beta;
            return *this;
        }

        XanteJTF build() {
            return XanteJTF(this->applicationName, this->description,
                            this->company, this->plugin, this->cfgPathname,
                            this->logPathname, this->version,
                            this->revision, this->build_, this->beta);
        }

    private:
        QString applicationName, description, company, plugin, cfgPathname,
                logPathname, version;

        int revision, build_;
        bool beta;
};

#endif

