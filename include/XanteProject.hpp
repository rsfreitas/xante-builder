
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Fri Jun  9 13:06:21 2017
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

#ifndef _XANTEPROJECT_HPP
#define _XANTEPROJECT_HPP          1

class XanteJTF;

/**
 * @name XanteProject
 * @brief A class to handle a project file.
 *
 * A project file is has information about a libxante application, where, this
 * application will be constituted of a JTF file, its translations, a script
 * to call the application and a template of its plugin.
 */
class XanteProject {
    public:
        XanteProject(QString filename);
        XanteProject(QString projectName, QString path, const XanteJTF &jtf);
        ~XanteProject();
        QString getJtfFilename(void);
        QString getProjectName(void);
        XanteJTF &getJtf(void);
        bool create(void);
        bool save(void);

    private:
        XanteJTF jtf;
        QFileInfo info;
        QDir projectRootPath;
        QString projectName, jtfFilename;
        int version;

        QString loadProjectFile(void);
        void writeProjectFile(void);
};

#endif

