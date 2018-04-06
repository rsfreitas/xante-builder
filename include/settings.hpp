
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Tue Apr  3 14:04:03 -03 2018
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

#ifndef _SETTINGS_HPP
#define _SETTINGS_HPP          1

class XanteBuilderConfig : public XanteConfigObject
{
    public:
        XanteBuilderConfig(const QString &app_name);
        ~XanteBuilderConfig();

        void loadSettings(QSettings &settings);
        void writeSettings(QSettings &settings);

        void sourceTemplatePath(QString &path) { m_sourceTemplatePath = path; }
        const QString sourceTemplatePath(void) const { return m_sourceTemplatePath; }

        void jerminusPath(QString &path) { m_jerminusPath = path; }
        const QString jerminusPath(void) const { return m_jerminusPath; }

        /* Default libxante's settings */
        void windowSize(QSize s) { config->windowSize(s); }
        QSize windowSize(void) const { return config->windowSize(); }

        void windowPosition(QPoint p) { config->windowPosition(p); }
        QPoint windowPosition(void) const { return config->windowPosition(); }

        const QString recentFile(int index) const { return config->recentFile(index); }
        bool recentFile(const QString &filename) { return config->recentFile(filename); }
        int totalRecentFiles(void) const { return config->totalRecentFiles(); }

    private:
        XanteConfig *config;
        QString m_sourceTemplatePath, m_jerminusPath;

        void startWithDefaultValues(void);
};

#endif

