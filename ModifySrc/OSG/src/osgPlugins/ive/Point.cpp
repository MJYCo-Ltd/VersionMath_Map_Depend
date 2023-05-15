/**********************************************************************
 *
 *    FILE:            Point.cpp
 *
 *    DESCRIPTION:    Read/Write osg::Point in binary format to disk.
 *
 *    CREATED BY:        Auto generated by iveGenerator
 *                    and later modified by Rune Schmidt Jensen.
 *
 *    HISTORY:        Created 27.3.2003
 *
 *    Copyright 2003 VR-C
 **********************************************************************/

#include "Exception.h"
#include "Point.h"
#include "Object.h"

using namespace ive;

void Point::write(DataOutputStream* out){
    // Write CullFace's identification.
    out->writeInt(IVEPOINT);
    // If the osg class is inherited by any other class we should also write this to file.
    osg::Object*  obj = dynamic_cast<osg::Object*>(this);
    if(obj){
        ((ive::Object*)(obj))->write(out);
    }
    else
        out_THROW_EXCEPTION("Point::write(): Could not cast this osg::Point to an osg::Object.");
    // Write Point's properties.
    out->writeFloat(getSize());
    out->writeFloat(getFadeThresholdSize());
    out->writeVec3(getDistanceAttenuation());
    out->writeFloat(getMinSize());
    out->writeFloat(getMaxSize());
}

void Point::read(DataInputStream* in){
    // Peek on Point's identification.
    int id = in->peekInt();
    if(id == IVEPOINT){
        // Read Point's identification.
        id = in->readInt();
        // If the osg class is inherited by any other class we should also read this from file.
        osg::Object*  obj = dynamic_cast<osg::Object*>(this);
        if(obj){
            ((ive::Object*)(obj))->read(in);
        }
        else
            in_THROW_EXCEPTION("Point::read(): Could not cast this osg::Point to an osg::Object.");
        // Read Point's properties
        setSize(in->readFloat());
        setFadeThresholdSize(in->readFloat());
        setDistanceAttenuation(in->readVec3());
        setMinSize(in->readFloat());
        setMaxSize(in->readFloat());
    }
    else{
        in_THROW_EXCEPTION("Point::read(): Expected Point identification.");
    }
}
