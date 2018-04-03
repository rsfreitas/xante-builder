
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Mon Jun 12 23:14:48 2017
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

#ifndef _XDIALOGITEM_HPP
#define _XDIALOGITEM_HPP          1

#include <QWidget>
#include <QVector>
#include <QTableWidget>

class QLineEdit;
class QComboBox;
class QGroupBox;
class QCheckBox;
class QRadioButton;
class QHBoxLayout;
class QListWidget;
class QVBoxLayout;
class QSpinBox;

class XanteProject;
class XanteItem;
class XanteMenu;
class XanteJTF;

class XDialogItem : public QWidget
{
    Q_OBJECT

    public:
        XDialogItem(QWidget *parent = 0);
        ~XDialogItem() {}
        void setCurrentProject(int selectedMenuIndex,
                               int selectedItemIndex);

        void setSelection(int selectedMenuIndex, int selectedItemIndex);
        void clear(void);
        void saveCurrentState(void);

    private slots:
        void selectItemType(int index);
        void addOption(void);
        void delOption(void);
        void addOptionHelp(void);
        void delOptionHelp(void);
        void helpGroupToggled(bool on);

    protected:
        void hideEvent(QHideEvent *event) override;

    signals:
        void treeViewNeedsUpdate();
        void projectHasChanges();

    private:
        enum LineEdit {
            Name,
            ObjectId,
            CfgBlock,
            CfgItem,
            Options,
            HelpBrief,
            HelpDescriptive,
            DefaultValue,
            BtnOkLabel,
            BtnCancelLabel,
            BtnExtraLabel,
            BtnHelpLabel,
            Title,

            MaxLineEdit
        };

        enum ComboBox {
            Type,
            Mode,
            MenuReference,

            MaxComboBox
        };

        enum GroupBox {
            Events,
            Help,
            OptionsGb,
            ListOptions,
            ListHelpOptions,
            Config,
            InputRanges,
            Geometry,
            BtnLabels,
            Ui,

            MaxGroupBox
        };

        enum ListWidget {
            OptionsLw,
            HelpOptions,

            MaxListWidget
        };

        enum SpinBox {
            InputMin,
            InputMax,
            InputStringLength,
            Width,
            Height,

            MaxSpinBox
        };

        enum CheckBox {
            ExtraButton,

            MaxCheckBox
        };

        int currentMenuIndex = -1, currentItemIndex = -1;

        /* UI */
        QVector<QListWidget *> listWidget;
        QVector<QLineEdit *> lineEdit;
        QVector<QGroupBox *> groupBox;
        QVector<QComboBox *> comboBox;
        QVector<QCheckBox *> checkBox;
        QVector<QSpinBox *> spinBox;
        QTableWidget *tbEvents;

        QGroupBox *createMainWidgets(void);
        QHBoxLayout *createRangesAndSettingsWidgets(void);
        QVBoxLayout *createContentWidgets(void);
        QVBoxLayout *createExtraFeaturesWidgets(void);
        QHBoxLayout *createUiTitleWidgets(void);
        QGroupBox *createDataWidgets(void);
        QGroupBox *createUiWidgets(void);
        QGroupBox *createItemConfigurationWidgets(void);
        QGroupBox *createItemOptionsWidgets(void);
        QGroupBox *createItemHelpWidgets(void);
        QGroupBox *createEventsWidgets(void);
        QGroupBox *createBtnLabelsWidgets(void);
        QGroupBox *createRangesWidgets(void);
        QGroupBox *createGeometryWidgets(void);

        void setupWidgets(void);
        void setupDynamicInfoWidgets(const XanteItem &item);
        void setupEventsWidgets(const XanteItem &item);
        void setupHelpWidgets(const XanteItem &item);
        void setupConfigWidgets(const XanteItem &item);
        void setupInputRangesWidgets(const XanteItem &item);
        void setupOptionsWidgets(const XanteItem &item);
        void setupMenuReferences(const XanteItem &item, XanteJTF &jtf);

        void disableAllWidgets(void);
        void enableInputRanges(int type);
        void enableOptions(int type);
        void enableHelp(int type);

        bool updateXanteItem(void);
        void updateXanteItemEvents(XanteItem &item);
        void updateXanteItemHelp(XanteItem &item);
        void updateXanteItemContent(XanteItem &item);
        void updateXanteItemOptions(XanteItem &item);
        void updateXanteItemInputRanges(XanteItem &item);
        void updateXanteItemConfig(XanteItem &item);

        XanteItem createXanteItemFromWidgets(XanteJTF &jtf, const XanteMenu &menu);
};

#endif

