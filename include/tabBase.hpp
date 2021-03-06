
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

#ifndef _TABBASE_HPP
#define _TABBASE_HPP          1

#include <xante/libxante.h>

class TabBase
{
    signals:
        virtual void dataChanged(void) = 0;

    public:
        /**
         * Clears all internal widgets.
         */
        virtual void clearCurrentData(void) = 0;

        /**
         * Enables/Disables internal widgets according an item type.
         */
        virtual void prepareWidgets(enum XanteItem::Type type) = 0;

        /**
         * Fills all internal widgets with @item data.
         */
        virtual void setSelectedItem(const XanteItem &item) = 0;

        /**
         * Fills @item with current widgets data.
         */
        virtual void updateSelectedItem(XanteItem &item) = 0;
};

#endif

