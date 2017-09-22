
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
        XanteItem(QString application_name, QString menu_name, QJsonObject item);
        XanteItem(QString application_name, QString menu_name, QString name);

        void write(QJsonObject &root) const;

        /* Item properties */
        const QString &get_name(void) const { return name; }
        const QString &get_object_id(void) const { return object_id; }
        const QString get_brief_help(void) const { return brief_help; }
        const QString get_descriptive_help(void) const { return descriptive_help; }
        const QString get_config_block(void) const { return config_block; }
        const QString get_config_item(void) const { return config_item; }
        enum XanteItem::OptionType get_option_type(void) const { return option_type; }
        int get_total_options(void) const { return options.size(); }
        const QString get_option(void) const { return fixed_option; }
        const QString get_option(int index) const { return options.at(index); }
        const QString get_default_value(void) const { return default_value; }
        const QString get_referenced_menu(void) const { return menu_reference_id; }
        const QVariant get_min(void) const { return min_input_range; }
        const QVariant get_max(void) const { return max_input_range; }
        enum XanteItem::Type get_type(void) const { return type; }
        enum XanteMode get_mode(void) const { return mode; }
        int get_string_length(void) const { return string_length; }
        const QString get_event(enum XanteItem::Event event) const {
            QMap<enum XanteItem::Event, QString>::const_iterator it;

            for (it = events.constBegin(); it != events.constEnd(); it++)
                if (it.key() == event)
                    return it.value();

            return QString("");
        }

        void set_name(QString name);
        void set_mode(enum XanteMode mode) { this->mode = mode; }
        void set_type(enum XanteItem::Type type) { this->type = type; }
        void set_config_block(QString config_block) { this->config_block = config_block; }
        void set_config_item(QString config_item) { this->config_item = config_item; }
        void set_option(QString option) {
            if ((type == XanteItem::Type::RadioChecklist) ||
                (type == XanteItem::Type::Checklist))
            {
                options.append(option);
                option_type = XanteItem::OptionType::List;
            } else {
                fixed_option = option;
                option_type = XanteItem::OptionType::Fixed;
            }
        }

        void set_default_value(QString value) { default_value = value; }
        void set_event(QString event, enum XanteItem::Event event_id) {
            events[event_id] = event;
        }

        void set_string_length(int length) { string_length = length; }
        void set_brief_help(QString help) { brief_help = help; }
        void set_descriptive_help(QString help) { descriptive_help = help; }

        bool has_events(void) const { return events.size() != 0; }
        bool has_options(void) const { return (type >= XanteItem::Type::InputInt) &&
                                              (type <= XanteItem::Type::DeleteDynamicMenu); }
        bool has_input_ranges(void) const { return (type >= XanteItem::Type::InputInt) &&
                                                   (type <= XanteItem::Type::InputPasswd); }
        bool has_config(void) const { return (type >= XanteItem::Type::InputInt) &&
                                             (type <= XanteItem::Type::YesNo); }

    private:
        QString application_name, menu_name, name, object_id, brief_help,
                descriptive_help, config_block, config_item, fixed_option,
                default_value, menu_reference_id;

        QVariant min_input_range, max_input_range;
        QList<QString> options, help_options;
        QMap<enum XanteItem::Type, QString> type_description;
        QMap<enum XanteItem::Event, QString> events;
        enum XanteMode mode;
        enum XanteItem::Type type;
        enum XanteItem::OptionType option_type;
        int string_length;

        void pre_load(void);
        void parse(QJsonObject item);
        void parse_common_data(QJsonObject item);
        void parse_events_data(QJsonObject item);
        void parse_config_data(QJsonObject item);
        void parse_ranges_data(QJsonObject item);
        void parse_help_data(QJsonObject item);

        void write_options(QJsonObject &root) const;
        void write_input_ranges(QJsonObject &input_ranges) const;
        void write_config(QJsonObject &config) const;
        void write_events(QJsonObject &events) const;
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
        XanteMenu(QString application_name, QJsonObject menu);
        XanteMenu(QString application_name, QString name);
        XanteMenu(QString object_id) : object_id(object_id) {}

        void write(QJsonObject &root) const;
        int total_items(void) { return items.size(); }
        XanteItem &item_at(int index) { return items[index]; }

        /* Menu properties */
        const QString get_name(void) const { return name; }
        const QString get_object_id(void) const { return object_id; }
        enum XanteMenu::Type get_type(void) const { return type; }
        enum XanteMenu::DynamicType get_dynamic_type(void) const { return dynamic_type; }
        enum XanteMode get_mode(void) const { return mode; }
        int get_dynamic_copies(void) const { return dynamic_copies; }
        const QStringList get_dynamic_options(void) const { return copies; }
        const QString get_dynamic_origin_block(void) const { return dynamic_origin_block; }
        const QString get_dynamic_origin_item(void) const { return dynamic_origin_item; }
        const QString get_block_prefix(void) const { return dynamic_block_prefix; }
        const QString get_event(enum XanteMenu::Event event) const {
            QMap<enum XanteMenu::Event, QString>::const_iterator it;

            for (it = events.constBegin(); it != events.constEnd(); it++)
                if (it.key() == event)
                    return it.value();

            return QString("");
        }

        bool has_events(void) const { return events.size() != 0; }

        void set_name(QString name);
        void set_type(enum XanteMenu::Type type) { this->type = type; }
        void set_mode(enum XanteMode mode) { this->mode = mode; }
        void set_dynamic(int dynamic_copies) {
            this->dynamic_copies = dynamic_copies;
            dynamic_type = XanteMenu::DynamicType::FixedSize;
        }

        void set_dynamic(QString origin_block, QString origin_item) {
            dynamic_origin_block = origin_block;
            dynamic_origin_item = origin_item;
            dynamic_type = XanteMenu::DynamicType::DynamicOptions;
        }

        void set_dynamic(QString copy) {
            copies.append(copy);
            dynamic_type = XanteMenu::DynamicType::FixedOptions;
        }

        void set_event(QString event, enum XanteMenu::Event event_id) {
            events.insert(event_id, event);
        }

        bool operator ==(const XanteMenu &other) const {
            return object_id.compare(other.get_object_id()) == 0;
        }

        bool operator ==(const QString &object_id) const {
            return this->object_id.compare(object_id) == 0;
        }

    private:
        QString application_name, name, object_id, dynamic_block_prefix,
                dynamic_origin_block, dynamic_origin_item;

        enum XanteMode mode;
        enum XanteMenu::Type type;
        enum XanteMenu::DynamicType dynamic_type;
        int dynamic_copies;
        QStringList copies;
        QList<XanteItem> items;
        QMap<enum XanteMenu::Type, QString> type_description;
        QMap<enum XanteMenu::Event, QString> events;

        void pre_load(void);
        void parse(QJsonObject menu);
        void parse_common_data(QJsonObject menu);
        void parse_events_data(QJsonObject menu);
        void parse_dynamic_data(QJsonObject menu);
        void parse_items(QJsonObject menu);
        void write_events(QJsonObject &events) const;
        void write_dynamic(QJsonObject &dynamic) const;
};

