#include "serialportframedeviceex.h"
#include <QThread>
#include <QSettings>
#include <QDebug>


SPFDWorkerEx::SPFDWorkerEx(SerialPortFrameDeviceEx *p_device)
    :device(p_device)
{

}

void SPFDWorkerEx::run()
{
    // Create serial port on worker thread
    device->serialPort = new QSerialPort();
    device->serialPort->moveToThread(this);
    connect(device->serialPort, SIGNAL(readyRead()), SLOT(onReadyRead()), Qt::DirectConnection);
    connect(device->serialPort, SIGNAL(error(QSerialPort::SerialPortError)), SLOT(onError(QSerialPort::SerialPortError)));

    // Notify main thread the port is created
    device->workerCreateSem.release();

    // Wait for WaitForDevice() command from main thread
    device->workerWaitForDeviceSem.acquire();

    forever {
        qDebug() << "Waiting for serial port device at: " << device->serialPort->portName();

        forever {
            if(device->serialPort->open(device->openMode))
                break;

            QSettings settings("PlatformConfig.ini", QSettings::IniFormat);
            int ms = settings.value("DisconnectRecovery/interval", 0).toInt();
            timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
            nanosleep(&ts, NULL);
        }

        qDebug() << "Serial port device connected at: " << device->serialPort->portName();

        // Enter main event loop, receive data from serial port
        int ret = exec();

        if(ret == 0) // quit() is received
            break;

        // otherwise some errors encountered, restart WaitForDevice
        if(device->serialPort->isOpen())
            device->serialPort->close();
    }

    // after receive quit() from main thread , serial port is destroyed here
    delete device->serialPort;
}


void SPFDWorkerEx::onReadyRead()
{
    const QByteArray& newData = device->serialPort->readAll();
    parseNewData(newData);
}

void SPFDWorkerEx::onError(QSerialPort::SerialPortError error)
{
    exit(error);
}

void SPFDWorkerEx::parseNewData(const QByteArray& newData)
{
    if(device->frameDelimiterStart == "" && device->frameDelimiterEnd == "")
    {
        device->onData(newData);
        return;
    }

    buffer.append(newData);

    if(device->frameDelimiterStart != "" && device->frameDelimiterEnd == "")
    {
        while(buffer.count(device->frameDelimiterStart) >= 2)
        {
            int frameStart = buffer.indexOf(device->frameDelimiterStart);
            int nextFrameStart = buffer.indexOf(device->frameDelimiterStart, frameStart + device->minFrameLength);
            if(nextFrameStart == -1)
                break;
            const QByteArray& frame = buffer.mid(frameStart, nextFrameStart - frameStart);
            device->onData(frame);
            buffer = buffer.mid(nextFrameStart);
        }
    }
    if(device->frameDelimiterStart == "" && device->frameDelimiterEnd != "")
    {
        while(buffer.indexOf(device->frameDelimiterEnd) != -1)
        {
            int frameEnd = buffer.indexOf(device->frameDelimiterEnd, device->minFrameLength - 1);
            if(frameEnd == -1)
                break;
            const QByteArray& frame = buffer.left(frameEnd + 1);
            device->onData(frame);
            buffer = buffer.mid(frameEnd + 1);
        }
    }
    if(device->frameDelimiterStart != "" && device->frameDelimiterEnd != "")
    {
        while(buffer.indexOf(device->frameDelimiterStart) != -1 && buffer.indexOf(device->frameDelimiterEnd, buffer.indexOf(device->frameDelimiterStart) + 1) != -1)
        {
            int frameEnd = buffer.indexOf(device->frameDelimiterEnd, buffer.indexOf(device->frameDelimiterStart) + 1 + device->minFrameLength);
            if(frameEnd == -1)
                break;
            const QByteArray& frame = buffer.mid(
                        buffer.indexOf(device->frameDelimiterStart),
                        frameEnd - buffer.indexOf(device->frameDelimiterStart) + 1
                        );
            device->onData(frame);
            buffer = buffer.mid(buffer.indexOf(device->frameDelimiterEnd, buffer.indexOf(device->frameDelimiterStart) + 1) + 1);
        }
    }
}

SerialPortFrameDeviceEx::SerialPortFrameDeviceEx()
    :worker(this)
{
    openMode = QSerialPort::ReadOnly;
    minFrameLength = 0;

    worker.start();
    workerCreateSem.acquire(); // Wait for worker to initialize
}


SerialPortFrameDeviceEx::~SerialPortFrameDeviceEx()
{
    worker.quit();
}

void SerialPortFrameDeviceEx::WaitForDevice()
{
    // Notify worker thread to wait for device
    workerWaitForDeviceSem.release();
}
