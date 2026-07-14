/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_2;
    QFormLayout *formLayout;
    QLabel *label;
    QSpinBox *spinNodeCount;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinDensity;
    QPushButton *btnGenerate;
    QPushButton *btnImportGraph;
    QPushButton *btnExportGraph;
    QGroupBox *groupBox_7;
    QVBoxLayout *verticalLayout_6;
    QPushButton *btnModePan;
    QPushButton *btnModeAddNode;
    QPushButton *btnModeAddEdge;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_3;
    QPushButton *btnPrim;
    QPushButton *btnKruskal;
    QPushButton *btnBenchmark;
    QCheckBox *checkShowWeight;
    QCheckBox *checkWeightedLayout;
    QPushButton *btnResetView;
    QPushButton *btnExportResult;
    QPushButton *btnHelp;
    QPushButton *btnClear;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_4;
    QVBoxLayout *layoutGraph;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_4;
    QPlainTextEdit *textResult;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_5;
    QPlainTextEdit *textLog;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 800);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName("verticalLayout_2");
        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName("groupBox_2");
        formLayout = new QFormLayout(groupBox_2);
        formLayout->setObjectName("formLayout");
        label = new QLabel(groupBox_2);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label);

        spinNodeCount = new QSpinBox(groupBox_2);
        spinNodeCount->setObjectName("spinNodeCount");
        spinNodeCount->setMinimum(2);
        spinNodeCount->setMaximum(5000);
        spinNodeCount->setValue(100);

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, spinNodeCount);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName("label_2");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_2);

        doubleSpinDensity = new QDoubleSpinBox(groupBox_2);
        doubleSpinDensity->setObjectName("doubleSpinDensity");
        doubleSpinDensity->setMinimum(0.010000000000000);
        doubleSpinDensity->setMaximum(1.000000000000000);
        doubleSpinDensity->setSingleStep(0.050000000000000);
        doubleSpinDensity->setValue(0.300000000000000);

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, doubleSpinDensity);


        verticalLayout_2->addWidget(groupBox_2);

        btnGenerate = new QPushButton(groupBox);
        btnGenerate->setObjectName("btnGenerate");

        verticalLayout_2->addWidget(btnGenerate);

        btnImportGraph = new QPushButton(groupBox);
        btnImportGraph->setObjectName("btnImportGraph");

        verticalLayout_2->addWidget(btnImportGraph);

        btnExportGraph = new QPushButton(groupBox);
        btnExportGraph->setObjectName("btnExportGraph");

        verticalLayout_2->addWidget(btnExportGraph);

        groupBox_7 = new QGroupBox(groupBox);
        groupBox_7->setObjectName("groupBox_7");
        verticalLayout_6 = new QVBoxLayout(groupBox_7);
        verticalLayout_6->setObjectName("verticalLayout_6");
        btnModePan = new QPushButton(groupBox_7);
        btnModePan->setObjectName("btnModePan");
        btnModePan->setCheckable(true);
        btnModePan->setChecked(true);

        verticalLayout_6->addWidget(btnModePan);

        btnModeAddNode = new QPushButton(groupBox_7);
        btnModeAddNode->setObjectName("btnModeAddNode");
        btnModeAddNode->setCheckable(true);

        verticalLayout_6->addWidget(btnModeAddNode);

        btnModeAddEdge = new QPushButton(groupBox_7);
        btnModeAddEdge->setObjectName("btnModeAddEdge");
        btnModeAddEdge->setCheckable(true);

        verticalLayout_6->addWidget(btnModeAddEdge);


        verticalLayout_2->addWidget(groupBox_7);

        groupBox_3 = new QGroupBox(groupBox);
        groupBox_3->setObjectName("groupBox_3");
        verticalLayout_3 = new QVBoxLayout(groupBox_3);
        verticalLayout_3->setObjectName("verticalLayout_3");
        btnPrim = new QPushButton(groupBox_3);
        btnPrim->setObjectName("btnPrim");

        verticalLayout_3->addWidget(btnPrim);

        btnKruskal = new QPushButton(groupBox_3);
        btnKruskal->setObjectName("btnKruskal");

        verticalLayout_3->addWidget(btnKruskal);


        verticalLayout_2->addWidget(groupBox_3);

        btnBenchmark = new QPushButton(groupBox);
        btnBenchmark->setObjectName("btnBenchmark");

        verticalLayout_2->addWidget(btnBenchmark);

        checkShowWeight = new QCheckBox(groupBox);
        checkShowWeight->setObjectName("checkShowWeight");
        checkShowWeight->setChecked(true);

        verticalLayout_2->addWidget(checkShowWeight);

        checkWeightedLayout = new QCheckBox(groupBox);
        checkWeightedLayout->setObjectName("checkWeightedLayout");
        checkWeightedLayout->setChecked(false);

        verticalLayout_2->addWidget(checkWeightedLayout);

        btnResetView = new QPushButton(groupBox);
        btnResetView->setObjectName("btnResetView");

        verticalLayout_2->addWidget(btnResetView);

        btnExportResult = new QPushButton(groupBox);
        btnExportResult->setObjectName("btnExportResult");

        verticalLayout_2->addWidget(btnExportResult);

        btnHelp = new QPushButton(groupBox);
        btnHelp->setObjectName("btnHelp");

        verticalLayout_2->addWidget(btnHelp);

        btnClear = new QPushButton(groupBox);
        btnClear->setObjectName("btnClear");

        verticalLayout_2->addWidget(btnClear);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout->addWidget(groupBox);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        groupBox_4 = new QGroupBox(centralwidget);
        groupBox_4->setObjectName("groupBox_4");
        layoutGraph = new QVBoxLayout(groupBox_4);
        layoutGraph->setObjectName("layoutGraph");

        verticalLayout->addWidget(groupBox_4);

        groupBox_5 = new QGroupBox(centralwidget);
        groupBox_5->setObjectName("groupBox_5");
        verticalLayout_4 = new QVBoxLayout(groupBox_5);
        verticalLayout_4->setObjectName("verticalLayout_4");
        textResult = new QPlainTextEdit(groupBox_5);
        textResult->setObjectName("textResult");
        textResult->setReadOnly(true);

        verticalLayout_4->addWidget(textResult);


        verticalLayout->addWidget(groupBox_5);

        groupBox_6 = new QGroupBox(centralwidget);
        groupBox_6->setObjectName("groupBox_6");
        verticalLayout_5 = new QVBoxLayout(groupBox_6);
        verticalLayout_5->setObjectName("verticalLayout_5");
        textLog = new QPlainTextEdit(groupBox_6);
        textLog->setObjectName("textLog");
        textLog->setReadOnly(true);
        textLog->setMaximumBlockCount(500);

        verticalLayout_5->addWidget(textLog);


        verticalLayout->addWidget(groupBox_6);


        horizontalLayout->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\234\200\345\260\217\347\224\237\346\210\220\346\240\221\347\256\227\346\263\225\345\217\257\350\247\206\345\214\226", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\346\216\247\345\210\266\351\235\242\346\235\277", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "\345\233\276\347\224\237\346\210\220", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\350\212\202\347\202\271\346\225\260:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\350\276\271\345\257\206\345\272\246:", nullptr));
        btnGenerate->setText(QCoreApplication::translate("MainWindow", "\347\224\237\346\210\220\351\232\217\346\234\272\345\233\276", nullptr));
        btnImportGraph->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\205\245\345\233\276", nullptr));
        btnExportGraph->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272\345\233\276", nullptr));
        groupBox_7->setTitle(QCoreApplication::translate("MainWindow", "\344\272\244\344\272\222", nullptr));
        btnModePan->setText(QCoreApplication::translate("MainWindow", "\346\265\217\350\247\210", nullptr));
        btnModeAddNode->setText(QCoreApplication::translate("MainWindow", "\345\212\240\347\202\271", nullptr));
        btnModeAddEdge->setText(QCoreApplication::translate("MainWindow", "\345\212\240\350\276\271", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "\347\256\227\346\263\225\346\211\247\350\241\214", nullptr));
        btnPrim->setText(QCoreApplication::translate("MainWindow", "Prim\347\256\227\346\263\225", nullptr));
        btnKruskal->setText(QCoreApplication::translate("MainWindow", "Kruskal\347\256\227\346\263\225", nullptr));
        btnBenchmark->setText(QCoreApplication::translate("MainWindow", "\346\211\271\351\207\217\346\200\247\350\203\275\346\265\213\350\257\225", nullptr));
        checkShowWeight->setText(QCoreApplication::translate("MainWindow", "\346\230\276\347\244\272\350\276\271\346\235\203\351\207\215", nullptr));
        checkWeightedLayout->setText(QCoreApplication::translate("MainWindow", "\350\267\235\347\246\273\347\255\211\346\257\224\350\276\271\346\235\203", nullptr));
        btnResetView->setText(QCoreApplication::translate("MainWindow", "\351\207\215\347\275\256\350\247\206\345\233\276", nullptr));
        btnExportResult->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272\347\273\223\346\236\234", nullptr));
        btnHelp->setText(QCoreApplication::translate("MainWindow", "\360\237\223\226 \344\275\277\347\224\250\346\225\231\347\250\213", nullptr));
        btnClear->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "\345\233\276\345\217\257\350\247\206\345\214\226", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "\347\273\223\346\236\234\350\276\223\345\207\272", nullptr));
        textResult->setPlaceholderText(QCoreApplication::translate("MainWindow", "\347\256\227\346\263\225\350\277\220\350\241\214\347\273\223\346\236\234\345\260\206\346\230\276\347\244\272\345\234\250\350\277\231\351\207\214...", nullptr));
        groupBox_6->setTitle(QCoreApplication::translate("MainWindow", "\350\277\220\350\241\214\346\227\245\345\277\227", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
