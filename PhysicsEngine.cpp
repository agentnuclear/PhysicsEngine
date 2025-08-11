// PhysicsEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include "Particle.h"

int main()
{
    const unsigned int WIN_W = 800;
    const unsigned int WIN_H = 600;

    sf::RenderWindow window(sf::VideoMode({ WIN_W,WIN_H }), "Verlet Physics Engine");
    //window.setFramerateLimit(60);

    std::mt19937 rng{ std::random_device{}() };
    std::uniform_real_distribution<float> ux(50.f, static_cast<float>(WIN_W) - 50.f);
    std::uniform_real_distribution<float> uy(50.f, static_cast<float>(WIN_H) - 50.f);


    const int NUM = 50;
    const float RADIUS = 6.f;
    std::vector<Particle> particles;
    particles.reserve(NUM);

    for (int i = 0; i < NUM; ++i) {
        sf::Vector2f p{ ux(rng), uy(rng) };
        particles.emplace_back(p, RADIUS);
    }

   
    //Particle p1(sf::Vector2f(400.f, 100.f), 10.f);
    sf::Clock clock;
    std::size_t frameCounter = 0; //frames
    float timeAccumulator = 0.f; //to update title per second
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event ->is<sf::Event::Closed>())
                window.close();
        }

        //update
        float dt = clock.restart().asSeconds();
        for (auto& pt : particles)
            pt.update(dt);

        //render
        window.clear(sf::Color::Black);
        for (auto& pt : particles)
            pt.draw(window);
        window.display();

        //FPS Counter
        frameCounter++;
        timeAccumulator += dt;
        if (timeAccumulator >= 1.0f) {
            float fps = frameCounter / timeAccumulator;
            window.setTitle("Simple Verlet Physics | Engine FPS ->" + std::to_string(static_cast<int>(fps)));
            frameCounter = 0;
            timeAccumulator = 0.f;
        }
    }

    return 0;
}