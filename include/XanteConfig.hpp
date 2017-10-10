
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

        void setWindowSize(QSize size) { window_size = size; }
        void setWindowPosition(QPoint point) { window_position = point; }
        QSize getWindowSize(void) { return window_size; }
        QPoint getWindowPosition(void) { return window_position; }
        const QString getRecentFile(int index) { return recent_opened_files.value(index); }
        int recentFilesSize(void) { return recent_opened_files.size(); }
        bool setRecentFile(const QString &filename);

    private:
        QString cfg_filename, pathname;
        QSize window_size;
        QPoint window_position;
        QStringList recent_opened_files;

        void loadDefaultValues(void);
        void readFile(void);
        void writeFile(void);
};

#endif

