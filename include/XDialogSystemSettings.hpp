
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

#ifndef _XDIALOGSYSTEMSETTINGS_HPP
#define _XDIALOGSYSTEMSETTINGS_HPP              1

#include <QtWidgets>
#include <QDialog>

class XanteBuilderConfig;

class ColorListEditor : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor USER true)

    public:
        ColorListEditor(QWidget *parent) : QComboBox(parent) {
            populateList();
        }

        QColor color() const {
            return qvariant_cast<QColor>(itemData(currentIndex(),
                                                  Qt::DecorationRole));
        }

        void setColor(QColor color) {
            setCurrentIndex(findData(color, int(Qt::DecorationRole)));
        }

    private:
        void populateList() {
            QStringList colorNames = QColor::colorNames();

            for (int i = 0; i < colorNames.size(); i++) {
                QColor color(colorNames[i]);

                insertItem(i, colorNames[i]);
                setItemData(i, color, Qt::DecorationRole);
            }
        }
};

class XDialogSystemSettings : public QDialog
{
    Q_OBJECT

    public:
        XDialogSystemSettings(XanteBuilderConfig &config, QWidget *parent = 0);
        ~XDialogSystemSettings() {}

    private slots:
        void confirmOk(void);
        void confirmCancel(void);
        void chooseSourceTemplatePath(void);
        void chooseJerminusPath(void);

    private:
        QLineEdit *leSourceTemplatePath, *leJerminusPath;
        QComboBox *cbMandatoryFieldColor;
        XanteBuilderConfig &config;

        void createWidgets(void);
        QHBoxLayout *createButtons(void);
        QGroupBox *createUtilitiesWidgets(void);
};

#endif

