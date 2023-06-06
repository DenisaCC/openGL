#pragma once
#pragma once
#ifndef MAIN_H
#define MAIN_H

#include "library/loadpng.h"
#include "library/process_image.h"
#include "library/GLTexture.h"

#include <GL/glut.h>


#include<math.h>
#include<time.h>
#include<vector>


#define width 880
#define height 464
#define start_x 80
#define start_y 80
#define cell_size 24
#define max_x 30
#define max_y 24
#define count_c 3
#define count_p 5
#define scale 3
#define interval 15
#define pi 3.141
#define rad 57.295

int pos_x, pos_y;

float boundary_left = start_x + 30.0f, boundary_right = start_x + 690.0f;
int game_state;
enum game_state { game_play };
float color_left_right[][3] = { {0.600f, 0.819f, 0.945f},
                                {0.345f, 0.561f, 0.698f} },
    color_bottom[] = { 0.267f, 0.267f,0.267f };
Rect rct_Left = { 0, start_x - 6.0f, start_y, height },
rct_White_Left = { start_x - 6.0f, start_x, start_y, height },
rct_Right = { width - start_x + 6.0f, width, start_y, height },
rct_White_Right{ width - start_x, width - start_x + 6.0f, start_y,height },
rct_Bottom = { 0, width, 0, start_y - 6.0f },
rct_White_Bottom = { 0, width, start_y - 6.0f, start_y };

Image img_numbers[10];
Image img_background, img_ground;
Rect rct_background = { start_x,720 + start_x,48 + start_y,384 + start_y },
rct_ground = { start_x,720 + start_x,start_y,48 + start_y };

float color_white[] = { 1.0f,1.0f,1.0f };
float gravity = -1.2f; 
int map[max_y][max_x];

class c_line {
public:
    static Image img_save[2];
    static Rect rct;

    Image* img;
    float x, y, angle;
    int player;

    c_line(int _player, float _x, float _y, float _angle) {
        player = _player;
        x = _x;
        y = _y;
        angle = _angle;
        img = &img_save[player];
    }

    void Draw() {
        glTranslatef(x, y, 0.0f);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);
        Map_Texture(img);
        Draw_Rect(&rct);
        glLoadIdentity();
    }

    static void Load_Image() {
        Image img;
        Load_Texture(&img, "images/lines.png");
        Crop_Image(&img, &img_save[0], 0, 4, 8, 4);
        Crop_Image(&img, &img_save[1], 0, 0, 8, 4);
        Zoom_Image(&img_save[0], scale);
        Zoom_Image(&img_save[1], scale);
        Delete_Image(&img);

        rct.Left = -12.0f;
        rct.Right = 12.0f;
        rct.Bottom = -8.0f;
        rct.Top = 8.0f;
    }
};

Image c_line::img_save[2];
Rect c_line::rct;
std::vector<c_line>lines;


class c_cloud {
public:
    static Image img_save;
    Rect rct;
    Image* img;
    float x, y;
    void Init(float _x, float _y) {
        img = &img_save;
        x = _x + start_x;
        y = _y + start_y;
        Update_Rect();
        rct.Bottom = y;
        rct.Top = rct.Bottom + img->h;
    }

    void Draw() {
        Map_Texture(img);
        Draw_Rect(&rct);
    }

    void Update_Rect() {
        rct.Left = x - img->w / 2;
        rct.Right = rct.Left + img->w;
    }

    void Update() {
        x -= 0.3f;
        if (x < -120.0f)
            x += 1080.0f;
        Update_Rect();
    }

    static void Load_Image() {
        Load_Texture_Swap(&img_save, "images/cloud.png");
        Zoom_Image(&img_save, scale);
    }
};
Image c_cloud::img_save;
c_cloud clouds[count_c];

class c_platformer {
public:
    static Image img_save;

    Rect rct;
    Image* img;

    void Init(int _x, int _y) {
        map[_y][_x] = map[_y][_x + 1] = map[_y][_x + 2] = map[_y][_x + 3] = 1;
        img = &img_save;
        float x = start_x + (_x + 2) * cell_size;
        float y = start_y + _y * cell_size;
        rct.Left = x - img->w / 2;
        rct.Right = rct.Left + img->w;
        rct.Bottom = y;
        rct.Top = rct.Bottom + img->h;
    }

    void Draw() {
        Map_Texture(img);
        Draw_Rect(&rct);
    }

