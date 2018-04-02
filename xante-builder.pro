
CURRENT_ROOT_PATH = $$_PRO_FILE_PWD_

TEMPLATE = app
TARGET = xante-builder
DEPENDPATH += $$CURRENT_ROOT_PATH/include src
INCLUDEPATH += $$CURRENT_ROOT_PATH/include /usr/local/include

QT += widgets core
QMAKE_LFLAGS +=
LIBS += /usr/local/lib/libxante.a
CONFIG += debug

# Input
HEADERS += $$CURRENT_ROOT_PATH/include/XMainWindow.hpp          \
        $$CURRENT_ROOT_PATH/include/XProjectWizard.hpp          \
        $$CURRENT_ROOT_PATH/include/XMainDialog.hpp             \
        $$CURRENT_ROOT_PATH/include/XTreeView.hpp               \
        $$CURRENT_ROOT_PATH/include/XTreeModel.hpp              \
        $$CURRENT_ROOT_PATH/include/XDialogMenu.hpp             \
        $$CURRENT_ROOT_PATH/include/XDialogItem.hpp             \
        $$CURRENT_ROOT_PATH/include/XDialogJTFInfo.hpp          \
        $$CURRENT_ROOT_PATH/include/XDialogItemList.hpp         \
        $$CURRENT_ROOT_PATH/include/XDialogSystemSettings.hpp

SOURCES += $$CURRENT_ROOT_PATH/src/main.cpp                 \
        $$CURRENT_ROOT_PATH/src/XanteProject.cpp            \
        $$CURRENT_ROOT_PATH/src/XMainWindow.cpp             \
        $$CURRENT_ROOT_PATH/src/XProjectWizard.cpp          \
        $$CURRENT_ROOT_PATH/src/XMainDialog.cpp             \
        $$CURRENT_ROOT_PATH/src/XTreeView.cpp               \
        $$CURRENT_ROOT_PATH/src/XTreeModel.cpp              \
        $$CURRENT_ROOT_PATH/src/XDialogMenu.cpp             \
        $$CURRENT_ROOT_PATH/src/XDialogItem.cpp             \
        $$CURRENT_ROOT_PATH/src/XDialogJTFInfo.cpp          \
        $$CURRENT_ROOT_PATH/src/XDialogItemList.cpp         \
        $$CURRENT_ROOT_PATH/src/XanteUtils.cpp              \
        $$CURRENT_ROOT_PATH/src/XDialogSystemSettings.cpp   \
        $$CURRENT_ROOT_PATH/src/settings.cpp

