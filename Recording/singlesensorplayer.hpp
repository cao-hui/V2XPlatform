#pragma once
#include <limits>

class AbstractSingleSensorPlayer: public AbstractDevice
{
public:
    virtual void Tick() = 0;
    virtual PlatformTimestamp GetStartTime() = 0;
};

template<typename T>
class SingleSensorPlayer: public AbstractSingleSensorPlayer
{
public:
    bool IsLastChunk;

    Sensor<T>* sensor;
    QString recordName;

    int NumTotalDataPoint;

    int CurrentDataPointIndex;
    int DataPointCacheEnd;

    DataChunk downloadedData;

    SingleSensorPlayer(Sensor<T>* p_sensor, QString p_recordName)
        :sensor(p_sensor), recordName(p_recordName)
    {

    }

    virtual void Initialize()
    {
        CurrentDataPointIndex = 0;
    }

    virtual void WaitForDevice()
    {
        // Get metadata of record from influxdb
        NumTotalDataPoint = GetRecordTotalCount(sensor->GetName(), recordName);
        CurrentDataPointIndex = 0;
        DataPointCacheEnd = 0;

        IsLastChunk = false;
    }

    PlatformTimestamp GetStartTime()
    {
        if(GetRecordChunk(sensor->GetName(), recordName, 0, 1).size() == 0)
            return PlatformTimestamp(std::numeric_limits<long long>::max(),0ll);
        qint64 time = GetRecordChunk(sensor->GetName(), recordName, 0, 1).back()["time"].toDouble();
        return PlatformTimestamp(time / 1000, time % 1000);
    }

    virtual void Tick()
    {
        // Search for data in current Tick time span
        qint64 currentTime = GPlayer->GetPlayerTime().GetTotalMilliseconds();
        qint64 nextTickTime = currentTime + qRound(1000. / PlayerFPS * RateMultiplier);

        if(!IsLastChunk && (downloadedData.size() == 0 || downloadedData.back()["time"].toDouble() <= nextTickTime))
        {
            //qDebug() << "Insufficient cached data, global pause and download";
            GPlayer->IsGlobalPaused = true;

            DataChunk newData = GetRecordChunk(sensor->GetName(), recordName, DataPointCacheEnd , NUM_DATA_POINTS_PER_DOWNLOAD);
            DataPointCacheEnd += newData.size();

           // qDebug() << "downloaded" << newData.size() << "new points";
            downloadedData.append(newData);

            if(newData.size() < NUM_DATA_POINTS_PER_DOWNLOAD)
            {
                IsLastChunk = true;
            }

            if(downloadedData.size() > NUM_MAX_DATA_POINTS_CACHED)
            {
                CurrentDataPointIndex -= downloadedData.size() - NUM_MAX_DATA_POINTS_CACHED;
                downloadedData = downloadedData.mid(downloadedData.size() - 1 - NUM_MAX_DATA_POINTS_CACHED, NUM_MAX_DATA_POINTS_CACHED);
            }

        }

        GPlayer->IsGlobalPaused = false;

        // Play the downloaded data
        //qDebug() << "Play points in this tick: " << currentTime << nextTickTime;

        for(; CurrentDataPointIndex < downloadedData.size() && downloadedData[CurrentDataPointIndex]["time"].toDouble() < nextTickTime; CurrentDataPointIndex++)
        {
           // qDebug() << "Play point" << CurrentDataPointIndex << downloadedData[CurrentDataPointIndex]["time"].toDouble();
            auto data = ConvertDataPointTo<T>(downloadedData[CurrentDataPointIndex]);
            sensor->SetData(data);
        }
    }
};
