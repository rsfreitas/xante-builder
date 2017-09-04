
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

class QLineEdit;
class QComboBox;
class QGroupBox;
class QCheckBox;
class QRadioButton;
class QHBoxLayout;
class QListWidget;

class XanteProject;
class XanteItem;

class XDialogItem : public QWidget
{
    Q_OBJECT

    public:
        XDialogItem(QWidget *parent = 0);
        ~XDialogItem();
        void set_current_project(XanteProject *project,
                                 int selected_menu_index,
                                 int selected_item_index);

        void set_selection(int selected_menu_index, int selected_item_index);

    private slots:
        void select_item_type(int index);
        void add_option(void);
        void del_option(void);
        void add_option_help(void);
        void del_option_help(void);

    protected:
        void hideEvent(QHideEvent *event) override;

    private:
        enum LineEdit {
            Name,
            ObjectId,
            EventSelected,
            EventExit,
            EventValueConfirmed,
            EventValueChanged,
            CfgBlock,
            CfgItem,
            Options,
            InputStringLength,
            InputMin,
            InputMax,
            HelpBrief,
            HelpDescription,
            DefaultValue,

            MaxLineEdit
        };

        enum ComboBox {
            Type,
            Mode,
            MenuReference,

            MaxComboBox
        };

        enum CheckBox {
            EvSelected,
            EvExit,
            EvValueConfirmed,
            EvValueChanged,

            MaxCheckBox
        };

        enum GroupBox {
            Events,
            Help,
            OptionsGb,
            ListOptions,
            ListHelpOptions,
            Config,
            InputRanges,

            MaxGroupBox
        };

        enum ListWidget {
            OptionsLw,
            HelpOptions,

            MaxListWidget
        };

        int current_menu_index = -1, current_item_index = -1;
        XanteProject *project = nullptr;

        /* UI */
        QVector<QLineEdit *> line_edit;
        QVector<QCheckBox *> check_box;
        QVector<QGroupBox *> group_box;
        QVector<QComboBox *> combo_box;
        QVector<QListWidget *> list_widget;

        QComboBox *cb_type, *cb_mode, *cb_menu_id;
        QGroupBox *gb_events, *gb_help;
        QCheckBox *chb_ev_selected, *chb_ev_exit, *chb_ev_value_confirmed,
                  *chb_ev_value_changed;

        QListWidget *l_options, *l_help_options;
        QLineEdit *le_name, *le_object_id, *le_ev_selected, *le_ev_exit,
                  *le_ev_value_confirmed, *le_ev_value_changed, *le_cfg_block,
                  *le_cfg_item, *le_options, *le_input_str_length,
                  *le_input_min, *le_input_max, *le_help_brief,
                  *le_help_description, *le_default_value;

        QHBoxLayout *create_identification_widgets(void);
        QHBoxLayout *create_type_widgets(void);
        QHBoxLayout *create_item_details_widgets(void);
        QHBoxLayout *create_ranges_and_events_widgets(void);
        QGroupBox *create_item_configuration_widgets(void);
        QGroupBox *create_item_options_widgets(void);
        QGroupBox *create_item_help_widgets(void);
        QGroupBox *create_ranges_widgets(void);
        QGroupBox *create_events_widgets(void);

        void setup_widgets(void);
        void setup_widgets(XanteItem item);
};

#endif

