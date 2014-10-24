#ifndef LOCATION_H
#define LOCATION_H

#include <QDialog>

namespace Ui {
class Location;
}

class Location : public QDialog
{
    Q_OBJECT

public:
    explicit Location(QWidget *parent = 0);
    ~Location();

private:
    Ui::Location *ui;
};

#endif // LOCATION_H
