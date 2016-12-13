#ifndef DATATABLE_H
#define DATATABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include "WorldModel/world.h"

class dataTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit dataTable(QTableWidget *parent = 0);
    ~dataTable();
    void refresh();

private:

signals:

public slots:
};

#endif // DATATABLE_H
