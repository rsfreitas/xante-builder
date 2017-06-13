
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jun  8 18:39:25 2017
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

#ifndef _XANTE_BUILDER_HPP
#define _XANTE_BUILDER_HPP          1

#define MAJOR_VERSION               0
#define MINOR_VERSION               1
#define RELEASE                     1

#define APP_NAME                    "xante-builder"

enum xante_mode {
    XANTE_ACCESS_HIDDEN             = 0,
    XANTE_ACCESS_VIEW               = (1 << 0),
    XANTE_ACCESS_EDIT               = (1 << 1)
};

/* Internal classes */
#include "XanteConfig.hpp"
#include "XanteProject.hpp"
#include "XanteJTF.hpp"
#include "XProjectWizard.hpp"
#include "XMainWindow.hpp"
#include "XMainDialog.hpp"
#include "XTreeView.hpp"
#include "XTreeModel.hpp"

#endif

