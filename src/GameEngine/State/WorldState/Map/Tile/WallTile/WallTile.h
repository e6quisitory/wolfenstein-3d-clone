#pragma once

#include "../Tile.h"

/*
=========================================================
    WallTile (basic textured wall)
=========================================================
*/

class WallTile : public Tile {
private:
    texturePair_t texture;

public:
    WallTile(const iPoint2 &tileCoord, const texturePair_t _texture);

    virtual textureSliceDistPair_o  RayTileHit       (RayHitMarker& hitInfo, const texturePair_o textureOverride) const override;
    virtual bool                    PlayerTileHit()                                                               const override;
};