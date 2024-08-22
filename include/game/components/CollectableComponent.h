#pragma once

enum CollectableType {
  eWeapons, eAmmo, eArtifact, ePowerups, eKeys, eUnknown
};

enum CollectableSubType {
  eWeaponBFG9000 = 2006, eWeaponChaingun = 2002, eWeaponChainsaw = 2005, eWeaponPlasmaGun = 2004,
  eWeaponRocketLauncher = 2003, eWeaponShotgun = 2001, eWeaponSuperShotgun = 82,
  
  eAmmo4ShotgunShells = 2008, eAmmoBoxBullets = 2048, eAmmoBoxRockets = 2046, eAmmoBoxShotgunShells = 2049,
  eAmmoClip = 2007, eAmmoEnergyCell = 2047, eAmmoEnergyCellPack = 17, eAmmoRocket = 2010,
  
  eArtifactArmorBonus = 2015, eArtifactBersek = 2023, eArtifactComputerAreaMap = 2026, eArtifactHealthBonus = 2014,
  eArtifactInvulnerability = 2022, eArtifactLightAmplificationVisor = 2045, eArtifactMegasphere = 83, eArtifactPartialInvisibility = 2024,
  eArtifactSupercharge = 2013,

  ePowerupsArmor = 2018, ePowerupsBackpack = 8, ePowerupsMedikit = 2012, ePowerupsMegaarmor = 2019,
  ePowerupsRadiationShieldingUnit = 2025, ePowerupsStimpack = 2011,

  eKeysBlueKeycard = 5, eKeysBlueSkullKey = 40, eKeysRedKeycard = 13, eKeysRedSkullKey = 38,
  eKeysYellowKeycard = 6, eKeysYellowSkullKey = 39
};

struct CollectableComponent {
  CollectableType type;
  CollectableSubType subType;
  int value;
  int radius;
};