
/*
 * Description: The main function.
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jun  8 18:14:14 2017
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

#include <QApplication>
#include <QCommandLineParser>

#include "xante_builder.hpp"

int main(int argc, char **argv)
{
    XanteConfig config;
    QApplication app(argc, argv);

    XMainWindow window(config);
    window.show();

    return app.exec();
}

