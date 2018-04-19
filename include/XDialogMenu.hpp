
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Mon Jun 12 23:14:44 2017
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

#ifndef _XDIALOGMENU_HPP
#define _XDIALOGMENU_HPP          1

#include <QtWidgets>
#include <xante/libxante.h>

class XanteProject;
class XanteBuilderConfig;

class XDialogMenu : public QWidget
{
    Q_OBJECT

    public:
        XDialogMenu(const XanteBuilderConfig &config, QWidget *parent = 0);
        ~XDialogMenu();
        void setCurrentProject(int selectedMenuIndex);
        void setSelection(int selectedMenuIndex);
        void clear(void);
        bool saveCurrentState(void);

    public slots:
        void dynamicRadioToggled(bool checked);
        void selectMenuType(int index);
        void addDynamicFixedOption(void);
        void delDynamicFixedOption(void);
        void contentChanged(const QString &value);
        void tableContentChanged(QTableWidgetItem *item);
        void selectionChanged(void);
        void groupSelected(bool checked);
        void handleNewSettings(void);

    signals:
        void treeViewNeedsUpdate();
        void projectHasChanges();

    private:
        enum LineEdit {
            Name,
            ObjectId,
            DynamicOriginBlock,
            DynamicOriginItem,
            DynamicNumberOfCopies,
            BlockPrefix,

            MaxLineEdit
        };

        enum ComboBox {
            Type,
            Mode,

            MaxComboBox
        };

        enum GroupBox {
            DynamicFixed,
            DynamicOptions,
            DynamicConfig,
            Events,
            Dynamic,

            MaxGroupBox
        };

        bool mayNotify = false;
        XanteMenu oldMenu;
        int currentMenuIndex = -1;
        const XanteBuilderConfig &config;
        QList<QLabel *> labels;

        /* UI */
        QVector<QLineEdit *> lineEdit;
        QVector<QComboBox *> comboBox;
        QVector<QGroupBox *> groupBox;
        QVector<QRadioButton *> radioButton;
        QSpinBox *sbWidth, *sbHeight;
        QTableWidget *tbEvents;
        QListWidget *dynamicOptions;

        QHBoxLayout *createIdentificationWidgets(void);
        QHBoxLayout *createTypeWidgets(void);
        QGroupBox *createEventsWidgets(void);
        QGroupBox *createDynamicDetailsWidgets(void);
        QGroupBox *createDynamicFixedSizeWidgets(void);
        QGroupBox *createDynamicFixedOptionsWidgets(void);
        QGroupBox *createDynamicOptionsWidgets(void);

        void setupWidgets(void);
        void setupWidgets(XanteMenu menu);
        void setupDynamicInfoWidgets(XanteMenu menu);
        void setupEventsWidgets(XanteMenu menu);

        void disableAllWidgets(void);
        bool updateXanteMenu(void);
        void updateXanteMenuEvents(XanteMenu &menu);
        void updateXanteMenuDynamic(XanteMenu &menu);

        XanteMenu createXanteMenuFromWidgets(XanteJTF &jtf);
        void notifyChange(void);
};

#endif

