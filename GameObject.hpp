#pragma once // ensure this is included only once
#include "Game.hpp"

struct telemetry{
        float x_pos; 
        float y_pos;
        float x_vel;
        float y_vel;
        float x_acc;
        float y_acc;
        double angle;
        double angle_vel;
    };

class GameObject {

public:
    GameObject(const char *texturesheet, int x, int y);  // int x and y represents initial location coords
    ~GameObject();

    void Update(uint64_t dt);
    void Render(uint64_t dt, uint64_t acc);

    struct telemetry getTelemetry();
    void setTelemetry(struct telemetry *tel);
    

private:

    SDL_Texture* objTexture;
    SDL_Rect srcRect, dstRect;    // src is position on texture sheet, dst is the destination data for renderer    
    telemetry tel;
    telemetry prev;
};