
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jun  8 18:59:59 2017
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

#ifndef _XANTECONFIG_HPP
#define _XANTECONFIG_HPP          1

#include <QList>
#include <QtGui>

/**
 * @name XanteConfig
 * @brief A class to control the application configuration file.
 */
class XanteConfig
{
    public:
        XanteConfig();
        ~XanteConfig();

        void windowSize(QSize size) { m_windowSize = size; }
        QSize windowSize(void) { return m_windowSize; }
        void windowPosition(QPoint point) { m_windowPosition = point; }
        QPoint windowPosition(void) { return m_windowPosition; }
        const QString recentFile(int index) { return m_recentOpenedFiles.value(index); }
        bool recentFile(const QString &filename);
        int recentFilesSize(void) { return m_recentOpenedFiles.size(); }

    private:
        QString m_cfgFilename, m_pathname;
        QSize m_windowSize;
        QPoint m_windowPosition;
        QStringList m_recentOpenedFiles;

        void loadDefaultValues(void);
        void readFile(void);
        void writeFile(void);
};

#endif

