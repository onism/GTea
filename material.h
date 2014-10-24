#ifndef MATERIAL_H
#define MATERIAL_H

#include <QDialog>

namespace Ui {
class Material;
}

class Material : public QDialog
{
    Q_OBJECT

public:
    explicit Material(QWidget *parent = 0);
    ~Material();

private:
    Ui::Material *ui;
};

#endif // MATERIAL_H
