
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

#ifndef _TABCONTENT_HPP
#define _TABCONTENT_HPP          1

#include <QtWidgets>
#include <xante/libxante.h>
#include "tabBase.hpp"

class XanteBuilderConfig;

class TabContent : public QWidget, public TabBase
{
    Q_OBJECT

    public:
        TabContent(const XanteBuilderConfig &config, QWidget *parent = 0);
        ~TabContent() {}

        void setSelectedItem(const XanteItem &item);
        void updateSelectedItem(XanteItem &item);
        void clearCurrentData(void);
        void prepareWidgets(enum XanteItem::Type type);

    private slots:
        void addOption(void);
        void delOption(void);
        void setDefaultValue(void);
        void contentChanged(const QString &value);
        void handleNewSettings(void);

    signals:
        void dataChanged(void);

    private:
        enum Label {
            Description,
            DefaultValue,
            Block,
            Entry,
            StringLength,
            Min,
            Max,
            ReferencedMenu,

            MaxLabel
        };

        enum GroupBox {
            OptionsList,

            MaxGroupBox
        };

        QLineEdit *leDescription, *leDefaultValue, *leBlock, *leEntry;
        QSpinBox *sbMin, *sbMax, *sbStringLength;
        QDoubleSpinBox *dsbMin, *dsbMax;
        QPushButton *btAdd, *btDel, *btDefaultValue;
        QListWidget *lwOptions;
        QComboBox *cbReferencedMenu, *cbDefaultValue;
        QGroupBox *gSettings, *gOptionsList;
        QDateTimeEdit *dtDefaultValue;
        const XanteBuilderConfig &config;

        //** Here we hold all available widgets for us to update their color.
        QVector<QLabel *> labels;
        QVector<QGroupBox *> groups;

        //** Here will have only the ones which will really need to be updated.
        QList<QLabel *> selectedLabels;
        QList<QGroupBox *> selectedGroup;

        /* Says if we can notify any change */
        bool mayNotify = false;

        QGroupBox *createItemOptionsWidgets(void);
        QGroupBox *createItemConfigurationWidgets(void);
        QGroupBox *createRangesWidgets(void);
        QHBoxLayout *createRangesAndSettingsWidgets(void);
        QVBoxLayout *createContentWidgets(void);

        void notifyChange(void);
        void prepareDefaultValue(enum XanteItem::Type type);
        void prepareMandatoryFields(enum XanteItem::Type type);
};

#endif

