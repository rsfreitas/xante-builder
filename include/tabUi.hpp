
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

#ifndef _TABUI_HPP
#define _TABUI_HPP          1

#include <QtWidgets>
#include <xante/libxante.h>
#include "tabBase.hpp"

class XanteBuilderConfig;

class TabUi : public QWidget, public TabBase
{
    Q_OBJECT

    public:
        TabUi(const XanteBuilderConfig &config, QWidget *parent = 0);
        ~TabUi() {}

        void setSelectedItem(const XanteItem &item);
        void updateSelectedItem(XanteItem &item);
        void clearCurrentData(void);
        void prepareWidgets(enum XanteItem::Type type);

    private slots:
        void addOption(void);
        void delOption(void);
        void contentChanged(const QString &value);

    signals:
        void dataChanged(void);

    private:
        QPushButton *btAdd, *btDel;
        QSpinBox *sbWidth, *sbHeight;
        QGroupBox *gGeometry, *gButtons, *gHelp;
        QCheckBox *cbExtraButton;
        QListWidget *lwOptions;
        QLineEdit *leTitle, *leBtnOk, *leBtnCancel, *leBtnExtra, *leBtnHelp,
                  *leDescription, *leBrief;

        const XanteBuilderConfig &config;

        QVBoxLayout *createExtraFeaturesWidgets(void);
        QGroupBox *createBtnLabelsWidgets(void);
        QGroupBox *createItemHelpWidgets(void);
        QGroupBox *createGeometryWidgets(void);
        void notifyChange(void);
};

#endif

