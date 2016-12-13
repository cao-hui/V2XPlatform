#include <QDebug>

#include "MapData.h"
#include "ApproachObject.h"
#include "Messaging/mapmessage.h"

// We assume that all memory allocation will success
QByteArray MapMsg::EncodeToMapMessage()
{
    MapData_t *map = (MapData_t *)calloc(sizeof(*map), 1);
    map->msgID = 7;
    map->intersections = (decltype(map->intersections))calloc(sizeof(*map->intersections), 1);

    Intersection_t   *intersect = nullptr;
    ApproachObject_t *approach  = nullptr;

    for (const auto &it : intersects) {
        intersect = (Intersection_t *)calloc(sizeof(Intersection_t), 1);
        intersect->id.size = 2;
        intersect->id.buf = (uint8_t *)calloc(2, 1);
        *(short *)intersect->id.buf = htons(it.id);
        intersect->refPoint = (Position3D_t *)calloc(sizeof(*intersect->refPoint), 1);
        intersect->refPoint->lat = it.center.latitude;
        intersect->refPoint->Long = it.center.longitude;
        for (const auto &iter : it.paths) {
            approach = (ApproachObject_t *)calloc(sizeof(ApproachObject_t), 1);
            approach->refPoint = (Position3D_t *)calloc(sizeof(*approach->refPoint), 1);
            approach->refPoint->lat = iter.junction.latitude;
            approach->refPoint->Long = iter.junction.longitude;
            approach->laneWidth = (LaneWidth_t *)calloc(sizeof(*approach->laneWidth), 1);
            *approach->laneWidth = iter.lanewidth;
            asn_set_add(&intersect->approaches.list, approach);
        }
        asn_set_add(&map->intersections->list, intersect);
    }
    // DER Encoding
    asn_enc_rval_t er;
    char buf[1024]; // Assume 1024 is enough
    er = der_encode_to_buffer(&asn_DEF_MapData, map, buf, sizeof(buf));
    if (er.encoded == -1) {
        asn_DEF_MapData.free_struct(&asn_DEF_MapData, map, 0);
        qDebug() << "Encoder error!";
        return QByteArray();
    }
    QByteArray data(buf, er.encoded);
    asn_DEF_MapData.free_struct(&asn_DEF_MapData, map, 0);
    return data;
}

bool MapMsg::DecodeFromMapMessage(QByteArray &data)
{
    MapData_t *map = nullptr;
    asn_dec_rval_t rval;
    rval = asn_DEF_MapData.ber_decoder(0, &asn_DEF_MapData, (void **)&map, data.data(), data.size(), 0);
    if(rval.code != RC_OK) {
        asn_DEF_MapData.free_struct(&asn_DEF_MapData, map, 0);
        return false;
    }
    Intersection_t **inters     = map->intersections->list.array;
    Intersection_t  *inter      = nullptr;
    int              intercount = map->intersections->list.count;

    ApproachObject_t **paths  = nullptr;
    ApproachObject_t  *path   = nullptr;
    int                pathcount  = 0;

    int id = -1;
    Path tmp;

    for (int i=0; i<intercount ; i++) {
        inter = inters[i];
        if (inter->id.size != 2) { // Temporary don't support other size (4)
            continue;
        }

        id = ntohs(*(short *)inter->id.buf);
        intersects[id].id = id;
        if (inter->refPoint != NULL) {
            intersects[id].center.latitude  = inter->refPoint->lat;
            intersects[id].center.longitude = inter->refPoint->Long;
        }
        paths = inter->approaches.list.array;
        pathcount = inter->approaches.list.count;

        for (int j=0; j<pathcount; j++) {
            path = paths[j];
            if (path->refPoint == NULL) {
                continue;
            }
            tmp.junction.latitude  = path->refPoint->lat;
            tmp.junction.longitude = path->refPoint->Long;
            tmp.lanewidth = 0;
            if (path->laneWidth != NULL) {
                tmp.lanewidth = *path->laneWidth;
            }

            if(path->approach != NULL && path->approach->drivingLanes != NULL)
            for(int k = 0; k < path->approach->drivingLanes->list.count; k++)
            {
                Lane lane;
                lane.laneNumber = path->approach->drivingLanes->list.array[k]->laneNumber.buf[0];
                lane.lanewidth = *path->approach->drivingLanes->list.array[k]->laneWidth;
                tmp.lanes.append(lane);
            }
            intersects[id].paths.append(tmp);
        }
    }
    asn_DEF_MapData.free_struct(&asn_DEF_MapData, map, 0);
    //Printf();
    return true;
}

void MapMsg::Printf()
{
    for (auto it : intersects) {
        qDebug() << "Intersection ID = " << it.id;
        qDebug() << "Intersection Center Point(Lat=" << it.center.latitude
                 << ", Lon=" << it.center.longitude << ")";
        qDebug() << "Paths :";
        for (auto iter : it.paths) {
            qDebug() << "\t Lat=" << iter.junction.latitude
                     << "Lon=" << iter.junction.longitude
                     << "LaneWidth=" << iter.lanewidth;
        }
    }

}