    static void Load_Image() {
        Load_Texture_Swap(&img_save, "images/p.png");
        Zoom_Image(&img_save, scale);
    }
};

Image c_platformer::img_save;
c_platformer platformers[count_p];

class c_coin {
public:
    static Image img_save;
    static float ax_base, ay_base;
    static float vx_max, vy_max;

    Rect rct;
    Image* img;
    float x, y, vx, vy, ax, ay, Scale;
    int timer, anim, region;
    bool is_Alive;

    c_coin(float _x, float _y, int _region) {
        x = _x + rand() % 41 - 20 + start_x;
        y = _y + rand() % 41 - 20 + start_y;
        vx = vx_max;
        vy = 0.0f;
        ax = ax_base;
        ay = ay_base;

        region = _region;
        timer = 0;
        anim = 0;
        Scale = 0.0f;
        is_Alive = false;
        img = &img_save;

    }

    void Update_Rect() {
        rct.Left = x - img->w / 2 * Scale;
        rct.Right = rct.Left + img->w * Scale;
        rct.Bottom = y - img->h / 2 * Scale;
        rct.Top = rct.Bottom + img->h * Scale;
    }

    void Draw() {
        Map_Texture(img);
        Draw_Rect(&rct);
    }

    void Update() {
        if (!is_Alive) {
            Scale += 0.05f;
            if (Scale >= 1.0f) {
                Scale = 1.0f;
                is_Alive = true;
            }
        }
        x += vx;
        y += vy;
        vx += ax;
        vy += ay;

        if (vx >= vx_max || vx <= -vx_max)
            ax = vx < 0 ? ax_base : -ax_base;
        if (vy >= vy_max || vy <= -vy_max)
            ay = vy < 0 ? ay_base : -ay_base;

        timer++;
        if (timer == 6) {
            timer = 0;
            anim = 1 - anim;
            img = &img_save;
        }
        Update_Rect();
    }

    bool is_Caught(float _x, float _y)
    {
        if (_x - 20.0f < x && _x + 20.0f > x && _y - 6.0f < y && _y + 34.0f > y)
            return true;
        return false;
    }

    static void Load_Image() {   
        Load_Texture(&img_save, "images/c2 .png");
        Zoom_Image(&img_save, scale);
    }
};

Image c_coin::img_save;
float c_coin::ax_base = 0.015f, c_coin::ay_base = 0.02f;
float c_coin::vx_max = 0.2f, c_coin::vy_max = 0.4f;
std::vector<c_coin> coins;

class c_point {
public:
    float x, y;

    c_point(float _x, float _y) {
        x = _x;
        y = _y;
    }
};

class c_spawn_coins {
public:
    c_point Spawn_Points[6] = {
        c_point(100.0f,300.0f), c_point(620.0f, 300.0f),
        c_point(360.0f,280.0f), c_point(360.0f, 130.0f),
        c_point(100.0f,120.0f), c_point(620.0f, 120.0f) };

    int max_coins, count_spawn_points, timer;

    c_spawn_coins(int _max_coins) {
        max_coins = _max_coins;
        timer = 60;
        count_spawn_points = sizeof(Spawn_Points) / sizeof(c_point);
    }
    void Update() {
        timer++;
        if (timer == 90) {
            timer = 0;
            if (coins.size() < max_coins) {
                bool check;
                int region;
                do {
                    check = false;
                    region = rand() % count_spawn_points;
                    for (c_coin coin : coins)
                        if (coin.region == region) {
                            check = true;
                            break;
                        }
                } while (check);
                coins.push_back(c_coin(Spawn_Points[region].x, Spawn_Points[region].y, region));
            }
        }
    }
};
c_spawn_coins spawn_coins(2);

class c_character {
public:
    static Image img_save[2][2][2];
    static Image img_button_save[2], img_button_shadow;
    static float map_offset[2];
    static float map_base_angle[2];

    Rect rct, rct_button, rct_button_shadow, rct_score;
    Image* img, * img_button, * img_score;

    float x, y, vx, vy, angle;
    int player, drt, anim, prepare_stt, angle_drt, score;
    bool is_jumping, is_jump_pressed;

    static bool (*check_boundary[2])(float x);
    static bool (*check_angle[2][2])(float angle);

