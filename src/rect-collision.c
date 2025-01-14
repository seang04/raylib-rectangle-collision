///rect-collision.c
///defines functions declared in rect-collision.h, used for swept aabb
///collision detection between rectangles.
///@author Sean Gaines

#include "rect-collision.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../raylib/include/raymath.h"

//check collision of a ray against a target rectangle
collision_data collision_ray_rect(Vector2 ray_origin, Vector2 ray_dir, Rectangle target)
{
    collision_data out;
    out.collision = false;
    out.contact_normal = (Vector2){0, 0};
    out.contact_point = (Vector2){0, 0};

    float t_near_x = (target.x - ray_origin.x) / ray_dir.x;
    float t_far_x = (target.x + target.width - ray_origin.x) / ray_dir.x;
    float t_near_y = (target.y - ray_origin.y) / ray_dir.y;
    float t_far_y = (target.y + target.height - ray_origin.y) / ray_dir.y;

    if (isnan(t_far_y) || isnan(t_far_x)) return out;
    if (isnan(t_near_y) || isnan(t_near_x)) return out;


    if (t_near_x > t_far_x)
    {
        float tmp = t_near_x;
        t_near_x = t_far_x;
        t_far_x = tmp;
    }
    if (t_near_y > t_far_y)
    {
        float tmp = t_near_y;
        t_near_y = t_far_y;
        t_far_y = tmp;
    }

    if (t_near_x > t_far_y || t_near_y > t_far_x) return out;

    out.t_hit_near = fmaxf(t_near_x, t_near_y);
    if(out.t_hit_near > 1) return out;

    float t_hit_far = fminf(t_far_x, t_far_y);
    if(t_hit_far < 0) return out;

    out.contact_point = Vector2Add(ray_origin, Vector2Scale(ray_dir, out.t_hit_near));

    if (t_near_x > t_near_y)
    {
        if (ray_dir.x < 0)
        {
            out.contact_normal = (Vector2){1, 0};
        }
        else
        {
            out.contact_normal = (Vector2){-1, 0};
        }
    }
    else if(t_near_x < t_near_y)
    {
        if(ray_dir.y < 0)
        {
            out.contact_normal = (Vector2){0, 1};
        }
        else
        {
            out.contact_normal = (Vector2){0, -1};
        }
    }

    out.collision = true;
    return out;
}

//check collision of a moving rectangle against a stationary target.
collision_data collision_rectangle(Rectangle source, Vector2 src_velocity, Rectangle target)
{
    collision_data out;
    out.collision = false;
    if (src_velocity.x == 0 && src_velocity.y == 0) return out;

    Rectangle expanded_target;
    expanded_target.x = target.x - source.width / 2;
    expanded_target.y = target.y - source.height / 2;
    expanded_target.width = target.width + source.width;
    expanded_target.height = target.height + source.height;

    out = collision_ray_rect((Vector2){source.x + source.width / 2, source.y + source.height / 2}, Vector2Scale(src_velocity, GetFrameTime()), expanded_target);
    if(out.collision) out.collision = out.t_hit_near >= 0.0 && out.t_hit_near < 1.0f;
    return out;
}

//check collision of a dynamic rectangle against a rectangle target.
collision_data collision_dynamic_rectangle(DynamicRectangle source, Rectangle target)
{
    collision_data out;
    out = collision_rectangle(source.rectangle, source.velocity, target);
    return out;
}


void __attribute__ ((constructor)) initLibrary(void)
{
    printf("raylib rect-collision library initialized.\n");
}
