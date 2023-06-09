/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/

#include <osg/ComputeBoundsVisitor>
#include <osg/Transform>
#include <osg/Drawable>
#include <osg/Geode>

using namespace osg;

ComputeBoundsVisitor::ComputeBoundsVisitor(TraversalMode traversalMode):
    osg::NodeVisitor(traversalMode)
{
}

void ComputeBoundsVisitor::reset()
{
    _matrixStack.clear();
    _bb.init();
}

void ComputeBoundsVisitor::getPolytope(osg::Polytope& polytope, float margin) const
{
    float delta = _bb.radius()*margin;
    polytope.add( osg::Plane(0.0, 0.0, 1.0, -(_bb.zMin()-delta)) );
    polytope.add( osg::Plane(0.0, 0.0, -1.0, (_bb.zMax()+delta)) );

    polytope.add( osg::Plane(1.0, 0.0, 0.0, -(_bb.xMin()-delta)) );
    polytope.add( osg::Plane(-1.0, 0.0, 0.0, (_bb.xMax()+delta)) );

    polytope.add( osg::Plane(0.0, 1.0, 0.0, -(_bb.yMin()-delta)) );
    polytope.add( osg::Plane(0.0, -1.0, 0.0, (_bb.yMax()+delta)) );
}

void ComputeBoundsVisitor::getBase(osg::Polytope& polytope, float margin) const
{
    float delta = _bb.radius()*margin;
    polytope.add( osg::Plane(0.0, 0.0, 1.0, -(_bb.zMin()-delta)) );
}

void ComputeBoundsVisitor::apply(osg::Transform& transform)
{
    osg::Matrix matrix;
    if (!_matrixStack.empty()) matrix = _matrixStack.back();

    transform.computeLocalToWorldMatrix(matrix,this);

    pushMatrix(matrix);

    traverse(transform);

    popMatrix();
}

void ComputeBoundsVisitor::apply(osg::Drawable& drawable)
{
    applyBoundingBox(drawable.getBoundingBox());
}

void ComputeBoundsVisitor::applyBoundingBox(const osg::BoundingBox& bbox)
{
    if (_matrixStack.empty()) _bb.expandBy(bbox);
    else if (bbox.valid())
    {
        const osg::Matrix& matrix = _matrixStack.back();
        _bb.expandBy(bbox.corner(0) * matrix);
        _bb.expandBy(bbox.corner(1) * matrix);
        _bb.expandBy(bbox.corner(2) * matrix);
        _bb.expandBy(bbox.corner(3) * matrix);
        _bb.expandBy(bbox.corner(4) * matrix);
        _bb.expandBy(bbox.corner(5) * matrix);
        _bb.expandBy(bbox.corner(6) * matrix);
        _bb.expandBy(bbox.corner(7) * matrix);
    }
}
