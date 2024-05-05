#include "../include/game/sprites/LevelMinimapSprite.h"


LevelMinimapSprite::LevelMinimapSprite(GameLevel *level) {
  map = sf::VertexArray(sf::Lines);
  for (auto linedef : level->linedefs) {
    mapvertexes_t start = level->vertexes[linedef.startVertex];
    mapvertexes_t end = level->vertexes[linedef.endVertex];

    map.append(sf::Vertex(sf::Vector2f((float)start.x, (float)start.y),
                            sf::Color(70,70,70)));
    map.append(sf::Vertex(sf::Vector2f((float)end.x, (float)end.y),
                          sf::Color(70, 70, 70)));
  }
}

void LevelMinimapSprite::update(TransformComponent transform) {}

void LevelMinimapSprite::draw(sf::RenderTarget &target,
                              sf::RenderStates states) const {
  target.draw(map);
}
