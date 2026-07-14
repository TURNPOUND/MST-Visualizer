#ifndef EDGEDIALOG_H
#define EDGEDIALOG_H

#include <QDialog>

namespace Ui {
class EdgeDialog;
}

class EdgeDialog : public QDialog {
    Q_OBJECT
public:
    explicit EdgeDialog(QWidget *parent = nullptr, int maxNode = 99);
    ~EdgeDialog();
    int getFromNode() const;
    int getToNode() const;
    int getWeight() const;

private:
    Ui::EdgeDialog *ui;
};

#endif
