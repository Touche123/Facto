#include "../array_list.h"
#include "../util.h"
#include "../entity.h"

static Array_List *entity_list;

void entity_init(void) {
	entity_list = array_list_create(sizeof(Entity), 0);
}

usize entity_create(vec2 position, vec2 size, vec2 sprite_offset, vec2 velocity, u8 collision_layer, u8 collision_mask, bool is_kinematic, usize animation_id, On_Hit on_hit, On_Hit_Static on_hit_static) {
	usize id = entity_list->len;

	// Find inactive entity.
	for (usize i = 0; i < entity_list->len; ++i) {
		Entity *entity = array_list_get(entity_list, i);
		if (!entity->is_active) {
			id = i;
			entity->is_bleeding = false;
			break;
		}
	}

	if (id == entity_list->len) {
		if (array_list_append(entity_list, &(Entity){0}) == (usize)-1) {
			ERROR_EXIT("Could not append entity to list\n");
		}
	}

	Entity *entity = entity_get(id);

	*entity = (Entity){
		.is_active = true,
		.animation_id = animation_id, 
		.body_id = physics_body_create(position, size, velocity, collision_layer, collision_mask, is_kinematic, on_hit, on_hit_static, id),
		.sprite_offset = { sprite_offset[0], sprite_offset[1] },
		.is_bleeding = false,
	};

	return id;
}

Entity *entity_get(usize id) {
	return array_list_get(entity_list, id);
}
usize entity_count() {
	return entity_list->len;
}

void entity_reset(void) {
	entity_list->len = 0;
}

bool entity_damage(usize entity_id, u8 amount)
{
	Entity* entity = entity_get(entity_id);
	if (!entity->is_active)
	{
		entity->is_bleeding = false;
		return;
	}

	if (amount >= entity->health)
	{
		entity->is_bleeding = false;
		entity_destroy(entity_id);
		return true;
	}
	
	entity->health -= amount;
	return false;
}

void entity_destroy(usize entity_id)
{
	Entity* entity = entity_get(entity_id);
	entity->is_bleeding = false;
	physics_body_destroy(entity->body_id);
	entity->is_active = false;
}

void entity_update(f32 dt)
{
	for (usize i = 0; i < entity_list->len; ++i)
	{
		Entity* entity = array_list_get(entity_list, i);
		
		if (entity->is_active)
		{
			if (entity->is_bleeding)
			{
				damage_tick += dt;
				if (damage_tick < 5)
					return;
				
				entity_damage(entity->body_id, 100);
			}
		} 
	}

	damage_tick = 0;
}