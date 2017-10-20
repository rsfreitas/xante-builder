
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
    m_pathname.sprintf("%s/.%s", getenv("HOME"), APP_NAME);
    m_cfgFilename.sprintf("%s.cfg", APP_NAME);

    readFile();
}

XanteConfig::~XanteConfig()
{
    writeFile();
}

void XanteConfig::loadDefaultValues(void)
{
    m_windowSize = QSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    m_windowPosition = QPoint(0, 0);
    m_recentOpenedFiles = QStringList("");
}

void XanteConfig::writeFile(void)
{
    QString filename = m_pathname + "/" + m_cfgFilename;
    QSettings settings(filename, QSettings::IniFormat);

    settings.beginGroup("Window");
    settings.setValue("windowSize", m_windowSize);
    settings.setValue("windowPosition", m_windowPosition);
    settings.endGroup();

    settings.beginGroup("RecentFiles");
    settings.setValue("files", m_recentOpenedFiles);
    settings.endGroup();
}

void XanteConfig::readFile(void)
{
    QString filename = m_pathname + "/" + m_cfgFilename;
    QFileInfo file(filename);

    if (file.exists() == false) {
        loadDefaultValues();
        return;
    }

    QSettings settings(filename, QSettings::IniFormat);

    settings.beginGroup("Window");
    m_windowSize = settings.value("windowSize").value<QSize>();
    m_windowPosition = settings.value("windowPosition").value<QPoint>();
    settings.endGroup();

    settings.beginGroup("RecentFiles");
    m_recentOpenedFiles = settings.value("files").value<QStringList>();
    settings.endGroup();
}

/*
 * Updates the internal list of recent opened files. If it is a new file, returns
 * true, otherwise returns false.
 */
bool XanteConfig::recentFile(const QString &filename)
{
    if (m_recentOpenedFiles.contains(filename))
        return false;

    m_recentOpenedFiles.prepend(filename);

    while (m_recentOpenedFiles.size() > MaxRecentFiles)
        m_recentOpenedFiles.removeLast();

    return true;
}

