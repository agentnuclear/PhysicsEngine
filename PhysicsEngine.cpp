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
    std::uniform_real_distribution<float> velDir(-1.f, 1.f); // random direction


    const int NUM = 100;
    const float RADIUS = 6.f;
    std::vector<Particle> particles;
    particles.reserve(NUM);

    sf::Vector2f spawnPoint(WIN_W / 2.f, WIN_H / 2.f);

    for (int i = 0; i < NUM; ++i) {
        /*sf::Vector2f p{ ux(rng), uy(rng) };
        particles.emplace_back(p, RADIUS);*/
        Particle p(spawnPoint, RADIUS);
        // Give random initial velocity (by modifying oldPosition)

        float vx = velDir(rng) * 100.f; // speed variation
        float vy = velDir(rng) * 100.f;
        p.prevPos = spawnPoint - sf::Vector2f(vx * 0.016f, vy * 0.016f); // back in time

        particles.push_back(p);
    }

    //Lambda for collision resolution
    auto resolveCollision = [&](std::vector<Particle>& particles) {
        for (size_t i = 0; i < particles.size(); ++i) {
            for (size_t j = i + 1; j < particles.size(); ++j) {
                sf::Vector2f delta = particles[j].pos - particles[i].pos;
                float dist2 = delta.x * delta.x + delta.y * delta.y;
                float minDist = particles[i].radius + particles[j].radius;

                if (dist2 < minDist * minDist) {
                    float dist = std::sqrt(dist2);
                    if (dist == 0.0f) {
                        delta = { 0.01f, 0.01f };
                        dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);
                    }

                    sf::Vector2f norm = delta / dist;
                    float overlap = (minDist - dist) * 0.5f;

                    particles[i].pos -= norm * overlap;
                    particles[j].pos += norm * overlap;
                }
            }
        }
    };

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

        resolveCollision(particles);

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