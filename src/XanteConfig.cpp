
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jun  8 18:58:47 2017
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

#include <QSettings>

#include "xante_builder.hpp"

#define DEFAULT_WINDOW_WIDTH            800
#define DEFAULT_WINDOW_HEIGHT           600

XanteConfig::XanteConfig()
{
    pathname.sprintf("%s/.%s", getenv("HOME"), APP_NAME);
    cfg_filename.sprintf("%s.cfg", APP_NAME);

    read_configuration_file();
}

XanteConfig::~XanteConfig()
{
    write_configuration_file();
}

void XanteConfig::create_configuration_directory(void)
{
    QDir dir(pathname);

    if (dir.exists() == false)
        dir.mkpath(".");
}

void XanteConfig::load_default_values(void)
{
    window_size = QSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    window_position = QPoint(0, 0);
    recent_opened_files = QStringList("");
}

void XanteConfig::write_configuration_file(void)
{
    QString filename = pathname + "/" + cfg_filename;
    QSettings settings(filename, QSettings::IniFormat);

    settings.beginGroup("Window");
    settings.setValue("window_size", window_size);
    settings.setValue("window_position", window_position);
    settings.endGroup();

    settings.beginGroup("RecentFiles");
    settings.setValue("files", recent_opened_files);
    settings.endGroup();
}

void XanteConfig::read_configuration_file(void)
{
    QString filename = pathname + "/" + cfg_filename;
    QFileInfo file(filename);

    if (file.exists() == false) {
        load_default_values();
        return;
    }

    QSettings settings(filename, QSettings::IniFormat);

    settings.beginGroup("Window");
    window_size = settings.value("window_size").value<QSize>();
    window_position = settings.value("window_position").value<QPoint>();
    settings.endGroup();

    settings.beginGroup("RecentFiles");
    recent_opened_files = settings.value("files").value<QStringList>();
    settings.endGroup();
}

/*
 * Updates the internal list of recent opened files. If it is a new file, returns
 * true, otherwise returns false.
 */
bool XanteConfig::set_recent_file(const QString &filename)
{
    if (recent_opened_files.contains(filename))
        return false;

    recent_opened_files.prepend(filename);

    while (recent_opened_files.size() > MaxRecentFiles)
        recent_opened_files.removeLast();

    return true;
}