    void Init(int _player) {
        player = _player;
        drt = 1 - player;
        anim = 0;
        float offset = 11.0f * cell_size * (player == 0 ? -1 : 1);
        x = width / 2 + offset;
        y = cell_size * 2.0f + start_y;
        prepare_stt = 0;
        is_jumping = false;
        is_jump_pressed = false;
        Update_Image();
        Update_Rect();
        offset *= 1.2f;
        img_button = &img_button_save[player];
        rct_button.Left = width / 2 + offset - img_button->w / 2;
        rct_button.Right = rct_button.Left + img_button->w;
        rct_button.Bottom = 24.0f;
        rct_button.Top = rct_button.Bottom + img_button->h;

        rct_button_shadow.Left = rct_button.Left + 12.0f;
        rct_button_shadow.Right = rct_button.Right + 12.0f;
        rct_button_shadow.Bottom = rct_button.Bottom - 12.0f;
        rct_button_shadow.Top = rct_button.Top - 12.0f;

        score = 0;

        offset *= 0.4f;
        img_score = &img_numbers[score];
        rct_score.Left = width / 2 + offset - img_score->w / 2;
        rct_score.Right = rct_score.Left + img_score->w;
        rct_score.Bottom = 18.0f;
        rct_score.Top = rct_score.Bottom + img_score->h;
       
    }

    void Update_Image() {
        img = &img_save[player][drt][anim];
    }

    void Update_Rect() {
        rct.Left = x - img->w / 2;
        rct.Right = rct.Left + img->w;
        rct.Bottom = y;
        rct.Top = rct.Bottom + img->h;
    }

    void Draw() {
        Map_Texture(img);
        Draw_Rect(&rct);
    }

    void Draw_Button() {
        Map_Texture(&img_button_shadow);
        Draw_Rect(&rct_button_shadow);
        Map_Texture(img_button);
        Draw_Rect(&rct_button);
    }

    void Draw_Score() {
        Map_Texture(img_score);
        Draw_Rect(&rct_score);
    }

    void Key_Down() {
        is_jump_pressed = true;
        rct_button.Left += 4.0f;
        rct_button.Right += 4.0f;
        rct_button.Bottom -= 4.0f;
        rct_button.Top -= 4.0f;
    }

    void Key_Up() {
        is_jump_pressed = false;
        rct_button.Left -= 4.0f;
        rct_button.Right -= 4.0f;
        rct_button.Bottom += 4.0f;
        rct_button.Top += 4.0f;
    }

    void Prepare_Start() {
        if (prepare_stt == 0) {
            prepare_stt = 1;
            angle_drt = drt;
            angle = map_base_angle[drt];
        }
    }

    void Prepare_End() {
        if (prepare_stt == 1)
            prepare_stt = 2;
    }

    void Jump() {
        if (!is_jumping) {
            is_jumping = true;
            anim = 1;
            Update_Image();
        }
    }
    void Update() {
       
        if (!is_jumping) {
            if (is_jump_pressed)
                Prepare_Start();
            else
                Prepare_End();
        }

        if (!is_jumping) {
            if (prepare_stt > 0) {
                if (prepare_stt == 2) {
                    prepare_stt = 0;
                    Jump();
                }
                else {
                    angle += map_offset[angle_drt];
                    if (check_angle[drt][angle_drt](angle))
                        angle_drt = 1 - angle_drt;

                    float angle2 = angle / rad;
                    float x2 = x, y2 = y + 4.0f, vx2, vy2;
                    vx2 = cos(angle2) * 4 + (drt == 0 ? angle2 - pi : angle2) * 9;
                    vy2 = sin(angle2) * 21;
                    vx = vx2;
                    vy = vy2;
                    for (int i = 0; i < 18; i++) {
                        x2 += vx2;
                        y2 += vy2;
                        if (i % 3 == 2) {
                            angle2 = atan2(vy2, vx2) * rad;
                            lines.push_back(c_line(player, x2, y2, angle2));
                        }
                        vy2 += gravity;
                    }
                }
            }
        }
        else {
            float y_old = y;
            x += vx;
            y += vy;
            vy += gravity;

            if (vy < -24.0f)
                vy = -24.0f;

            if (vy <= 0.0f) {
                int col1 = (x - start_x - 9.0f) / cell_size;
                int col2 = (x - start_x + 9.0f) / cell_size;
                int row_old = (y_old - start_y) / cell_size;
                int row = (y - start_y) / cell_size;
                if ((!map[row_old][col1] && !map[row_old][col2]) && (map[row][col1] || map[row][col2])) {
                    is_jumping = false;
                    y = (row + 1) * cell_size + start_y;
                    vx = 0.0f;
                    vy = 0.0f;
                    anim = 0;
                    Update_Image();
                }
            }

            if (check_boundary[drt](x)) {
                drt = 1 - drt;
                vx = -vx;
                Update_Image();
            }

            std::vector<c_coin>::iterator it = coins.begin();
            while (it != coins.end()) {
                if (it->is_Caught(x, y)) {
                    it = coins.erase(it);
                    score++;
                    img_score = &img_numbers[score];
                }
                else
                    it++;
            }
            if (score == 9)
            {
                printf("Game over!!");

            }
            Update_Rect();
          
        }
        
    }
    
