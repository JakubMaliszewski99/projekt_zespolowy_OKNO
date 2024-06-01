#include "../include/core/engine/GameEngine.h"
#include "../include/core/loaders/WADLoader.h"
#include "../include/game/components/GameDrawableComponent.h"
#include "../include/game/components/HealthComponent.h"
#include "../include/game/sprites/CollectableMinimapSprite.h"
#include "../include/game/sprites/LevelMinimapSprite.h"

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

GameEngine::GameEngine(InitSettings settings) {
  m_settings = settings;
  m_state = settings.initialState;

  if (settings.isDebug) {
    Logger::getInstance()->setLoggingLevel(eDebug);
  } else {
    Logger::getInstance()->setLoggingLevel(eWarn);
  }

  WADLoader loader;
  m_level = loader.loadFromFile("../data/assets/DOOM1.WAD", "E1M1");

  sf::Vector2f initialPlayerPosition;
  float initialPlayerAngle = 0.0f;
  for (auto thing : m_level->things) {
    if (thing.type == 1) {
      initialPlayerPosition = sf::Vector2f((float)thing.x, (float)thing.y);
      initialPlayerAngle = thing.angle * (M_PI / 180);
      break;
    }
  }

  m_window = std::make_shared<sf::RenderWindow>();

  // TODO: Move ECS setup to another methods
  // ECS
  m_ecsManager = std::make_shared<ECSManager>();
  m_ecsManager->init();
  // Components
  m_ecsManager->registerComponent<HealthComponent>();
  m_ecsManager->registerComponent<TransformComponent>();
  m_ecsManager->registerComponent<MinimapSpriteComponent>();
  m_ecsManager->registerComponent<ControllableComponent>();
  m_ecsManager->registerComponent<CollectableComponent>();
  m_ecsManager->registerComponent<GameDrawableComponent>();
  m_ecsManager->registerComponent<WeaponComponent>();
  m_ecsManager->registerComponent<DamageComponent>();

  // PlayerControllSystem Signature
  Signature playerSystemSignature;
  playerSystemSignature.set(m_ecsManager->getComponentType<HealthComponent>());
  playerSystemSignature.set(
      m_ecsManager->getComponentType<TransformComponent>());
  playerSystemSignature.set(
      m_ecsManager->getComponentType<ControllableComponent>());
  // PlayerControllSystem Register
  m_playerControllSystem = m_ecsManager->registerSystem<PlayerControllSystem>();
  m_playerControllSystem->init(m_ecsManager);
  m_ecsManager->setSystemSignature<PlayerControllSystem>(playerSystemSignature);

  // PlayerMovementSystem Signature
  Signature playerMovementSystemSignature;
  playerMovementSystemSignature.set(
      m_ecsManager->getComponentType<HealthComponent>());
  playerMovementSystemSignature.set(
      m_ecsManager->getComponentType<TransformComponent>());
  playerMovementSystemSignature.set(
      m_ecsManager->getComponentType<ControllableComponent>());
  // PlayerMovementSystem Register
  m_playerMovementSystem = m_ecsManager->registerSystem<PlayerMovementSystem>();
  m_playerMovementSystem->init(m_ecsManager, std::make_shared<BSP>(m_level));
  m_ecsManager->setSystemSignature<PlayerMovementSystem>(
      playerMovementSystemSignature);

  // Create player entity
  m_playerEntity = m_ecsManager->createEntity();

  // MinimapRenderingsystem Signature
  Signature minimapRenderingSystemSignature;
  minimapRenderingSystemSignature.set(
      m_ecsManager->getComponentType<TransformComponent>());
  minimapRenderingSystemSignature.set(
      m_ecsManager->getComponentType<MinimapSpriteComponent>());
  // MinimapRenderingSystem Register
  m_minimapRenderingSystem =
      m_ecsManager->registerSystem<MinimapRenderingSystem>();
  m_minimapRenderingSystem->init(m_ecsManager, m_window,
                                 std::make_shared<BSP>(m_level), m_playerEntity,
                                 m_settings.debugSettings);
  m_ecsManager->setSystemSignature<MinimapRenderingSystem>(
      minimapRenderingSystemSignature);

  // GameRenderingSystem Signature
  Signature gameRenderingSystemSignature;
  gameRenderingSystemSignature.set(
      m_ecsManager->getComponentType<TransformComponent>());
  gameRenderingSystemSignature.set(
      m_ecsManager->getComponentType<GameDrawableComponent>());
  // GameRenderingSystem Register
  m_gameRenderingSystem = m_ecsManager->registerSystem<GameRenderingSystem>();
  m_gameRenderingSystem->init(m_ecsManager, m_window,
                              std::make_shared<BSP>(m_level), m_playerEntity);
  m_ecsManager->setSystemSignature<GameRenderingSystem>(
      gameRenderingSystemSignature);

  // CollectableSystem Signature
  Signature collectableSystemSignature;
  collectableSystemSignature.set(
      m_ecsManager->getComponentType<TransformComponent>());
  collectableSystemSignature.set(
      m_ecsManager->getComponentType<CollectableComponent>());
  // CollectableSystem Register
  m_collectableSystem = m_ecsManager->registerSystem<CollectableSystem>();
  m_collectableSystem->init(m_ecsManager, m_playerEntity);
  m_ecsManager->setSystemSignature<CollectableSystem>(
      collectableSystemSignature);

  // WeaponSystem Signature
  Signature weaponSystemSignature;
  weaponSystemSignature.set(
      m_ecsManager->getComponentType<WeaponComponent>());
  //WeaponSystem Register
  m_weaponSystem = m_ecsManager->registerSystem<WeaponSystem>();
  m_weaponSystem->init(m_ecsManager);
  m_ecsManager->setSystemSignature<WeaponSystem>(
      weaponSystemSignature);

  //DamageSystem Signature
  Signature damageSystemSignature;
  damageSystemSignature.set(
      m_ecsManager->getComponentType<HealthComponent>());
    damageSystemSignature.set(
      m_ecsManager->getComponentType<TransformComponent>());
  //DamageSystem Register
  m_damageSystem = m_ecsManager->registerSystem<DamageSystem>();
  m_damageSystem->init(m_ecsManager, m_playerEntity, std::make_shared<BSP>(m_level));
  m_ecsManager->setSystemSignature<DamageSystem>(
      damageSystemSignature);

  //EnviromentDamageSystem Signature
  Signature enviromentDamageSystemSignature;
  enviromentDamageSystemSignature.set(
      m_ecsManager->getComponentType<DamageComponent>());
    damageSystemSignature.set(
      m_ecsManager->getComponentType<TransformComponent>());
  //EnviromentDamageSystem Register
  m_enviromentDamageSystem = m_ecsManager->registerSystem<EnviromentDamageSystem>();
  m_enviromentDamageSystem->init(m_ecsManager);
  m_ecsManager->setSystemSignature<EnviromentDamageSystem>(
      enviromentDamageSystemSignature);


  m_ecsManager->addComponent(m_playerEntity, HealthComponent{100, 100});
  m_ecsManager->addComponent(
      m_playerEntity,
      TransformComponent{initialPlayerPosition.x, initialPlayerPosition.y,
                         PLAYER_HEIGHT, PLAYER_HEIGHT,
                         sf::Vector2f(), initialPlayerAngle});
  m_ecsManager->addComponent(
      m_playerEntity,
      MinimapSpriteComponent{
          sf::View(),
          new PlayerMinimapSprite(m_settings.debugSettings.displayFov), true});
  m_ecsManager->addComponent(m_playerEntity, ControllableComponent{true});
  m_ecsManager->addComponent(
      m_playerEntity, 
      WeaponComponent{
          {WeaponModel::eWeaponModelFist, WeaponModel::eWeaponModelPistol, WeaponModel::eWeaponModelNone}, 
          WeaponType::eHandWeapon,
          false, true, 0, 1, 0});

  // Create map entity
  m_mapEntity = m_ecsManager->createEntity();
  m_ecsManager->addComponent(m_mapEntity, TransformComponent{
                                              0.0f,
                                              0.0f,
                                              0.0f,0.0f,
                                              sf::Vector2f(),
                                          });
  m_ecsManager->addComponent(
      m_mapEntity, MinimapSpriteComponent{
                       sf::View(), new LevelMinimapSprite(m_level), false});
  m_ecsManager->addComponent(
      m_mapEntity, GameDrawableComponent{GameDrawableType::eLevel, nullptr});

  // Spawn things
  for (auto thing : m_level->things) {
    if (thing.type == 1) {
      continue;
    }

    std::vector<int> skillLevels;
    if ((thing.flags & 1) > 0) {
      skillLevels.push_back(1);
      skillLevels.push_back(2);
    }
    if ((thing.flags & 2) > 0) {
      skillLevels.push_back(3);
    }
    if ((thing.flags & 4) > 0) {
      skillLevels.push_back(4);
      skillLevels.push_back(5);
    }

    if (std::find(skillLevels.begin(), skillLevels.end(),
                  m_settings.difficultyLevel) == skillLevels.end()) {
      continue;
    }

    sf::Color color;

    switch (getCollectableTypeFromSubType(thing.type)) {
    case CollectableType::eWeapons:
      color = sf::Color::Magenta;
      break;
    case CollectableType::eAmmo:
      color = sf::Color::Yellow;
      break;
    case CollectableType::eArtifact:
      color = sf::Color::Green;
      break;
    case CollectableType::ePowerups:
      color = sf::Color::Red;
      break;
    case CollectableType::eKeys:
      color = sf::Color::Blue;
      break;
    default:
      continue;
      break;
    }

    auto thingEntity = m_ecsManager->createEntity();
    m_ecsManager->addComponent(
        thingEntity,
        TransformComponent{(float)thing.x, (float)thing.y, 0.0f, 0.0f, sf::Vector2f(), 0});
    m_ecsManager->addComponent(
        thingEntity,
        MinimapSpriteComponent{sf::View(), new CollectableMinimapSprite(color),
                               false});
    m_ecsManager->addComponent(
        thingEntity,
        CollectableComponent{getCollectableTypeFromSubType(thing.type),
                             (CollectableSubType)thing.type, 10, 20});
      m_ecsManager->addComponent(thingEntity, HealthComponent{100, 100});
  }
}

