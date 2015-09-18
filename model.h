#pragma once
#include <vector>
#include <istream>

class Model;
class Face;

typedef float Vertex;
typedef std::vector<Vertex> VertexBuffer;

typedef ssize_t CoordId;
typedef std::vector<CoordId> CoordIdBuffer;
typedef std::vector<Face> FaceBuffer;


class Face
{
public:
    Face();
    Face(const Model*);
    Face(Face const&);

    size_t size() const { return mBuffer.size() / 3; }

    void addCoordinate(CoordId v, CoordId vt, CoordId vn);
    void getVertex(size_t, Vertex&, Vertex&, Vertex&) const;

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

    void getVertex(ssize_t, Vertex&, Vertex&, Vertex&) const;
    void getCentroid(Vertex&, Vertex&, Vertex&) const;
    void getLength(Vertex&, Vertex&, Vertex&) const;
    size_t size() const { return mVertices.size(); }

    FaceBuffer::const_iterator begin_faces() const { return mFaces.begin(); }
    FaceBuffer::const_iterator end_faces() const { return mFaces.end(); }

private:
    Model();

    VertexBuffer mVertices;
    VertexBuffer mVertexNormals;
    VertexBuffer mVertexTexture;
    FaceBuffer mFaces;
    Vertex mSumX, mSumY, mSumZ;
    Vertex mMinX, mMinY, mMinZ;
    Vertex mMaxX, mMaxY, mMaxZ;
};
