
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

#include <QWidget>
#include <QVector>

#include "XanteJTF.hpp"

class QLineEdit;
class QComboBox;
class QGroupBox;
class QCheckBox;
class QRadioButton;
class QHBoxLayout;
class QListWidget;
class XanteProject;

class XDialogMenu : public QWidget
{
    Q_OBJECT

    public:
        XDialogMenu(QWidget *parent = 0);
        ~XDialogMenu();
        void setCurrentProject(int selectedMenuIndex);
        void setSelection(int selectedMenuIndex);
        void clear(void);
        void saveCurrentState(void);

    public slots:
        void dynamicRadioToggled(bool checked);
        void selectMenuType(int index);
        void addDynamicFixedOption(void);
        void delDynamicFixedOption(void);

    protected:
        void hideEvent(QHideEvent *event) override;

    signals:
        void treeViewNeedsUpdate();
        void projectHasChanges();

    private:
        enum LineEdit {
            Name,
            ObjectId,
            EventSelected,
            EventExit,
            DynamicOriginBlock,
            DynamicOriginItem,
            DynamicNumberOfCopies,
            BlockPrefix,

            MaxLineEdit
        };

        enum CheckBox {
            EvSelected,
            EvExit,

            MaxCheckBox
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

        int currentMenuIndex = -1;

        /* UI */
        QVector<QLineEdit *> lineEdit;
        QVector<QCheckBox *> checkBox;
        QVector<QComboBox *> comboBox;
        QVector<QGroupBox *> groupBox;
        QVector<QRadioButton *> radioButton;

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
        bool updateXanteMenuEvents(XanteMenu &menu);
        bool updateXanteMenuDynamic(XanteMenu &menu);
};

#endif

