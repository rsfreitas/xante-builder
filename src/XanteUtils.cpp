
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Fri Oct 27 19:14:06 2017
 * Project: XanteUtils.cpp
 *
 * Copyright (C) 2017 Rodrigo Freitas
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#include "xante_builder.hpp"

namespace XanteUtils
{
    static QMap<QString, QPair<enum XanteItem::Type, QPixmap>> __appIcons;

    void loadIcons(void)
    {
        for (int i = XanteItem::Type::Menu; i < XanteItem::Type::MaxTypes; i++) {
//            __appIcons.insert(
        }
    }
}

