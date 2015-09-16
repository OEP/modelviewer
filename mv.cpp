#include <iostream>
#include <fstream>
#include <cstdlib>

#include <GLUT/glut.h>

#include "model.h"

#define MV_NAME "mv"
#define MV_FOV 60.0
#define MV_ASPECT 1.6
#define MV_NEAR 0.1
#define MV_FAR 100.0

#define MV_HEIGHT 480
#define MV_WIDTH (MV_HEIGHT * MV_ASPECT)

Model *g_model;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    /* Set up perspective transform */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(MV_FOV, MV_ASPECT, MV_NEAR, MV_FAR);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1, 1, 1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glTranslatef(0, 0, -2);

    glBegin(GL_POINTS);
    Vertex x, y, z;
    for(auto it = g_model->begin_faces(); it < g_model->end_faces(); it++) {
        auto &face = *it;
        for(size_t i = 0; i < 3; i++) {
            face.getVertex(i, x, y, z);
            glVertex3f(x, y, z);
        }
    }
    glEnd();

    glFlush();
}

void print_usage(const char* arg0, std::ostream& os)
{
    os << "Usage: " << arg0 << " model" << std::endl;
}

void error(const char* prefix, const char *msg)
{
    std::cerr << MV_NAME << ": ";
    if(prefix) {
        std::cerr << prefix << ": ";
    }
    std::cerr << msg << std::endl;
}

void fail(const char* prefix, const char *msg)
{
    error(prefix, msg);
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    if(argc != 2) {
        print_usage(argv[0], std::cerr);
        return 1;
    }

    std::ifstream is(argv[1]);
    if(!is) {
        fail(argv[1], strerror(errno));
    }
    g_model = Model::read(is);

    glutCreateWindow("modelviewer");
    glutReshapeWindow(MV_WIDTH, MV_HEIGHT);

    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}
