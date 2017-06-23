
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

class QLineEdit;
class QComboBox;
class QGroupBox;
class QCheckBox;
class QRadioButton;
class QHBoxLayout;
class QListWidget;

class XDialogMenu : public QWidget
{
    Q_OBJECT

    public:
        enum Type {
            DEFAULT,
            DYNAMIC
        };

        enum DynamicMenuType {
            FixedSize,
            FixedOptions,
            DynamicOptions,

            MaxType
        };

        XDialogMenu(QWidget *parent = 0);
        ~XDialogMenu();

    public slots:
        void onDmRadioToggled(bool checked);

    private:
        /* UI */
        QComboBox *cb_type, *cb_mode;
        QGroupBox *gb_events, *gb_dynamic;
        QCheckBox *chb_ev_selected, *chb_ev_exit;
        QListWidget *l_dm_options;
        QLineEdit *le_name, *le_object_id, *le_ev_selected, *le_ev_exit,
                  *le_dm_copies, *le_dm_origin_block, *le_dm_origin_item;

        QHBoxLayout *create_identification_widgets(void);
        QHBoxLayout *create_type_widgets(void);
        QGroupBox *create_events_widgets(void);
        QGroupBox *create_dynamic_details_widgets(void);
        QGroupBox *create_dm_fixed_size_widgets(void);
        QGroupBox *create_dm_fixed_options_widgets(void);
        QGroupBox *create_dm_options_widgets(void);
        void disable_all_widgets(void);
};

#endif

