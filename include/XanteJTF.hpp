
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
            AddDynamicMenu,

            MaxTypes
        };

        enum Event {
            Selected,
            Exit,
            ValueConfirmed,
            ValueChanged,

            MaxEvents
        };

        XanteItem() {}
        XanteItem(QString applicationName, QString menuName, QJsonObject item);
        XanteItem(QString applicationName, QString menuName, QString name);

        void write(QJsonObject &root) const;
        bool hasEvents(void) const { return m_events.size() != 0; }
        bool hasOptions(void) const { return (m_type >= XanteItem::Type::InputInt) &&
                                             (m_type <= XanteItem::Type::DeleteDynamicMenu); }

        bool hasInputRanges(void) const { return (m_type >= XanteItem::Type::InputInt) &&
                                                 (m_type <= XanteItem::Type::InputPasswd); }

        bool hasConfig(void) const { return (m_type >= XanteItem::Type::InputInt) &&
                                            (m_type <= XanteItem::Type::YesNo); }

        bool hasHelp(void) const {
            return (m_briefHelp.isEmpty() == false) ||
                   (m_descriptiveHelp.isEmpty() == false) ||
                   (((m_type == XanteItem::Type::Checklist) ||
                     (m_type == XanteItem::Type::RadioChecklist)) &&
                    m_helpOptions.size() != 0);
        }

        bool operator ==(const XanteItem &other) const {
            return (m_applicationName == other.m_applicationName) &&
                (m_menuName == other.m_menuName) &&
                (m_name == other.m_name) &&
                (m_objectId == other.m_objectId) &&
                (m_briefHelp == other.m_briefHelp) &&
                (m_descriptiveHelp == other.m_descriptiveHelp) &&
                (m_configBlock == other.m_configBlock) &&
                (m_configItem == other.m_configItem) &&
                (m_fixedOption == other.m_fixedOption) &&
                (m_defaultValue == other.m_defaultValue) &&
                (m_menuReferenceId == other.m_menuReferenceId) &&
                (m_minInputRange == other.m_minInputRange) &&
                (m_maxInputRange == other.m_maxInputRange) &&
                (m_options == other.m_options) &&
                (m_helpOptions == other.m_helpOptions) &&
                (m_events == other.m_events) &&
                (m_mode == other.m_mode) &&
                (m_type == other.m_type) &&
                (m_stringLength == other.m_stringLength);
        }

        bool operator !=(const XanteItem &other) const {
            return !(*this == other);
        }

        XanteItem &operator =(const XanteItem &other) {
            if (&other == this)
                return *this;

            m_applicationName = other.m_applicationName;
            m_menuName = other.m_menuName;
            m_name = other.m_name;
            m_objectId = other.m_objectId;
            m_briefHelp = other.m_briefHelp;
            m_descriptiveHelp = other.m_descriptiveHelp;
            m_configBlock = other.m_configBlock;
            m_configItem = other.m_configItem;
            m_fixedOption = other.m_fixedOption;
            m_defaultValue = other.m_defaultValue;
            m_menuReferenceId = other.m_menuReferenceId;
            m_minInputRange = other.m_minInputRange;
            m_maxInputRange = other.m_maxInputRange;
            m_options = other.m_options;
            m_helpOptions = other.m_helpOptions;
            m_events = other.m_events;
            m_mode = other.m_mode;
            m_type = other.m_type;
            m_stringLength = other.m_stringLength;
            m_typeDescription = other.m_typeDescription;

            return *this;
        }

        QString debug(void) const {
            QString info = QString(
                    "{applicationName: '%1', menuName: '%2', name: '%3', "
                    "objectId: '%4', briefHelp: '%5', descriptiveHelp: '%6', "
                    "configBlock: '%7', configItem: '%8', fixedOption: '%9', "
                    "defaultValue: '%10', menuReferenceId: '%11', mode: '%12', "
                    "type: '%13', min: '%14', max: '%15', stringLength: '%16'")
                .arg(m_applicationName)
                .arg(m_menuName)
                .arg(m_name)
                .arg(m_objectId)
                .arg(m_briefHelp)
                .arg(m_descriptiveHelp)
                .arg(m_configBlock)
                .arg(m_configItem)
                .arg(m_fixedOption)
                .arg(m_defaultValue)
                .arg(m_menuReferenceId)
                .arg(m_mode)
                .arg(m_type)
                .arg((m_type == XanteItem::Type::InputFloat) ? m_minInputRange.toFloat()
                                                             : m_minInputRange.toInt())
                .arg((m_type == XanteItem::Type::InputFloat) ? m_maxInputRange.toFloat()
                                                             : m_maxInputRange.toInt())
                .arg(m_stringLength);

            if (m_events.size() > 0) {
                QMap<enum XanteItem::Event, QString>::const_iterator it;
                info += QString(", events: {");

                for (it = m_events.constBegin(); it != m_events.constEnd(); it++)
                    info += QString("%1: '%2' ").arg(it.key()).arg(it.value());

                info += QString("}");
            }

            if (m_options.size() > 0) {
                info += QString(", options: [");

                for (int i = 0; i < m_options.size(); i++)
                    info += QString("'%1' ").arg(m_options.at(i));

                info += QString("]");
            }

            if (m_helpOptions.size() > 0) {
                info += QString(", helpOptions: [");

                for (int i = 0; i < m_helpOptions.size(); i++)
                    info += QString("'%1' ").arg(m_helpOptions.at(i));

                info += QString("]");
            }

            info += QString("}");

            return info;
        }

        /* Item properties */
        const QString name(void) const { return m_name; }
        void name(QString name);

        const QString objectId(void) const { return m_objectId; }
        void objectId(QString objectId) { m_objectId = objectId; }

        const QString briefHelp(void) const { return m_briefHelp; }
        void briefHelp(QString briefHelp) { m_briefHelp = briefHelp; }

        const QString descriptiveHelp(void) const { return m_descriptiveHelp; }
        void descriptiveHelp(QString descriptiveHelp) { m_descriptiveHelp = descriptiveHelp; }

        const QString configBlock(void) const { return m_configBlock; }
        void configBlock(QString configBlock) { m_configBlock = configBlock; }

        const QString configItem(void) const { return m_configItem; }
        void configItem(QString configItem) { m_configItem = configItem; }

        enum XanteItem::Type type(void) const { return m_type; }
        void type(enum XanteItem::Type type) { m_type = type; }

        enum XanteMode mode(void) const { return m_mode; }
        void mode(enum XanteMode mode) { m_mode = mode; }

        int stringLength(void) const { return m_stringLength; }
        void stringLength(int stringLength) { m_stringLength = stringLength; }

        template <typename T>
        void minMax(T min, T max) {
            m_minInputRange = QVariant(min);
            m_maxInputRange = QVariant(max);
        }

        const QPair<QVariant, QVariant> minMax(void) const {
            return qMakePair(m_minInputRange, m_maxInputRange);
        }

        const QString defaultValue(void) const { return m_defaultValue; }
        void defaultValue(QString value) { m_defaultValue = value; }

        const QString referencedMenu(void) const { return m_menuReferenceId; }
        void referencedMenu(QString referencedMenu) { m_menuReferenceId = referencedMenu; }

        const QString event(enum XanteItem::Event event) const {
            QMap<enum XanteItem::Event, QString>::const_iterator it;

            for (it = m_events.constBegin(); it != m_events.constEnd(); it++)
                if (it.key() == event)
                    return it.value();

            return QString("");
        }

        void event(QString event, enum XanteItem::Event eventId) {
            m_events[eventId] = event;
        }

        int totalOptions(void) const { return m_options.size(); }
        const QString option(void) const { return m_fixedOption; }
        const QString option(int index) const { return m_options.at(index); }
        void option(QString option) {
            if ((m_type == XanteItem::Type::RadioChecklist) ||
                (m_type == XanteItem::Type::Checklist))
            {
                if (m_options.contains(option) == false)
                    m_options.append(option);
            } else
                m_fixedOption = option;
        }

        int totalHelpOptions(void) const { return m_helpOptions.size(); }
        const QString helpOption(int index) const { return m_helpOptions.at(index); }
        void helpOption(QString help) {
            if (m_helpOptions.contains(help) == false)
                m_helpOptions.append(help);
        }

    private:
        QString m_applicationName, m_menuName, m_name, m_objectId, m_briefHelp,
                m_descriptiveHelp, m_configBlock, m_configItem, m_fixedOption,
                m_defaultValue, m_menuReferenceId;

        QVariant m_minInputRange, m_maxInputRange;
        QList<QString> m_options, m_helpOptions;
        QMap<enum XanteItem::Type, QString> m_typeDescription;
        QMap<enum XanteItem::Event, QString> m_events;
        enum XanteMode m_mode;
        enum XanteItem::Type m_type;
        int m_stringLength = 0;

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
        XanteMenu(QString objectId) : m_objectId(objectId) {}

        int totalItems(void) { return m_items.size(); }
        XanteItem &itemAt(int index) { return m_items[index]; }
        void itemMove(int from, int to) { m_items.move(from, to); }
        void addItem(XanteItem item) { m_items.append(item); }
        void removeItem(int index) { m_items.removeAt(index); }

        void write(QJsonObject &root) const;
        bool hasEvents(void) const { return m_events.size() != 0; }

        /*
         * The purpose of this function is to be used by the method indexOf of
         * a QList<XanteMenu>.
         *
         * We search for a XanteMenu only by its objectId attribute.
         */
        bool operator ==(const XanteMenu &other) const {
            return m_objectId.compare(other.m_objectId) == 0;
        }

        /*
         * This one is actually used to compare two XanteMenu objects.
         */
        bool operator !=(const XanteMenu &other) const {
            return (m_applicationName != other.m_applicationName) ||
                (m_name != other.m_name) ||
                (m_objectId != other.m_objectId) ||
                (m_dynamicBlockPrefix != other.m_dynamicBlockPrefix) ||
                (m_dynamicOriginBlock != other.m_dynamicOriginBlock) ||
                (m_dynamicOriginItem != other.m_dynamicOriginItem) ||
                (m_mode != other.m_mode) ||
                (m_type != other.m_type) ||
                (m_dynamicType != other.m_dynamicType) ||
                (m_dynamicCopies != other.m_dynamicCopies) ||
                (m_copies != other.m_copies) ||
                (m_events != other.m_events);
        }

        XanteMenu &operator =(const XanteMenu &other) {
            if (&other == this)
                return *this;

            m_applicationName = other.m_applicationName;
            m_name = other.m_name;
            m_objectId = other.m_objectId;
            m_dynamicBlockPrefix = other.m_dynamicBlockPrefix;
            m_dynamicOriginBlock = other.m_dynamicOriginBlock;
            m_dynamicOriginItem = other.m_dynamicOriginItem;

            m_mode = other.m_mode;
            m_type = other.m_type;
            m_dynamicType = other.m_dynamicType;
            m_dynamicCopies = other.m_dynamicCopies;
            m_copies = other.m_copies;
            m_typeDescription = other.m_typeDescription;
            m_events = other.m_events;

            m_items = other.m_items;

            return *this;
        }

        QString debug(bool printItems) const {
            QString info = QString("{applicationName: '%1', "
                    "name: '%2', objectId: '%3', dynamicBlockPrefix: '%4', "
                    "dynamicOriginBlock: '%5', dynamicOriginItem: '%6', "
                    "mode: '%7', type: '%8', dynamicType: '%9', "
                    "dynamicCopies: '%10'")
                .arg(m_applicationName)
                .arg(m_name)
                .arg(m_objectId)
                .arg(m_dynamicBlockPrefix)
                .arg(m_dynamicOriginBlock)
                .arg(m_dynamicOriginItem)
                .arg(m_mode)
                .arg(m_type)
                .arg(m_dynamicType)
                .arg(m_dynamicCopies);

            if (m_events.size() > 0) {
                QMap<enum XanteMenu::Event, QString>::const_iterator it;
                info += QString(", events: {");

                for (it = m_events.constBegin(); it != m_events.constEnd(); it++)
                    info += QString("%1: '%2' ").arg(it.key()).arg(it.value());

                info += QString("}");
            }

            if (printItems && (m_items.size() > 0)) {
                info += QString(", items: [");

                for (int i = 0; i < m_items.size(); i++) {
                    XanteItem it = m_items.at(i);
                    info += it.debug();
                }

                info += QString("]");
            } else
                info += QString("}");

            return info;
        }

        /* Menu properties */
        const QString name(void) const { return m_name; }
        void name(QString name);

        const QString objectId(void) const { return m_objectId; }
        void objectId(QString objectId) { m_objectId = objectId; }

        enum XanteMenu::Type type(void) const { return m_type; }
        void type(enum XanteMenu::Type type) { m_type = type; }

        enum XanteMenu::DynamicType dynamicType(void) const { return m_dynamicType; }
        void dynamicType(enum XanteMenu::DynamicType dynamicType) { m_dynamicType = dynamicType; }

        enum XanteMode mode(void) const { return m_mode; }
        void mode(enum XanteMode mode) { m_mode = mode; }

        const QString event(enum XanteMenu::Event event) const {
            QMap<enum XanteMenu::Event, QString>::const_iterator it;

            for (it = m_events.constBegin(); it != m_events.constEnd(); it++)
                if (it.key() == event)
                    return it.value();

            return QString("");
        }

        void event(QString event, enum XanteMenu::Event eventId) {
            m_events.insert(eventId, event);
        }

        int dynamicCopies(void) const { return m_dynamicCopies; }
        void dynamicCopies(int dynamicCopies) {
            m_dynamicCopies = dynamicCopies;
            m_dynamicType = XanteMenu::DynamicType::FixedSize;
        }

        const QStringList dynamicOptions(void) const { return m_copies; }
        void dynamicOptions(QString copy) {
            if (m_copies.contains(copy) == false)
                m_copies.append(copy);

            m_dynamicType = XanteMenu::DynamicType::FixedOptions;
        }

        const QPair<QString, QPair<QString, QString>> dynamicOrigin(void) {
            return qMakePair(m_dynamicBlockPrefix,
                             qMakePair(m_dynamicOriginBlock,
                                       m_dynamicOriginItem));
        }

        void dynamicOrigin(QString blockPrefix, QString originBlock, QString originItem) {
            m_dynamicBlockPrefix = blockPrefix;
            m_dynamicOriginBlock = originBlock;
            m_dynamicOriginItem = originItem;
            m_dynamicType = XanteMenu::DynamicType::DynamicOptions;
        }

    private:
        QString m_applicationName, m_name, m_objectId, m_dynamicBlockPrefix,
                m_dynamicOriginBlock, m_dynamicOriginItem;

        enum XanteMode m_mode;
        enum XanteMenu::Type m_type;
        enum XanteMenu::DynamicType m_dynamicType = XanteMenu::DynamicType::FixedSize;
        int m_dynamicCopies = 0;
        QStringList m_copies;
        QList<XanteItem> m_items;
        QMap<enum XanteMenu::Type, QString> m_typeDescription;
        QMap<enum XanteMenu::Event, QString> m_events;

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
                m_applicationName(applicationName),
                m_description(description),
                m_company(company),
                m_plugin(plugin),
                m_cfgPathname(cfgPathname),
                m_logPathname(logPathname),
                m_version(version),
                m_revision(revision),
                m_build(build),
                m_beta(beta)
        {
            buildDefaultMenu();

            /*
             * Sets the main menu of a JTF as the first one inside our list.
             */
            mainMenu(m_menus.at(0).objectId());

            /* We're not empty anymore */
            m_empty = false;
        }

        static QString objectIdCalc(QString applicationName,
                                    QString menuName,
                                    QString itemName = nullptr);

        /* Operations */
        bool isEmpty(void) const { return m_empty; }
        bool save(QString filename);
        bool load(const QString &filename);
        void clear(void);

        /* JTF information */
        const QString mainMenu(void) const { return m_mainMenu; }
        void mainMenu(QString mainMenu) { m_mainMenu = mainMenu; }

        const QString applicationName(void) const { return m_applicationName; }
        void applicationName(QString applicationName) { m_applicationName = applicationName; }

        const QString description(void) const { return m_description; }
        void description(QString description) { m_description = description; }

        const QString company(void) const { return m_company; }
        void company(QString company) { m_company = company; }

        const QString plugin(void) const { return m_plugin; }
        void plugin(QString plugin) { m_plugin = plugin; }

        const QString cfgPathname(void) const { return m_cfgPathname; }
        void cfgPathname(QString cfgPathname) { m_cfgPathname = cfgPathname; }

        const QString logPathname(void) const { return m_logPathname; }
        void logPathname(QString logPathname) { m_logPathname = logPathname; }

        const QString version(void) const { return m_version; }
        void version(QString version) { m_version = version; }

        int revision(void) const { return m_revision; }
        void revision(int revision) { m_revision = revision; }

        int build(void) const { return m_build; }
        void build(int build) { m_build = build; }

        bool beta(void) const { return m_beta; }
        void beta(bool beta) { m_beta = beta; }

        XanteMenu &menuAt(int index);
        XanteMenu &getMenu(QString objectId);
        int totalMenus(void) const { return m_menus.size(); }
        void addMenu(XanteMenu menu) { m_menus.append(menu); }
        void removeMenu(int index) { m_menus.removeAt(index); }

    private:
        QString m_applicationName, m_description, m_company, m_plugin,
                m_cfgPathname, m_logPathname, m_version, m_mainMenu;

        int m_revision, m_build, m_fileRevision = 1;
        bool m_beta, m_empty = true;
        QList<XanteMenu> m_menus;
        QJsonObject m_jtfRoot;

        void writeJtfData(QJsonObject &root);
        void writeJtfUi(QJsonObject &root);
        void writeJtfGeneral(QJsonObject &root);
        void writeJtfInternal(QJsonObject &root);
        bool loadJtfFromFile(void);
        bool loadJtfInternal(void);
        bool loadJtfGeneral(void);
        bool loadJtfUi(void);
        void buildDefaultMenu(void);
};

