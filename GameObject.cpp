#include "GameObject.hpp"
#include "TextureManager.hpp"

GameObject::GameObject(const char* texturesheet, int x, int y)
{
    objTexture = TextureManager::LoadTexture(texturesheet);

    SDL_QueryTexture(objTexture, NULL, NULL, &dstRect.w, &dstRect.h);

    tel.x_pos = x - (dstRect.w/2);
    tel.y_pos = y - (dstRect.h/2);
    tel.x_vel = 300;
    tel.y_vel = 300;
    tel.x_acc = 0;
    tel.y_acc = 0;
    tel.angle = 60;
    tel.angle_vel = 0;

}

void GameObject::Update(uint64_t dt)
{   
    prev = tel;
    // global collision detector soon
    if (tel.x_pos <= 0) {
            tel.x_pos = 0;
            tel.x_vel = -tel.x_vel;
        }
    if (tel.y_pos <= 0) {
            tel.y_pos = 0;
            tel.y_vel = -tel.y_vel;
        }
    if (tel.x_pos >= 800 - dstRect.w){
            tel.x_pos = 800 - dstRect.w;
            tel.x_vel = -tel.x_vel;
        }
    if (tel.y_pos >= 600 - dstRect.h){
            tel.y_pos = 600 - dstRect.h;
            tel.y_vel = -tel.y_vel;
        }

    // calculate new y velocity
    tel.y_vel += tel.y_acc * (dt/1000.0f);
    
    // calculate position
    tel.x_pos += (tel.x_vel) * (dt/1000.0f);
    tel.y_pos += (tel.y_vel) * (dt/1000.0f);

    // calculate angle
    tel.angle += (tel.angle_vel) * (dt/1000.0f);

    // update sprite position
    dstRect.x = (int) tel.x_pos;
    dstRect.y = (int) tel.y_pos;
    
    srcRect.w = 180;  // specifying height and coordinates of sprite in texture sheet
    srcRect.h = 168;
    srcRect.x = 0;
    srcRect.y = 0;

}

void GameObject::Render(uint64_t dt, uint64_t acc)
{   
    float alpha = (float) acc / dt; // aha this doesn't make you want to cry, im sure
    if (alpha == 0.0f) {alpha = 0.1f;}
    if (alpha == 1.0f) {alpha = 0.9f;}
    
    // interpolating 
    prev.y_pos = tel.y_pos * alpha + prev.y_pos * (1 - alpha);
    prev.x_pos = tel.x_pos * alpha + prev.x_pos * (1 - alpha);

    // update sprite position
    dstRect.x = (int) prev.x_pos;
    dstRect.y = (int) prev.y_pos;

    SDL_RenderCopyEx(Game::renderer, objTexture, &srcRect, &dstRect, tel.angle, NULL, SDL_FLIP_NONE);
}

struct telemetry GameObject::getTelemetry()
{
    return tel;
}

void GameObject::setTelemetry(struct telemetry *newTel)
{
    tel = *newTel;
}

