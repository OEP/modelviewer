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

struct vector {
    GLfloat x, y, z;
};

Model *g_model;
struct vector g_translate = {0, 0, -2};
struct vector g_rotate = {0, 0, 0};
struct vector g_scale = {1, 1, 1};

struct vector g_last_mouse_pos = {0, 0, 0};

enum CameraState { NONE, TUMBLE, PAN, ZOOM };
CameraState g_camera_state = NONE;

GLenum g_primitive = GL_POINTS;

void error(const char*, const char*);

void reset_view()
{
    g_rotate.x = 0;
    g_rotate.y = 0;
    g_rotate.z = 0;
    g_scale.x = 1;
    g_scale.y = 1;
    g_scale.z = 1;
    g_translate.x = 0;
    g_translate.y = 0;
    g_translate.z = -2;
    g_primitive = GL_POINTS;
    glutPostRedisplay();
}

void change_primitive(GLenum primitive)
{
    g_primitive = primitive;
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN && g_camera_state == NONE) {
        switch(button) {
        case GLUT_LEFT_BUTTON:
            g_camera_state = TUMBLE;
            break;
        default:
            break;
        }
        return;
    }

    /*
     * The remainder of code will unset the camera state only if the button is
     * being released.
     */
    if(state != GLUT_UP) {
        return;
    }

    if(g_camera_state == TUMBLE && button == GLUT_LEFT_BUTTON) {
        g_camera_state = NONE;
    }
}

void update_mouse_position(int x, int y)
{
    g_last_mouse_pos.x = x;
    g_last_mouse_pos.y = y;
}

void motion(int x, int y)
{
    int dx = x - g_last_mouse_pos.x;
    int dy = y - g_last_mouse_pos.y;
    switch(g_camera_state) {
    case TUMBLE:
        g_rotate.y += dx;
        g_rotate.x += dy;
        glutPostRedisplay();
        break;
    case PAN:
    case ZOOM:
    case NONE:
        break;
    }
    update_mouse_position(x, y);
}

void passive_motion(int x, int y)
{
    update_mouse_position(x, y);
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key) {
        case 'r':
            reset_view();
            break;
        case '1':
            change_primitive(GL_POINTS);
            break;
        case '2':
            change_primitive(GL_LINES);
            break;
        case '3':
            change_primitive(GL_LINE_STRIP);
            break;
        case '4':
            change_primitive(GL_LINE_LOOP);
            break;
        case '5':
            change_primitive(GL_TRIANGLES);
            break;
        case '6':
            change_primitive(GL_TRIANGLE_STRIP);
            break;
        case '7':
            change_primitive(GL_TRIANGLE_FAN);
            break;
        case '8':
            change_primitive(GL_QUADS);
            break;
        case '9':
            change_primitive(GL_QUAD_STRIP);
            break;
        case '0':
            change_primitive(GL_POLYGON);
            break;
        default:
            error(NULL, "Unhandled key");
            break;
    }
}

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

    glTranslatef(g_translate.x, g_translate.y, g_translate.z);
    glRotatef(g_rotate.x, 1, 0, 0);
    glRotatef(g_rotate.y, 0, 1, 0);
    glRotatef(g_rotate.z, 0, 0, 1);
    glScalef(g_scale.x, g_scale.y, g_scale.z);

    Vertex x, y, z;
    for(auto it = g_model->begin_faces(); it < g_model->end_faces(); it++) {
        auto &face = *it;
        glBegin(g_primitive);
        for(size_t i = 0; i < face.size(); i++) {
            face.getVertex(i, x, y, z);
            glVertex3f(x, y, z);
        }
        glEnd();
    }

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
    exit(EXIT_FAILURE);
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
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passive_motion);

    glutMainLoop();

    return 0;
}
