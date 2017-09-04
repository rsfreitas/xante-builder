
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
        void set_current_project(XanteProject *project,
                                 int selected_menu_index);

        void set_selection(int selected_menu_index);

    public slots:
        void dynamic_radio_toggled(bool checked);
        void select_menu_type(int index);
        void add_dynamic_fixed_option(void);
        void del_dynamic_fixed_option(void);

    protected:
        void hideEvent(QHideEvent *event) override;

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

        XanteProject *project = nullptr;
        int current_menu_index = -1;

        /* UI */
        QVector<QLineEdit *> line_edit;
        QVector<QCheckBox *> check_box;
        QVector<QComboBox *> combo_box;
        QVector<QGroupBox *> group_box;
        QVector<QRadioButton *> radio_button;

        QListWidget *dynamic_options;

        QHBoxLayout *create_identification_widgets(void);
        QHBoxLayout *create_type_widgets(void);
        QGroupBox *create_events_widgets(void);
        QGroupBox *create_dynamic_details_widgets(void);
        QGroupBox *create_dm_fixed_size_widgets(void);
        QGroupBox *create_dm_fixed_options_widgets(void);
        QGroupBox *create_dm_options_widgets(void);

        void setup_widgets(void);
        void setup_widgets(XanteMenu menu);
        void setup_widgets_dynamic_info(XanteMenu menu);
        void setup_widgets_events(XanteMenu menu);
};

#endif

