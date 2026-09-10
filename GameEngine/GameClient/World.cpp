//
// Created by dlllibstdntc on 10.09.2026.
//

#include "World.h"

#include "Doomsday.h"

BOOL Entity::IsAlive() const {
    return health <= 0;
}

struct Player {
    float x, y;
    AABB aabb{

    };
};

Player player;
void World::RenderWorld(RenderSystem *render_system) {
    glPushMatrix();
    auto deltaTick = render_system->GetDeltaTime();
    constexpr float speed = 13;
    auto w = Doomsday::GetInstance().GetWindow();

    if (glfwGetKey(w->GetHandle(), GLFW_KEY_W)) {
        player.y += speed * deltaTick;
    }
    if (glfwGetKey(w->GetHandle(), GLFW_KEY_S)) {
        player.y -= speed * deltaTick;
    }
    if (glfwGetKey(w->GetHandle(), GLFW_KEY_D)) {
        player.x += speed * deltaTick;
    }
    if (glfwGetKey(w->GetHandle(), GLFW_KEY_A)) {
        player.x -= speed * deltaTick;
    }

    double mx, my;
    glfwGetCursorPos(w->GetHandle(), &mx, &my);

    player.aabb.minX = player.x - 1;
    player.aabb.maxX = player.x + 1;
    player.aabb.minY = player.y - 1;
    player.aabb.maxY = player.y + 1;

    for (auto block : blocks) {
        if (block.aabb.OnCollision(player.aabb)) {
        }
    }

    render_system->Buffer(GL_QUADS).Color3(1, 0, 0).Rect2x2(player.x, player.y, 1, 1).End();
    render_system->Buffer(GL_LINE_LOOP).Color3(1, 0, 0).Circle2x2(
        render_system->ToWorldX(mx),
        render_system->ToWorldY(my), 1).End();

    RenderAllEntities(render_system);
    RenderBlocks(render_system);
    glPopMatrix();
}

void World::RenderBlocks(RenderSystem* render_system) {
    constexpr int mapSize = 45;
    constexpr FLOAT blockSize = 2.0f;
    constexpr FLOAT halfBlock = blockSize / 2.0f;

    constexpr FLOAT mapSizeWorld = mapSize * blockSize;
    constexpr FLOAT mapHalf = mapSizeWorld / 2.0f;

    for (int x = 0; x < mapSize; x++) {
        for (int y = 0; y < mapSize; y++) {
            if (x != 0 && x != mapSize - 1 &&
                y != 0 && y != mapSize - 1)
                continue;

            const FLOAT blockX =
                -mapHalf + x * blockSize + halfBlock;

            const FLOAT blockY =
                -mapHalf + y * blockSize + halfBlock;

            blocks.push_back({blockX, blockY, halfBlock,
            {.minX = blockX - halfBlock,
             .minY = blockY - halfBlock,
             .maxX = blockX + halfBlock,
             .maxY = blockY + halfBlock}});

            render_system->Buffer(GL_QUADS)
                .Color3(0.6f, 0.6f, 0.6f)
                .Rect2x2(
                    blockX,
                    blockY,
                    halfBlock,
                    halfBlock
                )
                .End();

            glLineWidth(1.0f);

            render_system->Buffer(GL_LINE_LOOP)
                .Color3(0.1f, 0.1f, 0.1f)
                .Rect2x2(
                    blockX,
                    blockY,
                    halfBlock,
                    halfBlock
                )
                .End();
        }
    }
}

void World::RenderAllEntities(RenderSystem *render_system) {
    for (auto entity : entities)
        if (entity && entity->IsAlive()) RenderCurEntity(render_system, entity.get());
}

void World::RenderCurEntity(RenderSystem *render_system, Entity *entity) {
}

