
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Sat Jun 10 11:05:50 2017
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

#ifndef _XTREEVIEW_HPP
#define _XTREEVIEW_HPP          1

#include <QTreeView>

class XanteProject;

class XTreeView : public QTreeView
{
    Q_OBJECT

    public:
        int currentSelectedMenu = 0;
        int currentSelectedItem = -1;

        XTreeView(QWidget *parent = 0);
        ~XTreeView();

        void controlDialogActions(bool enable);
        void redraw(QAbstractItemModel *model);

    protected:
        void mousePressEvent(QMouseEvent *event);
        void mouseDoubleClickEvent(QMouseEvent *event);
        void keyPressEvent(QKeyEvent *event);

    private slots:
        void addMenu();
        void addItem();
        void copyItem();
        void pasteItem();
        void cutItem();
        void removeItem();
        void changeItemsPosition();

    signals:
        void itemSelected();
        void menuSelected();
        void contentChanged();
        void treeViewNeedsUpdate();
        void projectHasChanges();

    private:
        enum SelectedLine {
            None,
            ItemLine,
            MenuLine
        };

        enum XTreeView::SelectedLine selectedLine = XTreeView::SelectedLine::None;
        QMenu *menu;
        QAction *acAddMenu, *acAddItem, *acCopy, *acPaste, *acCut, *acRemove,
                *acChange;

        void createRightClickMenu(void);
        void displaySelectedItem(QModelIndex index);
        void emitSignalToUpdate(void);

        QStringList saveState(void);
        void restoreState(QStringList state);
};

#endif

