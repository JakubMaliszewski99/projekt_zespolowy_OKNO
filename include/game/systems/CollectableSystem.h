#pragma once
#include "../components/CollectableComponent.h"
#include "../components/TransformComponent.h"
#include "../include/core/ecs/ECSManager.h"

class CollectableSystem : public System {
public:
  void init(std::shared_ptr<ECSManager> manager, Entity playerEntity) {
    m_manager = manager;
    m_playerEntity = playerEntity;
  }

  void update(float dt) {

    auto &playerTransform =
        m_manager->getComponent<TransformComponent>(m_playerEntity);

    std::vector<Entity> entitiesToDelete;
    for (auto const &entity : m_entities) {
      auto &collectableTransform =
          m_manager->getComponent<TransformComponent>(entity);
      auto &collectableSprite =
          m_manager->getComponent<SpriteComponent>(entity);
      auto &collectable = m_manager->getComponent<CollectableComponent>(entity);

      if (this->distance(playerTransform.positionX, playerTransform.positionY,
                         collectableTransform.positionX,
                         collectableTransform.positionY) < collectable.radius) {
        // TODO: Add effect after collecting
        entitiesToDelete.push_back(entity);
        collectableSprite.draw = false;
      }
    }

    if (entitiesToDelete.empty()) {
      return;
    }

    for (auto entity : entitiesToDelete) {
      m_manager->destroyEntity(entity);
    }
  }

  bool isCollectible(int type) { return false; }

  CollectableType getCollectableTypeFromSubType(int subType) {
    switch (subType) {
    case CollectableSubType::eWeaponBFG9000:
    case CollectableSubType::eWeaponChaingun:
    case CollectableSubType::eWeaponChainsaw:
    case CollectableSubType::eWeaponPlasmaGun:
    case CollectableSubType::eWeaponRocketLauncher:
    case CollectableSubType::eWeaponShotgun:
    case CollectableSubType::eWeaponSuperShotgun:
      return CollectableType::eWeapons;
      break;
    case CollectableSubType::eAmmo4ShotgunShells:
    case CollectableSubType::eAmmoBoxBullets:
    case CollectableSubType::eAmmoBoxRockets:
    case CollectableSubType::eAmmoBoxShotgunShells:
    case CollectableSubType::eAmmoClip:
    case CollectableSubType::eAmmoEnergyCell:
    case CollectableSubType::eAmmoEnergyCellPack:
    case CollectableSubType::eAmmoRocket:
      return CollectableType::eAmmo;
      break;
    case CollectableSubType::eArtifactArmorBonus:
    case CollectableSubType::eArtifactBersek:
    case CollectableSubType::eArtifactComputerAreaMap:
    case CollectableSubType::eArtifactHealthBonus:
    case CollectableSubType::eArtifactInvulnerability:
    case CollectableSubType::eArtifactLightAmplificationVisor:
    case CollectableSubType::eArtifactMegasphere:
    case CollectableSubType::eArtifactPartialInvisibility:
    case CollectableSubType::eArtifactSupercharge:
      return CollectableType::eArtifact;
      break;
    case CollectableSubType::ePowerupsArmor:
    case CollectableSubType::ePowerupsBackpack:
    case CollectableSubType::ePowerupsMedikit:
    case CollectableSubType::ePowerupsMegaarmor:
    case CollectableSubType::ePowerupsRadiationShieldingUnit:
    case CollectableSubType::ePowerupsStimpack:
      return CollectableType::ePowerups;
      break;
    case CollectableSubType::eKeysBlueKeycard:
    case CollectableSubType::eKeysBlueSkullKey:
    case CollectableSubType::eKeysRedKeycard:
    case CollectableSubType::eKeysRedSkullKey:
    case CollectableSubType::eKeysYellowKeycard:
    case CollectableSubType::eKeysYellowSkullKey:
      return CollectableType::eKeys;
      break;
    default:
      return CollectableType::eUnknown;
      break;
    }
  }

private:
  float distance(float x1, float y1, float x2, float y2) {
    float x = x1 - x2;
    float y = y1 - y2;
    float distance = sqrtf(x * x + y * y);

    return distance;
  }
  std::shared_ptr<ECSManager> m_manager;
  Entity m_playerEntity;
};
