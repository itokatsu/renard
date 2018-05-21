#ifndef DEF_ENEMY
#define DEF_ENEMY

#include "SDL.h"
//#include "IEntity.h"
#include "Vec2.h"
#include "GameEngine.h"
#include "Sprite.h"

class Enemy
{
  public:
	Enemy(GameEngine *, int posX, int posY);

	void Draw(GameEngine *game);
	void Update(GameEngine *game, float dt);

	SDL_Point GetPosition();
	Vec2f GetVelocity();
	void SetVelocity(Vec2f v);
	void SetVelocity(float velX, float velY);
	void AddVelocity(Vec2f v);
	SDL_Rect GetRect();
	SDL_Color GetColor();

  private:
	int width_;
	int height_;
	SDL_Point position_;
	Vec2f velocity_;
	float maxSpeed_;
	int health_;
	SDL_Color color_;
};

#endif