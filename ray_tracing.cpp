#include <GL/glut.h>
#include <cmath>
#define WIDTH 1920
#define HEIGHT 1080

int xLight = 10, yLight = 10;
int xObj = 1000, yObj = 500, rObj = 50;
float luminosity = 0;
unsigned int maxCollisions = 0;
float wall_reflection_coefficient = 1.0f;

void drawCircle(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 3 - 2 * r;

    while (x <= y) {
        glBegin(GL_POINTS);
        glVertex2i(xc + x, yc + y);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc + x, yc - y);
        glVertex2i(xc - x, yc - y);
        glVertex2i(xc + y, yc + x);
        glVertex2i(xc - y, yc + x);
        glVertex2i(xc + y, yc - x);
        glVertex2i(xc - y, yc - x);
        glEnd();

        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
            d = d + 4 * x + 6;
    }
}

static void illuminate() {
    glColor3f(1, 1, 1);
    drawCircle(xObj, yObj, rObj);

    glBegin(GL_POINTS);
    for (float theta = 0; theta < 2 * 3.14159f; theta += 5 / (float)WIDTH) {
        float x = xLight, y = yLight;
        float dx = cos(theta), dy = sin(theta);
        unsigned int collisionCount = 0;

        while (((x - xObj) * (x - xObj) + (y - yObj) * (y - yObj) > rObj * rObj) && collisionCount <= maxCollisions) {
            if (x <= 0) {
                x = 1;
                dx *= -1;
                collisionCount++;
            }
            if (x >= WIDTH) {
                x = WIDTH - 1;
                dx *= -1;
                collisionCount++;
            }
            if (y <= 0) {
                y = 1;
                dy *= -1;
                collisionCount++;
            }
            if (y >= HEIGHT) {
                y = HEIGHT - 1;
                dy *= -1;
                collisionCount++;
            }
            glColor4f(1, 1, 0, luminosity * pow(wall_reflection_coefficient, collisionCount));
            glVertex2i(x, y);
            x += dx;
            y += dy;
        }
    }
    glEnd();
}

static void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    illuminate();
    
    glutSwapBuffers();
}

static void passiveMotion(int x, int y) {
    xLight = x;
    yLight = HEIGHT - y;
    glutPostRedisplay();
}

static void keyboard(unsigned char key, int x, int y) {
    if (key == 'c')
        maxCollisions++;
    if (key == 'C' && maxCollisions > 0)
        maxCollisions--;
    if (key == 'r' && wall_reflection_coefficient < 1)
        wall_reflection_coefficient += 0.1;
    if (key == 'R' && wall_reflection_coefficient > 0)
        wall_reflection_coefficient -= 0.1;
    glutPostRedisplay();
}

static void specialKeyboard(int key, int x, int y) {
    if (key == GLUT_KEY_UP)
        luminosity += 0.01f;
    if (key == GLUT_KEY_DOWN)
        luminosity -= 0.01f;
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Ray Tracing");
    gluOrtho2D(0, WIDTH, 0, HEIGHT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glutDisplayFunc(display);

    glutPassiveMotionFunc(passiveMotion);

    glutKeyboardFunc(keyboard);

    glutSpecialFunc(specialKeyboard);

    glutMainLoop();

    return 0;
}