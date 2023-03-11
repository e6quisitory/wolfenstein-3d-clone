#include "EnemyManager.h"
#include "../../WorldState/Map/Tile/EnemyContainerTile/EnemyContainerTile.h"


/*
=========================================================
    Public methods
=========================================================
*/

void EnemyManager::Init(WorldState* const _worldState, Multimedia* const _multimedia) {
    worldState = _worldState;
    multimedia = _multimedia;

    // Load enemy textures into EnemyTile
    LoadEnemyTypeTextures(textureType_t::GUARD);
    LoadEnemyTypeTextures(textureType_t::OFFICER);
    LoadEnemyTypeTextures(textureType_t::SS);

    // Pass map pointer to enemy tile
    Enemy::map = &(worldState->map);

    // Set enemies movement speed
    Enemy::moveIncrement = 0.02/(multimedia->displayParams.refreshRate/60.0)/10;

    // Gather all enemies in map
    for (const auto& tileColumn : worldState->map.tiles) {
        for (const auto tile : tileColumn) {
            if (tile->enemyContainer) {
                auto enemyContainerTile = static_cast<EnemyContainerTile*>(tile);
                for (const auto& enemy : enemyContainerTile->enemies)
                    enemies.push_back(enemy);
            }
        }
    }

}

void EnemyManager::Update() const {
    for (const auto enemy : enemies) {
        enemy->Walk();
    }
}

/*
=========================================================
    Private methods
=========================================================
*/

void EnemyManager::LoadEnemyTypeTextures(const textureType_t enemyType) {
    typedef std::pair<enemyTextureType_t, std::vector<int>> textureTypeIDList_t;

    static const textureTypeIDList_t STATIC       = {enemyTextureType_t::STATIC, {1, 2, 3, 4, 5, 6, 7, 8}};
    static const textureTypeIDList_t RUN1         = {enemyTextureType_t::RUN1, {9, 10, 11, 12, 13, 14, 15, 16}};
    static const textureTypeIDList_t RUN2         = {enemyTextureType_t::RUN2, {17, 18, 19, 20, 21, 22, 23, 24}};
    static const textureTypeIDList_t RUN3         = {enemyTextureType_t::RUN3, {25, 26, 27, 28, 29, 30, 31, 32}};
    static const textureTypeIDList_t RUN4         = {enemyTextureType_t::RUN4, {33, 34, 35, 36, 37, 38, 39, 40}};
    static const textureTypeIDList_t ALERTED      = {enemyTextureType_t::ALERTED, {49}};
    static const textureTypeIDList_t SHOOTING     = {enemyTextureType_t::SHOOTING, {50, 51}};
    static const textureTypeIDList_t DAMAGE_RIGHT = {enemyTextureType_t::DAMAGE_RIGHT, {41}};
    static const textureTypeIDList_t DAMAGE_LEFT  = {enemyTextureType_t::DAMAGE_LEFT, {48}};
    static const textureTypeIDList_t DEATH        = {enemyTextureType_t::DEATH, {42, 43, 44, 45}};

    static const std::vector<textureTypeIDList_t> allTextureTypeIDLists = {STATIC, RUN1, RUN2, RUN3, RUN4, ALERTED, SHOOTING, DAMAGE_RIGHT, DAMAGE_LEFT, DEATH};

    for (const auto& textureTypeIDList : allTextureTypeIDLists) {
        const auto& [textureType, IDsList] = textureTypeIDList;
        for (const auto ID : IDsList)
            EnemyContainerTile::textures[enemyType][textureType].push_back(multimedia->GetTexture(enemyType, ID));
    }
}