class XanteJTF
{
    public:
        class Builder;
        XanteJTF() {}
        XanteJTF(const QString application_name, const QString description,
                 const QString company, const QString plugin,
                 const QString cfg_pathname, const QString log_pathname,
                 const QString version, const int revision, const int build,
                 const bool beta)
            :
                application_name(application_name),
                description(description),
                company(company),
                plugin(plugin),
                cfg_pathname(cfg_pathname),
                log_pathname(log_pathname),
                version(version),
                revision(revision),
                build(build),
                beta(beta)
        {
            build_default_menu();

            /*
             * Sets the main menu of a JTF as the first one inside our list.
             */
            set_main_menu(menus.at(0).get_object_id());

            /* We're not empty anymore */
            empty = false;
        }

        static QString object_id_calc(QString application_name,
                                      QString menu_name,
                                      QString item_name = nullptr);

        bool save(QString filename);
        bool is_empty(void) const { return empty; }
        void clear(void);
        void load(const QString &filename);
        void add_menu(XanteMenu menu) { menus.append(menu); }
        void set_main_menu(QString menu_name) { main_menu = menu_name; }

        /* JTF information */
        const QString get_main_menu(void) { return main_menu; }
        const QString get_application_name(void) const { return application_name; }
        const QString get_description(void) const { return description; }
        const QString get_company(void) const { return company; }
        const QString get_plugin(void) const { return plugin; }
        const QString get_cfg_pathname(void) const { return cfg_pathname; }
        const QString get_log_pathname(void) const { return log_pathname; }
        const QString get_version(void) const { return version; }
        int get_revision(void) const { return revision; }
        int get_build(void) const { return build; }
        bool get_beta(void) const { return beta; }

        int total_menus(void) const { return menus.size(); }
        XanteMenu &menu_at(int index);
        XanteMenu &get_menu(QString object_id);

    private:
        QString application_name, description, company, plugin, cfg_pathname,
                log_pathname, version;

        int revision, build;
        bool beta, empty = true;
        QList<XanteMenu> menus;
        QString main_menu;
        int file_revision = 1;
        QJsonObject jtf_root;

        void write_jtf_data(QJsonObject &root);
        void write_jtf_ui(QJsonObject &root);
        void write_jtf_general(QJsonObject &root);
        void write_jtf_internal(QJsonObject &root);
        void load_jtf_from_file(void);
        void load_jtf_internal(void);
        void load_jtf_general(void);
        void load_jtf_ui(void);
        void build_default_menu(void);
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

        Builder &set_application_name(const QString application_name) {
            this->application_name = application_name;
            return *this;
        }

        Builder &set_description(const QString description) {
            this->description = description;
            return *this;
        }

        Builder &set_company(const QString company) {
            this->company= company;
            return *this;
        }

        Builder &set_plugin(const QString plugin) {
            this->plugin = plugin;
            return *this;
        }

        Builder &set_cfg_pathname(const QString cfg_pathname) {
            this->cfg_pathname = cfg_pathname;
            return *this;
        }

        Builder &set_log_pathname(const QString log_pathname) {
            this->log_pathname = log_pathname;
            return *this;
        }

        Builder &set_version(const QString version) {
            this->version = version;
            return *this;
        }

        Builder &set_revision(const int revision) {
            this->revision = revision;
            return *this;
        }

        Builder &set_build(const int build_) {
            this->build_ = build_;
            return *this;
        }

        Builder &set_beta(const bool beta) {
            this->beta = beta;
            return *this;
        }

        XanteJTF build() {
            return XanteJTF(this->application_name, this->description,
                                this->company, this->plugin, this->cfg_pathname,
                                this->log_pathname, this->version,
                                this->revision, this->build_, this->beta);
        }

    private:
        QString application_name, description, company, plugin, cfg_pathname,
                log_pathname, version;

        int revision, build_;
        bool beta;
};

#endif

