#pragma once
#include <vector>
#include <istream>

typedef float Vertex;
typedef std::vector<Vertex> VertexBuffer;

typedef ssize_t CoordId;
typedef std::vector<CoordId> CoordIdBuffer;

class Model;

class Face
{
public:
    Face();
    Face(const Model*);
    Face(Face const&);

    size_t size() const { return mBuffer.size() / 3; }

    void addCoordinate(CoordId v, CoordId vt, CoordId vn);

private:
    const Model* mModel;
    CoordIdBuffer mBuffer;
};

class Model
{
public:
    /* Public static methods */
    static Model* read(std::istream& is);

    /* Public instance methods */
    void addVertex(Vertex x, Vertex y, Vertex z);
    void addVertexNormal(Vertex x, Vertex y, Vertex z);
    void addVertexTexture(Vertex x, Vertex y);
    void addFace(const Face& face);

private:
    Model();

    VertexBuffer mVertices;
    VertexBuffer mVertexNormals;
    VertexBuffer mVertexTexture;
    std::vector<Face> mFaces;
};
