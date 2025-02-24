#include <SDL3/SDL.h>
#include <Box2D/Box2D.h>
#include <iostream>
#include <Box2D/Particle/b2ParticleSystem.h>
#include <Box2D/Particle/b2Particle.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[]) {
    // SDL başlat
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL baslatilamadi: " << SDL_GetError() << std::endl;
        return -1;
    }

    // SDL Pencere oluştur
    SDL_Window* window = SDL_CreateWindow("LiquidFun Particles", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "Pencere olusturulamadı: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // SDL Renderer oluştur
    SDL_Renderer* renderer = SDL_CreateRenderer(window, 0);
    if (!renderer) {
        std::cerr << "Renderer olusturulamadi: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Box2D dünyası oluştur
    b2Vec2 gravity(0.0f, 10.0f);
    b2World world(gravity);

    // 🔹 **Zemin (Ground Body)**
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(4.0f, 5.5f);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(4.0f, 0.1f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    // 🔹 **Sol Duvar**
    b2BodyDef leftWallDef;
    leftWallDef.position.Set(0.1f, 3.0f);
    b2Body* leftWall = world.CreateBody(&leftWallDef);
    b2PolygonShape leftWallBox;
    leftWallBox.SetAsBox(0.1f, 3.0f);
    leftWall->CreateFixture(&leftWallBox, 0.0f);

    // 🔹 **Sağ Duvar**
    b2BodyDef rightWallDef;
    rightWallDef.position.Set(7.9f, 3.0f);
    b2Body* rightWall = world.CreateBody(&rightWallDef);
    b2PolygonShape rightWallBox;
    rightWallBox.SetAsBox(0.1f, 3.0f);
    rightWall->CreateFixture(&rightWallBox, 0.0f);

    // 🔹 **LiquidFun parçacık sistemi tanımla**
    b2ParticleSystemDef particleSystemDef;
    particleSystemDef.radius = 0.02f;
    b2ParticleSystem* particleSystem = world.CreateParticleSystem(&particleSystemDef);

    // 🔹 **Parçacık grubu oluştur**
    b2ParticleGroupDef particleGroupDef;
    particleGroupDef.flags = b2_waterParticle;
    particleGroupDef.position.Set(4.0f, 1.0f);

    b2PolygonShape waterShape;
    waterShape.SetAsBox(1.5f, 1.5f);

    particleGroupDef.shape = &waterShape;

    b2ParticleGroup* group = particleSystem->CreateParticleGroup(particleGroupDef);
    if (group == nullptr) {
        std::cerr << "Parcacik grubu olusturulamadi!" << std::endl;
    }
    else {
        std::cout << "Parcacik grubu basariyla olusturuldu!" << std::endl;
    }

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        world.Step(1.0f / 60.0f, 8, 3);

        // 🔹 **Arka planı beyaz yap**
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        int32 particleCount = particleSystem->GetParticleCount();
        const b2Vec2* positions = particleSystem->GetPositionBuffer();
        SDL_SetRenderDrawColor(renderer, 0, 191, 255, 255);  // Açık mavi (su rengi)

        // 🔹 **Parçacıkları çiz**
        for (int i = 0; i < particleCount; i++) {
            SDL_FPoint point = { positions[i].x * 100, positions[i].y * 100 };
            SDL_FRect rect = { point.x - 1, point.y - 1, 2, 2 };
            SDL_RenderFillRect(renderer, &rect);
        }

        // 🔹 **Zemin ve duvarları sarı renkte çiz**
        SDL_SetRenderDrawColor(renderer, 255, 204, 0, 255);  // Sarı renk (altın tonu)

        // **Zemin çiz**
        SDL_FRect groundRect = { 0, 550, 800, 10 };
        SDL_RenderFillRect(renderer, &groundRect);

        // **Sol Duvar**
        SDL_FRect leftWallRect = { 0, 0, 10, 600 };
        SDL_RenderFillRect(renderer, &leftWallRect);

        // **Sağ Duvar**
        SDL_FRect rightWallRect = { 790, 0, 10, 600 };
        SDL_RenderFillRect(renderer, &rightWallRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