    static bool check_boundary_left(float x) {
        return x < boundary_left;

    }

    static bool check_boundary_right(float x) {
        return x > boundary_right;
    }

    static bool check_angle_left_decrease(float angle) {
        return angle <= 110.0f;
    }

    static bool check_angle_left_increase(float angle) {
        return angle >= 160.0f;
    }

    static bool check_angle_right_decrease(float angle) {
        return angle <= 20.0f;
    }

    static bool check_angle_right_increase(float angle) {
        return angle >= 70.0f;
    }

    static void Load_Image() {
        Image img;
        Load_Texture(&img, "images/c1.png");
        Crop_Image(&img, &img_save[0][1][0], 0, 0, 18, 16);
        Crop_Image(&img, &img_save[0][1][1], 0, 16, 18, 16);
        Crop_Image(&img, &img_save[1][1][0], 18, 0, 18, 16);
        Crop_Image(&img, &img_save[1][1][1], 18, 16, 18, 16);
        Swap_Image(img_save[0][1][0].img, 18, 16);
        Swap_Image(img_save[0][1][1].img, 18, 16);
        Swap_Image(img_save[1][1][0].img, 18, 16);
        Swap_Image(img_save[1][1][1].img, 18, 16);
        Zoom_Image(&img_save[0][1][0], scale);
        Zoom_Image(&img_save[0][1][1], scale);
        Zoom_Image(&img_save[1][1][0], scale);
        Zoom_Image(&img_save[1][1][1], scale);
        Flip_Horizontal(&img_save[0][1][0], &img_save[0][0][0]);
        Flip_Horizontal(&img_save[0][1][1], &img_save[0][0][1]);
        Flip_Horizontal(&img_save[1][1][0], &img_save[1][0][0]);
        Flip_Horizontal(&img_save[1][1][1], &img_save[1][0][1]);
        Delete_Image(&img);

        Load_Texture(&img, "images/b2.png");
        Crop_Image(&img, &img_button_save[0], 0, 0, 44, 24);
        Crop_Image(&img, &img_button_save[1], 0, 24, 44, 24);
        Swap_Image(img_button_save[0].img, 44, 24);
        Swap_Image(img_button_save[1].img, 44, 24);
        Zoom_Image(&img_button_save[0], scale);
        Zoom_Image(&img_button_save[1], scale);

        unsigned char Color_Shadow[] = { 0, 0, 0, 50 };
        Clone_Image_Shadow(&img_button_save[0], &img_button_shadow, Color_Shadow);
        Delete_Image(&img);
    }
  
};

Image c_character::img_save[2][2][2], c_character::img_button_save[2], c_character::img_button_shadow;
float c_character::map_offset[2] = { -1.0f,1.0f };
float c_character::map_base_angle[2] = { 160.0f, 20.0f };
bool(*c_character::check_boundary[2])(float x) = { c_character::check_boundary_left, c_character::check_boundary_right };
bool (*c_character::check_angle[2][2])(float angle) = {
    {check_angle_left_decrease, check_angle_left_increase},
    {check_angle_right_decrease, check_angle_right_increase} };

c_character characters[2];

