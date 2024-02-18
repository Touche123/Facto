#include "../world.h"

void world_init(void) {
    render_sprite_sheet_init(&sprite_sheet_world, "assets/world.png", 24, 24);
}

void world_render(void) {
    render_sprite_sheet_frame(&sprite_sheet_world, 0, 0, (vec2) { 0, 0 }, false, (vec4) { 1, 1, 1, 0.2 }, sprite_sheet_world.texture_id);
}