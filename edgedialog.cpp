#include "edgedialog.h"
#include "ui_edgedialog.h"

EdgeDialog::EdgeDialog(QWidget *parent, int maxNode) :
    QDialog(parent),
    ui(new Ui::EdgeDialog) {
    ui->setupUi(this);
    ui->spinFrom->setRange(0, maxNode);
    ui->spinTo->setRange(0, maxNode);
    ui->spinWeight->setRange(1, 100);
    ui->spinWeight->setValue(50);
}

EdgeDialog::~EdgeDialog() {
    delete ui;
}

int EdgeDialog::getFromNode() const {
    return ui->spinFrom->value();
}

int EdgeDialog::getToNode() const {
    return ui->spinTo->value();
}

int EdgeDialog::getWeight() const {
    return ui->spinWeight->value();
}