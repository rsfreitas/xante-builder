
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Tue Apr  3 13:54:53 -03 2018
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

TabMixedform::TabMixedform(QWidget *parent)
    : QWidget(parent)
{
}

void TabMixedform::setSelectedItem(const XanteItem &item)
{
    Q_UNUSED(item);
}

void TabMixedform::updateSelectedItem(XanteItem &item)
{
    Q_UNUSED(item);
}

void TabMixedform::clearCurrentData(void)
{
}

void TabMixedform::prepareWidgets(int type)
{
    Q_UNUSED(type);
}

