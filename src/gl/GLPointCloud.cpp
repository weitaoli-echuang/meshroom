#include "GLPointCloud.hpp"
#include "io/AlembicImport.hpp"
#include <iostream>

namespace mockup
{

GLPointCloud::GLPointCloud()
    : GLDrawable(*_colorArray)
    , _pointPositions(QOpenGLBuffer::VertexBuffer)
    , _pointColors(QOpenGLBuffer::VertexBuffer)
    , _npoints(0)
{
    _vertexArrayObject.create();
}

void GLPointCloud::setRawPositions(const void* pointsBuffer, size_t npoints)
{
    // Allow only one load
    if(_npoints != 0)
        return;

    _vertexArrayObject.bind();
    if(_pointPositions.create())
    {
        _pointPositions.setUsagePattern(QOpenGLBuffer::StaticDraw);
        _pointPositions.bind();
        _npoints = npoints;
        _pointPositions.allocate(pointsBuffer, npoints * 3 * sizeof(float));
        glPointSize(3);
        _program.enableAttributeArray("in_position");
        _program.setAttributeBuffer("in_position", GL_FLOAT, 0, 3);
        _pointPositions.release();
    }
    else
    {
        std::cout << "unable to create buffer for point cloud" << std::endl;
    }
    _vertexArrayObject.release();
}

void GLPointCloud::setRawColors(const void* pointsBuffer, size_t npoints)
{
    _vertexArrayObject.bind();
    if(_pointColors.create())
    {
        _pointColors.setUsagePattern(QOpenGLBuffer::StaticDraw);
        _pointColors.bind();
        _pointColors.allocate(pointsBuffer, npoints * 3 * sizeof(float));
        _program.enableAttributeArray("in_color");
        _program.setAttributeBuffer("in_color", GL_FLOAT, 0, 3);
        _pointColors.release();
    }
    else
    {
        std::cout << "unable to create buffer for point cloud" << std::endl;
    }
    _vertexArrayObject.release();
}

void GLPointCloud::draw()
{
    if(_npoints)
    {
        _program.bind();
        _vertexArrayObject.bind();
        glDrawArrays(GL_POINTS, 0, _npoints);
        _vertexArrayObject.release();
        _program.release();
    }
}

} // namespace