void GameEngine::processEvents() {
  sf::Event event;
  while (m_window->pollEvent(event)) {
    switch (event.type) {
    case sf::Event::Closed:
      m_window->close();
      break;
    }
  }
}

void GameEngine::run() {
  init();
  sf::Clock clock;

  while (m_window->isOpen()) {
    sf::Time deltaTime = clock.restart();
    processEvents();
    update(deltaTime);
  }
}

void GameEngine::init() {
  m_window->create(
      sf::VideoMode(m_settings.windowWidth, m_settings.windowHeight),
      m_settings.windowTitle);
}

void GameEngine::update(sf::Time deltaTime) {
  float dt = deltaTime.asSeconds();
  if (InputManager::getInstance()->isKeyPressed(sf::Keyboard::Tab)) {
    m_state = GameEngineState::eGameMinimap;
  } else {
    m_state = GameEngineState::eGame;
  }

  m_playerControllSystem->update(dt);
  m_playerMovementSystem->update(dt);
  //m_minimapRenderingSystem->update(dt);
  if (m_state == GameEngineState::eGameMinimap) {
    m_minimapRenderingSystem->update(dt);
  } else if (m_state == GameEngineState::eGame) {
    m_gameRenderingSystem->update(dt);
  }
  m_collectableSystem->update(dt);
  m_weaponSystem->update(dt);
  m_damageSystem->update(dt);
  m_enviromentDamageSystem->update(dt);
}