class XanteJTF::Builder
{
    public:
        Builder()
            :
                m_version(QString("0.1")),
                m_revision(1),
                m_build(1),
                m_beta(false){}

        Builder &setApplicationName(const QString applicationName) {
            m_applicationName = applicationName;
            return *this;
        }

        Builder &setDescription(const QString description) {
            m_description = description;
            return *this;
        }

        Builder &setCompany(const QString company) {
            m_company= company;
            return *this;
        }

        Builder &setPlugin(const QString plugin) {
            m_plugin = plugin;
            return *this;
        }

        Builder &setCfgPathname(const QString cfgPathname) {
            m_cfgPathname = cfgPathname;
            return *this;
        }

        Builder &setLogPathname(const QString logPathname) {
            m_logPathname = logPathname;
            return *this;
        }

        Builder &setVersion(const QString version) {
            m_version = version;
            return *this;
        }

        Builder &setRevision(const int revision) {
            m_revision = revision;
            return *this;
        }

        Builder &setBuild(const int build_) {
            m_build = build_;
            return *this;
        }

        Builder &setBeta(const bool beta) {
            m_beta = beta;
            return *this;
        }

        XanteJTF build() {
            return XanteJTF(m_applicationName, m_description,
                            m_company, m_plugin, m_cfgPathname,
                            m_logPathname, m_version,
                            m_revision, m_build, m_beta);
        }

    private:
        QString m_applicationName, m_description, m_company, m_plugin,
                m_cfgPathname, m_logPathname, m_version;

        int m_revision, m_build;
        bool m_beta;
};

#endif

