#include "datatable.h"

dataTable::dataTable(QTableWidget *parent) : QTableWidget(parent)
{
    //set 6 colums
    this->setColumnCount(6);

    //HorizontalHeader title
    QStringList headerLabels;
    headerLabels << "Car ID" << "Longitude" << "Latitude" << "Heading" << "Speed(m/s)" << "Wheel Angle";
    this->setHorizontalHeaderLabels(headerLabels);
    this->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //TableContent config
    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectItems);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

dataTable::~dataTable()
{

}

void dataTable::refresh()
{
    this->clearContents();
    this->setSortingEnabled(false);
    this->setRowCount(GWorld->Vehicles.count());
    int cnt = 0;
    for(auto it : GWorld->Vehicles)
    {
        this->setItem(cnt,0,new QTableWidgetItem(QString::number(it->ID)));
        this->setItem(cnt,1,new QTableWidgetItem(QString::number(it->Longitude,10,6)));
        this->setItem(cnt,2,new QTableWidgetItem(QString::number(it->Latitude,10,6)));
        this->setItem(cnt,3,new QTableWidgetItem(QString::number(it->Heading,10,6)));
        this->setItem(cnt,4,new QTableWidgetItem(QString::number(it->Speed,10,6)));
        this->setItem(cnt,5,new QTableWidgetItem(QString::number(it->WheelAngle,10,6)));
        ++cnt;
    }
    this->setSortingEnabled(true);
    this->sortByColumn(0);
}
