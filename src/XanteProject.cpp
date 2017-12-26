
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jun  8 23:50:04 2017
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

#include <memory>

#include "xante_builder.hpp"

#define DEFAULT_PROJECT_FILE_VERSION            1

/**
 * @name XanteProject
 * @brief The class constructor.
 *
 * This constructor creates a XanteProject object from a project file, i.e.,
 * an existing project.
 *
 * @param [in] filename: The project filename.
 */
XanteProject::XanteProject(QString filename)
{
    info = QFileInfo(filename);
    jtfFilename = loadProjectFile();

    if (jtf.load(jtfFilename) == false)
        throw std::runtime_error(QObject::tr("Unable to load JTF file").toLocal8Bit().data());
}

/**
 * @name XanteProject
 * @brief The class constructor
 *
 * This constructor creates a XanteProject from its bare informations. It's used
 * when a new project is created by the application.
 *
 * @param [in] projectName: The project name.
 * @param [in] path: The path where the project will be saved.
 * @param [in] jtf: The previously created JTF content.
 */
XanteProject::XanteProject(QString projectName, QString path, const XanteJTF &jtf)
    : jtf(jtf), projectName(projectName)
{
    projectRootPath = QDir(path + "/" + projectName);
    info = QFileInfo(projectRootPath.path() + "/" + projectName + ".pjx");
    jtfFilename = projectRootPath.path() + "/jtf/" + projectName + ".jtf";
    version = DEFAULT_PROJECT_FILE_VERSION;
}

QString XanteProject::loadProjectFile(void)
{
    QSettings settings(info.absoluteFilePath(), QSettings::IniFormat);
    QString pathname = info.absolutePath(), filename;

    settings.beginGroup("Project");
    projectName = settings.value("Name").value<QString>();
    version = settings.value("Version").value<int>();
    filename = pathname + "/jtf/" + settings.value("JTF").value<QString>();
    settings.endGroup();

    return filename;
}

void XanteProject::writeProjectFile(void)
{
    QSettings settings(info.absoluteFilePath(), QSettings::IniFormat);
    QFileInfo jtf(jtfFilename);

    settings.beginGroup("Project");
    settings.setValue("Name", projectName);
    settings.setValue("Version", version);
    settings.setValue("JTF", jtf.fileName());
    settings.endGroup();
}

QString XanteProject::getJtfFilename(void)
{
    return jtfFilename;
}

QString XanteProject::getProjectName(void)
{
    return projectName;
}

bool XanteProject::create(void)
{
    if (jtf.isEmpty())
        return false;

    if (projectRootPath.exists() == false)
        projectRootPath.mkpath(".");

    writeProjectFile();
    projectRootPath.mkdir("jtf");
    projectRootPath.mkdir("script");
    save();

    return true;
}

bool XanteProject::save(void)
{
    if (jtf.isEmpty())
        return false;

    if (jtf.save(jtfFilename) == false)
        return false;

    return true;
}

XanteJTF &XanteProject::getJtf(void)
{
    return jtf;
}

