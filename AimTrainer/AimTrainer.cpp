#define OLC_PGE_APPLICATION
#include <iostream>
#include "olcPixelGameEngine.h"
#include <vector>
using namespace std;
using namespace olc;

struct v2 {
    float x;
    float y;

    float distance() {
        return sqrt(x * x + y * y);
    }

    v2(int a, int b) {
        x = a;
        y = b;
    }

    v2(float a, float b) {
        x = a;
        y = b;
    }

    v2() {

    }

    v2 makeunit() {
        v2 temp;
        float d = distance();
        temp.x = x / d;
        temp.y = y / d;
        return temp;
    }

    v2 operator -(v2 other) {
        v2 temp;
        temp.x = x - other.x;
        temp.y = y - other.y;
        return temp;
    }

    v2 operator +(v2 other) {
        v2 temp;
        temp.x = x + other.x;
        temp.y = y + other.y;
        return temp;
    }

    v2 operator /(float other) {
        v2 temp;
        temp.x = x / other;
        temp.y = y / other;
        return temp;
    }

    v2 operator *(float other) {
        v2 temp;
        temp.x = x * other;
        temp.y = y * other;
        return temp;
    }

    void operator +=(v2 other) {
        x += other.x;
        y += other.y;
    }

    void operator -=(v2 other) {
        x -= other.x;
        y -= other.y;
    }
};

struct button {
    Pixel color;
    Pixel border;
    string text;
    v2 start;
    v2 end;
    bool mouse;
    button(Pixel c, Pixel b, string t, v2 s, v2 e, bool m) {
        color = c;
        border = b;
        text = t;
        start = s;
        end = e;
        mouse = m;
    }
};

struct ball {
    v2 pos;
    int frames;
    ball(v2 p, int f) {
        pos = p;
        frames - f;
    }
};

int getindex(vector<ball> list, v2 value) {
    int x = 0;
    for (ball b : list) {
        if (b.pos.x == value.x and b.pos.y == value.y) { return x; }
        else { x++; }
    }
}

bool mouseincircle(v2 circlepos, v2 mouse, int diff) {
    
    v2 distancevector = mouse - circlepos;
    float distancediagonal = distancevector.distance();
    return (distancediagonal < diff) ? true : false;
}

bool mouseinbutton(v2 pos, v2 end, v2 mouse) {
    if (end.x >= mouse.x and mouse.x >= pos.x and end.y >= mouse.y and mouse.y >= pos.y) {
        return true;
    }
    else { return false; }
}

void drawbuttons(vector<button> butt, PixelGameEngine* engine) {
    for (button b : butt) {
        float vartam = (b.mouse == true) ? 1.1 : 1;
        int tamx = b.end.x - b.start.x;
        int tamy = b.end.y - b.start.y;
        float varpos = (b.mouse == true) ? tamx / 10 : 0;
        engine->FillRect(b.start.x - varpos, b.start.y - varpos, tamx * vartam + varpos, tamy * vartam + varpos, b.color);
        engine->DrawRect(b.start.x - 1 - varpos, b.start.y - 1 - varpos, (tamx + 2) * vartam + varpos, (tamy + 2) * vartam + varpos, b.border);
        if (b.text == "MEDIUM") {
            engine->DrawString(b.start.x + 35, b.start.y + 20, b.text, WHITE, 2 * vartam);
        }
        else if (b.text == "EXIT") {
            engine->DrawString(b.start.x + 5, b.start.y + 9, b.text, WHITE, 2 * vartam);
        }
        else {
            engine->DrawString(b.start.x + 45, b.start.y + 20, b.text, WHITE, 2 * vartam);
        }
    }
}

void drawballs(vector<ball> balls, PixelGameEngine* engine, int diff) {
    for (ball b : balls) {
        engine->FillCircle(b.pos.x, b.pos.y, diff - b.frames, RED);
    }
}

struct aim : PixelGameEngine {

    bool OnUserCreate(){
        return true;
    }
    bool menu = true;
    int diff; // balls radius - hard = 8, medium = 15, easy = 25
    vector<button> mybutt = {
        button(GREY, WHITE, "EASY", {120, 325}, {280, 375}, false),
        button(GREY, WHITE, "MEDIUM", {320, 325}, {480, 375}, false),
        button(GREY, WHITE, "HARD", { 520, 325 }, { 680, 375}, false),
        button(RED, WHITE, "EXIT", {50, 720}, {120, 750}, false)
    };
    vector<ball>myballs;
    int score = -1;
    int errors = 0;
    bool OnUserUpdate(float dt) {
        v2 mousepos = { GetMouseX(), GetMouseY() };
        Clear(BLACK);
        if (menu) {
            score = -1;
            errors = 0;
            myballs.clear();
            drawbuttons(mybutt, this);
            for (button& b : mybutt) {
                b.mouse = (mouseinbutton(b.start, b.end, mousepos)) ? true : false;
            }
            if (GetMouse(0).bPressed) {
                if (mouseinbutton({ 120, 320 }, { 280, 375 }, mousepos)) { menu = false; diff = 25; }
                else if (mouseinbutton({ 320, 325 }, { 480, 375 }, mousepos)) { menu = false; diff = 15; }
                else if (mouseinbutton({ 520, 325 }, { 680, 375 }, mousepos)) { menu = false; diff = 8; }
                else if (mouseinbutton({ 50, 720 }, { 120, 750 }, mousepos)) { exit(0); }
            }
        }
        
        else {
            bool boom = false;
            if (score == -1) {
                FillCircle(400, 400, diff, RED);
                DrawString(320, 350, "Click to start", WHITE, 2);
                if (GetMouse(0).bPressed and mouseincircle({ 400, 400 }, mousepos, diff)) { 
                    score += 1;
                    v2 randpos = { rand() % 770 + 30 , rand() % 770 + 30 };
                    myballs.push_back(ball(randpos, 0));
                    randpos = { rand() % 770 + 30 , rand() % 770 + 30 };
                    myballs.push_back(ball(randpos, 0));
                    randpos = { rand() % 770 + 30 , rand() % 770 + 30 };
                    myballs.push_back(ball(randpos, 0));
                }
            }
            else {
                drawballs(myballs, this, diff);
                DrawString(30, 30, "Score: " + to_string(score), WHITE, 2);
                DrawString(30, 100, "Errors: " + to_string(errors), WHITE, 2);
                if (GetMouse(0).bPressed) {
                    for (ball& b : myballs) {
                        if (mouseincircle(b.pos, mousepos, diff)) {
                            boom = true;
                            score ++;
                            int i = getindex(myballs, b.pos);
                            v2 randpos = { rand() % 770 + 30 , rand() % 770 + 30 };
                            myballs[i].pos = randpos;
                            myballs[i].frames = 0;
                        }
                    }
                    if (boom == false) { errors++; }
                }
            }
            for (ball& b : myballs) {
                b.frames += 0.001;
                if (diff - b.frames <= 0) {
                    errors++;
                    int i = getindex(myballs, b.pos);
                    v2 randpos = { rand() % 770 + 30 , rand() % 770 + 30 };
                    myballs[i].pos = randpos;
                    myballs[i].frames = 0;
                }
            }
            if (GetMouse(1).bPressed) { menu = true; }
        }
        return true;
    }

};

int main() {

    aim game;
    game.Construct(800, 800, 1, 1);
    game.Start();

}