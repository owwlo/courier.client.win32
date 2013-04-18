/********************************************************************************
** Form generated from reading UI file 'uimanager.ui'
**
** Created: Thu Mar 7 15:45:11 2013
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIMANAGER_H
#define UI_UIMANAGER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UIManager
{
public:

    void setupUi(QWidget *UIManager)
    {
        if (UIManager->objectName().isEmpty())
            UIManager->setObjectName(QString::fromUtf8("UIManager"));
        UIManager->resize(400, 300);

        retranslateUi(UIManager);

        QMetaObject::connectSlotsByName(UIManager);
    } // setupUi

    void retranslateUi(QWidget *UIManager)
    {
        UIManager->setWindowTitle(QApplication::translate("UIManager", "UIManager", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UIManager: public Ui_UIManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIMANAGER_H
