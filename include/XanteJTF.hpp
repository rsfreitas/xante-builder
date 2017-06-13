
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
#define _XANTEJTF_HPP          1

#include <QList>
#include <QJsonObject>

class XanteItem
{
    public:
        XanteItem();
        ~XanteItem();
        void write(QJsonObject &root) const;

    private:
        QString name, object_id;
        enum xante_mode mode;
};

class XanteMenu
{
    public:
        XanteMenu();
        ~XanteMenu();
        void write(QJsonObject &root) const;

    private:
        QList<XanteItem> items;
        QString name, object_id;
        enum xante_mode mode;
};

class XanteJTF
{
    public:
        class Builder;
        XanteJTF(const QString application_name, const QString description,
                 const QString company, const QString plugin,
                 const QString cfg_pathname, const QString log_pathname,
                 const QString version, const int revision, const int build,
                 const bool beta) :
            application_name(application_name), description(description),
            company(company), plugin(plugin), cfg_pathname(cfg_pathname),
            log_pathname(log_pathname), version(version), revision(revision),
            build(build), beta(beta) {}

        bool save(QString filename);
        void add_menu(XanteMenu menu);
        void set_main_menu(QString menu_name);

        /* JTF main information */
        const QString get_application_name() { return application_name; }
        const QString get_description() { return description; }
        const QString get_company() { return company; }
        const QString get_plugin() { return plugin; }
        const QString get_cfg_pathname() { return cfg_pathname; }
        const QString get_log_pathname() { return log_pathname; }
        const QString get_version() { return version; }
        int get_revision() { return revision; }
        int get_build() { return build; }
        bool get_beta() { return beta; }

    private:
        const QString application_name, description, company, plugin,
                      cfg_pathname, log_pathname, version;

        const int revision, build;
        const bool beta;
        QList<XanteMenu> menus;
        QString main_menu;
        int file_revision = 1;

        void write_jtf_data(QJsonObject &root);
        void write_jtf_ui(QJsonObject &root);
        void write_jtf_general(QJsonObject &root);
        void write_jtf_internal(QJsonObject &root);
};

class XanteJTF::Builder
{
    private:
        QString application_name, description, company, plugin, cfg_pathname,
                log_pathname, version;

        int revision, build_;
        bool beta;

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

        XanteJTF *build() {
            return new XanteJTF(this->application_name, this->description,
                                this->company, this->plugin, this->cfg_pathname,
                                this->log_pathname, this->version,
                                this->revision, this->build_, this->beta);
        }
};

#endif

