/********************************************************************************
** Form generated from reading UI file 'edgedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDGEDIALOG_H
#define UI_EDGEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_EdgeDialog
{
public:
    QFormLayout *formLayout;
    QLabel *label;
    QSpinBox *spinFrom;
    QLabel *label_2;
    QSpinBox *spinTo;
    QLabel *label_3;
    QSpinBox *spinWeight;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *EdgeDialog)
    {
        if (EdgeDialog->objectName().isEmpty())
            EdgeDialog->setObjectName("EdgeDialog");
        EdgeDialog->resize(300, 200);
        formLayout = new QFormLayout(EdgeDialog);
        formLayout->setObjectName("formLayout");
        label = new QLabel(EdgeDialog);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label);

        spinFrom = new QSpinBox(EdgeDialog);
        spinFrom->setObjectName("spinFrom");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, spinFrom);

        label_2 = new QLabel(EdgeDialog);
        label_2->setObjectName("label_2");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_2);

        spinTo = new QSpinBox(EdgeDialog);
        spinTo->setObjectName("spinTo");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, spinTo);

        label_3 = new QLabel(EdgeDialog);
        label_3->setObjectName("label_3");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_3);

        spinWeight = new QSpinBox(EdgeDialog);
        spinWeight->setObjectName("spinWeight");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, spinWeight);

        buttonBox = new QDialogButtonBox(EdgeDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        formLayout->setWidget(3, QFormLayout::ItemRole::SpanningRole, buttonBox);


        retranslateUi(EdgeDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, EdgeDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, EdgeDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(EdgeDialog);
    } // setupUi

    void retranslateUi(QDialog *EdgeDialog)
    {
        EdgeDialog->setWindowTitle(QCoreApplication::translate("EdgeDialog", "\346\267\273\345\212\240\350\276\271", nullptr));
        label->setText(QCoreApplication::translate("EdgeDialog", "\350\265\267\347\202\271:", nullptr));
        label_2->setText(QCoreApplication::translate("EdgeDialog", "\347\273\210\347\202\271:", nullptr));
        label_3->setText(QCoreApplication::translate("EdgeDialog", "\346\235\203\351\207\215:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EdgeDialog: public Ui_EdgeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDGEDIALOG_H
