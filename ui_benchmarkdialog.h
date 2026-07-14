/********************************************************************************
** Form generated from reading UI file 'benchmarkdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BENCHMARKDIALOG_H
#define UI_BENCHMARKDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_BenchmarkDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *tableResults;
    QPushButton *btnExport;

    void setupUi(QDialog *BenchmarkDialog)
    {
        if (BenchmarkDialog->objectName().isEmpty())
            BenchmarkDialog->setObjectName("BenchmarkDialog");
        BenchmarkDialog->resize(700, 500);
        verticalLayout = new QVBoxLayout(BenchmarkDialog);
        verticalLayout->setObjectName("verticalLayout");
        tableResults = new QTableWidget(BenchmarkDialog);
        tableResults->setObjectName("tableResults");
        tableResults->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableResults->setAlternatingRowColors(true);
        tableResults->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        verticalLayout->addWidget(tableResults);

        btnExport = new QPushButton(BenchmarkDialog);
        btnExport->setObjectName("btnExport");

        verticalLayout->addWidget(btnExport);


        retranslateUi(BenchmarkDialog);

        QMetaObject::connectSlotsByName(BenchmarkDialog);
    } // setupUi

    void retranslateUi(QDialog *BenchmarkDialog)
    {
        BenchmarkDialog->setWindowTitle(QCoreApplication::translate("BenchmarkDialog", "\346\211\271\351\207\217\346\200\247\350\203\275\346\265\213\350\257\225\347\273\223\346\236\234", nullptr));
        btnExport->setText(QCoreApplication::translate("BenchmarkDialog", "\345\257\274\345\207\272CSV", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BenchmarkDialog: public Ui_BenchmarkDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BENCHMARKDIALOG_H
