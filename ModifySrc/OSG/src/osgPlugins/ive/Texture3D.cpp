/**********************************************************************
 *
 *    FILE:            Texture3D.cpp
 *
 *    DESCRIPTION:    Read/Write osg::Texture3D in binary format to disk.
 *
 *    CREATED BY:        Auto generated by iveGenerated
 *                    and later modified by Rune Schmidt Jensen.
 *
 *    HISTORY:        Created 20.3.2003
 *
 *    Copyright 2003 VR-C
 **********************************************************************/

#include "Exception.h"
#include "Texture3D.h"
#include "Texture.h"
#include "Image.h"

using namespace ive;

void Texture3D::write(DataOutputStream* out){
    // Write Texture3D's identification.
    out->writeInt(IVETEXTURE3D);
    // If the osg class is inherited by any other class we should also write this to file.
    osg::Texture*  tex = dynamic_cast<osg::Texture*>(this);
    if(tex){
        ((ive::Texture*)(tex))->write(out);
    }
    else
        out_THROW_EXCEPTION("Texture3D::write(): Could not cast this osg::Texture3D to an osg::Texture.");
    // Write Texture3D's properties.
    // Write image.

    // Should we include images date in stream
    out->writeImage(getImage());
}

void Texture3D::read(DataInputStream* in){
    // Read Texture3D's identification.
    int id = in->peekInt();
    if(id == IVETEXTURE3D){
        // Code to read Texture3D's properties.
        id = in->readInt();
        // If the osg class is inherited by any other class we should also read this from file.
        osg::Texture*  tex = dynamic_cast<osg::Texture*>(this);
        if(tex){
            ((ive::Texture*)(tex))->read(in);
        }
        else
            in_THROW_EXCEPTION("Texture3D::read(): Could not cast this osg::Texture3D to an osg::Texture.");
        // Read image.

        // Should we read image data from stream
        osg::Image *image = in->readImage();
        if(image) {
            setImage(image);
        }
    }
    else{
        in_THROW_EXCEPTION("Texture3D::read(): Expected Texture3D identification.");
    }
}