void Fr() {
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glColor3fv(color_left_right[0]);
    glVertex2f(rct_Left.Left, rct_Left.Bottom);
    glVertex2f(rct_Left.Right, rct_Left.Bottom);
    glColor3fv(color_left_right[1]);
    glVertex2f(rct_Left.Right, rct_Left.Top);
    glVertex2f(rct_Left.Left, rct_Left.Top);

    glColor3fv(color_left_right[0]);
    glVertex2f(rct_Right.Left, rct_Right.Bottom);
    glVertex2f(rct_Right.Right, rct_Right.Bottom);
    glColor3fv(color_left_right[1]);
    glVertex2f(rct_Right.Right, rct_Right.Top);
    glVertex2f(rct_Right.Left, rct_Right.Top);

    glColor3fv(color_bottom);
    glVertex2f(rct_Bottom.Left, rct_Bottom.Bottom);
    glVertex2f(rct_Bottom.Right, rct_Bottom.Bottom);
    glVertex2f(rct_Bottom.Right, rct_Bottom.Top);
    glVertex2f(rct_Bottom.Left, rct_Bottom.Top);

    glColor3fv(color_white);
    glVertex2f(rct_White_Left.Left, rct_White_Left.Bottom);
    glVertex2f(rct_White_Left.Right, rct_White_Left.Bottom);
    glVertex2f(rct_White_Left.Right, rct_White_Left.Top);
    glVertex2f(rct_White_Left.Left, rct_White_Left.Top);

    glVertex2f(rct_White_Right.Left, rct_White_Right.Bottom);
    glVertex2f(rct_White_Right.Right, rct_White_Right.Bottom);
    glVertex2f(rct_White_Right.Right, rct_White_Right.Top);
    glVertex2f(rct_White_Right.Left, rct_White_Right.Top);

    glVertex2f(rct_White_Bottom.Left, rct_White_Bottom.Bottom);
    glVertex2f(rct_White_Bottom.Right, rct_White_Bottom.Bottom);
    glVertex2f(rct_White_Bottom.Right, rct_White_Bottom.Top);
    glVertex2f(rct_White_Bottom.Left, rct_White_Bottom.Top);
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

void GDP() {
    glLoadIdentity();

    Map_Texture(&img_background);
    Draw_Rect(&rct_background);

    for (int i = 0; i < count_c; i++)
        clouds[i].Draw();

    for (c_line Line : lines)
        Line.Draw();

    Map_Texture(&img_ground);
    Draw_Rect(&rct_ground);

    for (int i = 0; i < count_p; i++)
        platformers[i].Draw();

    int Size = coins.size();
    for (int i = 0; i < Size; i++)
        coins[i].Draw();

    characters[0].Draw();
    characters[1].Draw();

    Fr();

    characters[0].Draw_Button();
    characters[1].Draw_Button();

    characters[0].Draw_Score();
    characters[1].Draw_Score();
}


void GPP() {
    for (int i = 0; i < count_c; i++)
        clouds[i].Update();

   int Size = coins.size();
  for (int i = 0; i < Size; i++)
        coins[i].Update();

    lines.clear();
    characters[0].Update();
    characters[1].Update();

    spawn_coins.Update();

    glutPostRedisplay();
}


void GKDP(GLubyte& key) {
    switch (key) {
    case 32:
        characters[0].Key_Down();
        break;
    case 13:
        characters[1].Key_Down();
        break;
    }
}

void GKUP(GLubyte& key) {
    switch (key) {
    case 32:
        characters[0].Key_Up();
        break;
    case 13:
        characters[1].Key_Up();
        break;
    }
}

void RG() {
    clouds[0].Init(570.0f, 210.0f);
    clouds[1].Init(930.0f, 300.0f);
    clouds[2].Init(240.0f, 255.0f);

    characters[0].Init(0);
    characters[1].Init(1);
};

void IniN() {
    Image img;
    Load_Texture_Swap(&img, "images/numbers.png");
    for (int i = 0; i < 10; i++) {
        Crop_Image(&img, &img_numbers[i], i * 6, 0, 5, 7);
        Zoom_Image(&img_numbers[i], scale * 2);
    }
    Delete_Image(&img);
}

void IniG() {
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

    platformers[0].Init(5, 5);
    platformers[1].Init(21, 5);
    platformers[2].Init(13, 9);
    platformers[3].Init(5, 13);
    platformers[4].Init(21, 13);
  

    IniN();

    RG();
}

void IniGL() {
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



void (*Game_Display_Func[])() = { GDP };
void (*Game_Process_Func[])() = { GPP };
void (*Game_Keyboard_Down_Func[])(GLubyte& key) = { GKDP };
void (*Game_Keyboard_Up_Func[])(GLubyte& key) = { GKUP };


#endif