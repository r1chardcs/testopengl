//
// Created by dlllibstdntc on 10.09.2026.
//

#ifndef GAMEENGINE_WORLD_H
#define GAMEENGINE_WORLD_H

#include <vector>
#include <memory>

#include "RenderSystem.h"

struct AABB {
    FLOAT minX, minY, maxX, maxY;

    BOOL OnCollision(const AABB& aabb) const {
        return minX <= aabb.maxX &&
               maxX >= aabb.minX &&
               minY <= aabb.maxY &&
               maxY >= aabb.minY;
    }
};

struct Block {
    FLOAT x, y, size;
    AABB aabb;
};

class Entity {
    INT health = 0;
    FLOAT x = 0, y = 0;

public:
    BOOL IsAlive() const;
};

class World {
    std::vector<std::shared_ptr<Entity>> entities;
    std::vector<Block> blocks;
public:
    void RenderWorld(RenderSystem* render_system);
    void RenderBlocks(RenderSystem* render_system);
    void RenderAllEntities(RenderSystem* render_system);
    void RenderCurEntity(RenderSystem* render_system, Entity* entity);
};


#endif //GAMEENGINE_WORLD_H
