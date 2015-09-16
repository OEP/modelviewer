#include <string>
#include <sstream>
#include <iostream>
#include <boost/lexical_cast.hpp>

#include "model.h"

Face::Face():
    mModel(NULL), mBuffer()
{
}

Face::Face(const Model* model):
    mModel(model), mBuffer()
{
}

Face::Face(Face const& other):
    mModel(other.mModel), mBuffer(other.mBuffer)
{
}

void
Face::addCoordinate(CoordId v, CoordId vt, CoordId vn)
{
    mBuffer.push_back(v);
    mBuffer.push_back(vt);
    mBuffer.push_back(vn);
}

void
Face::getVertex(size_t i, Vertex &x, Vertex &y, Vertex &z) const
{
    size_t j = mBuffer[3 * i];
    mModel->getVertex(j, x, y, z);
}

static void
model_read_face(Model *model, std::istream& is)
{
    Face buffer(model);
    std::string face;

    /*
    * Loop over the faces. For each face, read the vertex, texture,
    * and normal coordinates.
    */
    while(is >> face) {
        CoordId coordinates[3];
        std::string idx;
        size_t i = 0;
        std::stringstream iss(face);

        while(i < 3 && std::getline(iss, idx, '/')) {
            if(idx == "") {
                coordinates[i] = -1; 
            }
            else {
                coordinates[i] = boost::lexical_cast<CoordId>(idx);
            }
            i++;
        }

        buffer.addCoordinate(coordinates[0], coordinates[1], coordinates[2]);
    }

    /* Now actually add them to the Model object's buffer */
    model->addFace(buffer);
}

Model::Model():
    mVertices(), mVertexNormals(), mVertexTexture()
{
}

Model*
Model::read(std::istream& is)
{
    Model *model = new Model();

    std::string line;
    std::string verb;

    while(std::getline(is, line)) {
        if(line[0] == '#') {
            continue;
        }
        if(line.length() == 0) {
            continue;
        }

        std::istringstream iss(line);
        iss >> verb;

        if(verb == "v") {
            Vertex x, y, z;
            iss >> x;
            iss >> y;
            iss >> z;
            model->addVertex(x, y, z);
        }
        else if(verb == "vt") {
            Vertex x, y;
            iss >> x;
            iss >> y;
            model->addVertexTexture(x, y);
        }
        else if(verb == "vn") {
            Vertex x, y, z;
            iss >> x;
            iss >> y;
            iss >> z;
            model->addVertexNormal(x, y, z);
        }
        else if(verb == "f") {
            model_read_face(model, iss);
        }
        else {
            /* TODO: Log unhandled verbs */
            std::cout << line << std::endl;
        }
    }

    return model;
}

void
Model::addVertex(Vertex x, Vertex y, Vertex z)
{
    mVertices.push_back(x);
    mVertices.push_back(y);
    mVertices.push_back(z);
}

void
Model::addVertexNormal(Vertex x, Vertex y, Vertex z)
{
    mVertexNormals.push_back(x);
    mVertexNormals.push_back(y);
    mVertexNormals.push_back(z);
}

void
Model::addVertexTexture(Vertex x, Vertex y)
{
    mVertexTexture.push_back(x);
    mVertexTexture.push_back(y);
}

void
Model::addFace(const Face& face)
{
    mFaces.push_back(face);
}

void
Model::getVertex(ssize_t i, Vertex &x, Vertex &y, Vertex &z) const
{
    x = mVertices[i * 3 + 0];
    y = mVertices[i * 3 + 1];
    z = mVertices[i * 3 + 2];
}
