#include "Physics.h"

Physics::Physics()
{
}

Physics::~Physics()
{
}

CollisionResult Physics::checkCollision(Entity& player1, Entity& player2, Entity& ball)
{

    if (collisionAABB(ball.m_body, player1.m_body))
    {
        return { CollisionType::Player1 };
    }
    if (collisionAABB(ball.m_body, player2.m_body))
    {
        return { CollisionType::Player2 };
    }

    if (collisionAABB(ball.m_body, m_walls[Wallid::Left]))
    {
        return { CollisionType::Wall_Left };
    }
    if (collisionAABB(ball.m_body, m_walls[Wallid::Right]))
    {
        return { CollisionType::Wall_Right };
    }


    if (collisionAABB(ball.m_body, m_walls[Wallid::Top]) || collisionAABB(ball.m_body, m_walls[Wallid::Bottom]))
    {
        return { CollisionType::Wall_TopBottom };
    }

    return { CollisionType::None };

}

bool Physics::collisionAABB(SDL_FRect& a, SDL_FRect& b)
{
    return
        a.x       < b.x + b.w &&
        a.x + a.w > b.x &&
        a.y       < b.y + b.h &&
        a.y + a.h > b.y;
}

