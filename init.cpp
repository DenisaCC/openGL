
#include "main.h"

void Reload_Game() {
    clouds[0].Init(570.0f, 210.0f);
    clouds[1].Init(930.0f, 300.0f);
    clouds[2].Init(240.0f, 255.0f);

    characters[0].Init(0);
    characters[1].Init(1);
};

void Init_Numbers() {
    Image img;
    Load_Texture_Swap(&img, "images/numbers.png");
    for (int i = 0; i < 10; i++) {
        Crop_Image(&img, &img_numbers[i], i * 6, 0, 6, 7);
        Zoom_Image(&img_numbers[i], scale * 2);
    }
    Delete_Image(&img);
}

void Init_Game() {
    Load_Texture_Swap(&img_background, "images/b1.png");
    Zoom_Image(&img_background, scale);
    Load_Texture_Swap(&img_ground, "images/ground.png");
    Zoom_Image(&img_ground, scale);

    c_line::Load_Image();
    c_cloud::Load_Image();
    c_platformer::Load_Image();
    c_character::Load_Image();
    c_coin::Load_Image();

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < max_x; j++)
            map[i][j] = 1;
    for (int i = 2; i < max_y; i++)
        for (int j = 0; j < max_x; j++)
            map[i][j] = 0;

    platformers[0].Init(7, 5);
    platformers[1].Init(19, 5);
    platformers[2].Init(4, 9);
    platformers[3].Init(22, 9);
    platformers[4].Init(9, 13);
    platformers[5].Init(17, 13);

    Init_Numbers();

    Reload_Game();
}

void Init_GL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glutIgnoreKeyRepeat(GL_TRUE);
    glEnable(GL_TEXTURE_2D);

    game_state = game_play;
}
