#include <iostream>
#include <fstream>

#include <GLUT/glut.h>

#include "model.h"

Model *g_model;

void display()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_TRIANGLES);
    Vertex x, y, z;
    for(auto it = g_model->begin_faces(); it < g_model->end_faces(); it++) {
        auto &face = *it;
        for(size_t i = 0; i < 3; i++) {
            face.getVertex(i, x, y, z);
            glVertex3f(x, y, z);
        }
    }
    glEnd();
}

void reshape(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("modelviewer");

    std::ifstream is("models/unitsphere.obj");
    g_model = Model::read(is);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}
