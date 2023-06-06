#include "main.h"

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    Game_Display_Func[game_state]();
    glutSwapBuffers();
}

void Resize(int x, int y) {
    glutPositionWindow(pos_x, pos_y);
    glutReshapeWindow(width, height);
}

void Keyboard_Down(GLubyte key, int x, int y) {
    Game_Keyboard_Down_Func[game_state](key);
}

void Keyboard_Up(GLubyte key, int x, int y) {
    Game_Keyboard_Up_Func[game_state](key);
}

void Timer(int value) {
    Game_Process_Func[game_state]();
    glutTimerFunc(interval, Timer, 0);
}

int main(int argc, char** argv) {
    srand(time(NULL));

    IniG();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    pos_x = (glutGet(GLUT_SCREEN_WIDTH) - width) >> 1;
    pos_y = (glutGet(GLUT_SCREEN_HEIGHT) - height) >> 1;
    glutInitWindowPosition(pos_x, pos_y);
    glutInitWindowSize(width, height);
    glutCreateWindow("PROIECT PG");
    IniGL();
    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard_Down);
    glutKeyboardUpFunc(Keyboard_Up);
    glutTimerFunc(0, Timer, 0);
    glutMainLoop();
    return 0;
}