/* -*-c++-*- */
/**
 * osgAudio - OpenSceneGraph Audio Library
 * (C) Copyright 2009-2012 byKenneth Mark Bryden
 * (programming by Chris 'Xenon' Hanson, AlphaPixel, LLC xenon at alphapixel.com)
 * based on a fork of:
 * Osg AL - OpenSceneGraph Audio Library
 * Copyright (C) 2004 VRlab, Ume� University
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * Please see COPYING file for special static-link exemption to LGPL.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef OSGAUDIO_FILESTREAM_H
#define OSGAUDIO_FILESTREAM_H 1

#include <osgAudio/Config.h>

#ifdef ENABLE_SUBSYSTEM_FMOD
#include <osgAudio/BackendFMOD/FileStreamFMOD.h>
#endif // ENABLE_SUBSYSTEM_FMOD

#ifdef ENABLE_SUBSYSTEM_OPENAL
#include <osgAudio/BackendOpenALpp/FileStreamOpenALpp.h>
#endif // ENABLE_SUBSYSTEM_OPENAL

#endif /* OSGAUDIO_FILESTREAM_H */
