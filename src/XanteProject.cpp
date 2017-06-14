
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

XanteProject::XanteProject(QString filename)
{
    info = QFileInfo(filename);
    load_project_file();
}

XanteProject::XanteProject(QString project_name, QString path, XanteJTF *jtf)
    : jtf(std::move(jtf)), project_name(project_name)
{
    project_root_path = QDir(path + "/" + project_name);
    info = QFileInfo(project_root_path.path() + "/" + project_name + ".pjx");
    jtf_filename = project_root_path.path() + "/jtf/" + project_name + ".jtf";
    version = DEFAULT_PROJECT_FILE_VERSION;
}

XanteProject::~XanteProject()
{
}

void XanteProject::load_project_file(void)
{
    QSettings settings(info.absoluteFilePath(), QSettings::IniFormat);
    QString pathname = info.absolutePath();

    settings.beginGroup("Project");
    project_name = settings.value("Name").value<QString>();
    version = settings.value("Version").value<int>();
    jtf_filename = pathname + "/jtf/" + settings.value("JTF").value<QString>();
    settings.endGroup();
}

void XanteProject::write_project_file(void)
{
    QSettings settings(info.absoluteFilePath(), QSettings::IniFormat);
    QFileInfo jtf(jtf_filename);

    settings.beginGroup("Project");
    settings.setValue("Name", project_name);
    settings.setValue("Version", version);
    settings.setValue("JTF", jtf.fileName());
    settings.endGroup();
}

QString XanteProject::get_jtf_filename(void)
{
    return jtf_filename;
}

QString XanteProject::get_project_name(void)
{
    return project_name;
}

bool XanteProject::create(void)
{
    if (nullptr == jtf)
        return false;

    if (project_root_path.exists() == false)
        project_root_path.mkpath(".");

    write_project_file();
    project_root_path.mkdir("jtf");
    project_root_path.mkdir("script");

    return true;
}

bool XanteProject::save(void)
{
    if (nullptr == jtf)
        return false;

    if (jtf->save(jtf_filename) == false)
        return false;

    return true;
}

const XanteJTF &XanteProject::get_jtf(void)
{
    return *jtf;
}

