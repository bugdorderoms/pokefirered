#ifndef GUARD_POKEDEX_AREA_MARKERS_H
#define GUARD_POKEDEX_AREA_MARKERS_H

struct PAM_TaskData
{
    struct SubspriteTable subsprites;
    void * buffer;
    u16 tilesTag;
    u8 spr_id;
};

void SetAreaSubsprite(s32 i, s32 whichArea, struct Subsprite * subsprites);
void Dtor_PokedexAreaMarkers(u32 taskId);
u32 Ctor_PokedexAreaMarkers(u32 species, u32 tilesTag, u32 palIdx, u32 y);
u32 PokedexAreaMarkers_Any(u32 taskId);

#endif //GUARD_POKEDEX_AREA_MARKERS_H
