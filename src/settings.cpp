
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Mon Apr  2 10:45:05 -03 2018
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

#include "xante_builder.hpp"

XanteBuilderConfig::XanteBuilderConfig(const QString &app_name)
{
    startWithDefaultValues();
    config = new XanteConfig(app_name, *this);
}

XanteBuilderConfig::~XanteBuilderConfig()
{
    delete config;
}

void XanteBuilderConfig::loadSettings(QSettings &settings)
{
    settings.beginGroup("System");
    m_sourceTemplatePath = settings.value("SourceTemplate").toString();
    m_jerminusPath = settings.value("Jerminus").toString();
    m_mandatoryFieldColor = settings.value("MandatoryFieldColor").toString();
    settings.endGroup();
}

void XanteBuilderConfig::writeSettings(QSettings &settings)
{
    settings.beginGroup("System");
    settings.setValue("SourceTemplate", m_sourceTemplatePath);
    settings.setValue("Jerminus", m_jerminusPath);
    settings.setValue("MandatoryFieldColor", m_mandatoryFieldColor);
    settings.endGroup();
}

void XanteBuilderConfig::startWithDefaultValues(void)
{
    m_sourceTemplatePath = QString("/usr/local/bin");
    m_jerminusPath = QString("/usr/local/bin");
    m_mandatoryFieldColor = QString("cyan");
}

