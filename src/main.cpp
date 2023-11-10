#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <GL/glu.h>
#include "textureManager.h"
#include "player.h"
#include "map.h"

using namespace sf;

float angleX, angleY; 
const float PI = 3.141592653;
std::vector<std::vector<std::vector<bool>>> mass(1000, std::vector<std::vector<bool>>(1000, std::vector<bool>(1000, false)));
float size = 20.f;
TextureManager textureManager;
Map map(100, 60, 100);


int main()
{

    sf::RenderWindow window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);

    window.setActive(true);

    Texture t;
    t.loadFromFile("resources/cursor.png");
    Sprite s(t); s.setOrigin(8, 8); s.setPosition(400, 300);
    std::vector<GLuint> skybox = textureManager.createSkybox();
    GLuint box = textureManager.createBox();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClearDepth(1.f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 2000.f);
    glEnable(GL_TEXTURE_2D);
    ShowCursor(FALSE);


    map.createMap(mass);

    Clock clock;

    bool running = true;

    bool mLeft = 0, mRight = 0;
    Player p(100, 200, 100, size);

    while (window.isOpen())
    {

        float time = clock.getElapsedTime().asMilliseconds();
        clock.restart();
        time = time / 50;
        if (time > 3) time = 3;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if ((event.type == Event::KeyPressed) && (event.key.code == Keyboard::Escape))
                window.close();

            if (event.type == Event::MouseButtonPressed)
            {
                if (event.key.code == Mouse::Right) mRight = 1;
                if (event.key.code == Mouse::Left)  mLeft = 1;
            }
        }


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        p.keyboard(angleX);
        p.update(time, mass, map);


        POINT mousexy;
        GetCursorPos(&mousexy);
        int xt = window.getPosition().x + 400;
        int yt = window.getPosition().y + 300;

        angleX += (xt - mousexy.x) / 4;
        angleY += (yt - mousexy.y) / 4;

        if (angleY < -89.0) { angleY = -89.0; }
        if (angleY > 89.0) { angleY = 89.0; }

        SetCursorPos(xt, yt);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(p.x, p.y + p.h / 2, p.z, p.x - sin(angleX / 180 * PI), p.y + p.h / 2 + (tan(angleY / 180 * PI)), p.z - cos(angleX / 180 * PI), 0, 1, 0);

        glTranslatef(p.x, p.y, p.z);
        textureManager.drawSkybox(skybox, 1000);
        glTranslatef(-p.x, -p.y, -p.z);

        map.drawMap(textureManager, size, box, mass);
        

        window.pushGLStates();
        window.draw(s);   
        window.popGLStates();

        window.display();
    }


    return 0;
}