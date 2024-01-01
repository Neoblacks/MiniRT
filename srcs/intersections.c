/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 11:47:22 by casomarr          #+#    #+#             */
/*   Updated: 2024/01/01 18:06:29 by octonaute        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*Checks if the ray intersects with the current sphere.
If yes, checks if there is no other object intersecting
with the ray before it hits the sphere by comparing its
position.z to the z_index that keeps in memory the closest
intersection point (closest to the camera/origin of the ray).
If the spphere is closest, the variables z_index and
front_object_color are updated.
The discriminant determines how many times the ray intersects
with the sphere at this (x, y) point (it can pass through the
sphere and therefore intersect with it at two different z depths).
discriminant < 0 --> no intersection
discriminant = 0 --> intersects with the edge of the sphere
discriminant > 0 --> intersects the sphere twice (= it passes
through the "middle" of the sphere)*/
void	camera_sphere_intersection(bool *intersection, t_data *data, t_objs *sphere)
{
	float	a;
	float	b;
	float	c;
	float	discriminant;
	float	t;

	a = DotProduct(data->ray.direction, data->ray.direction);
	b = 2 * DotProduct(data->ray.direction, vecSubstract(data->ray.origin, sphere->position));
	c = DotProduct(vecSubstract(data->ray.origin, sphere->position), vecSubstract(data->ray.origin, sphere->position)) - powf(sphere->diameter, 2);
	
	discriminant = powf(b, 2) - (4 * a * c);
	if (discriminant == 0)
	{
		*intersection = true; //en un seul point (sur le bord)
		// printf("Intersection point = (%f, %f, %f)\n", data->intersection_point.x, data->intersection_point.y, data->intersection_point.z);
		// exit(1); //bizarre que discriminant jamais = 0
	}
	if (discriminant > 0)
	{
		*intersection = true; //en deux points
		// printf("Intersection point = (%f, %f, %f)\n", data->intersection_point.x, data->intersection_point.y, data->intersection_point.z);
		// exit(1); //Intersection point = (-nan, -nan, -nan) WTF
	}

	//on choisit le point d'intersection le plus proche de la camera
	if ((-b + sqrtf(discriminant)) / (2 * a) < (-b - sqrtf(discriminant)) / (2 * a))
		t = (-b + sqrtf(discriminant)) / (2 * a);
	else
		t = (-b - sqrtf(discriminant)) / (2 * a);
	
	data->intersection_point = vecAdd(data->ray.origin, vecMultiplyFloat(data->ray.direction, t));

	if (discriminant >= 0 && t > 0 && t < data->z_index) //t > 0 car sinon derriere camera
	{
		data->z_index = t; // et non data->intersection_point.z car peut etre négatif vu que 3D
		data->front_object_color = sphere->color.full;
		// printf("Sphere position: (%f, %f, %f)\n", sphere->position.x, sphere->position.y, sphere->position.z);
		// if (sphere->position.x != 200) //les deux sphères entrent dans cette fonction mais la première n'entre jamais dans cette condition
		// 	printf("sphere 1\n");
	}
	// printf("color = %d\n", data->front_object_color);

/////////////////////////////TEST
/* 	if (sphere->color.full == 16777215)
		printf("sphere 1\n");
	else if (sphere->color.full == 6579300)
		printf("sphere 2\n"); */
/////////////////////////////TEST

}

float vecMagnitude(const t_vec vector)
{
    return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

bool	sphere_intersection_check(t_ray *light_ray, t_objs *object, float light_distance)
{
	float	a;
	float	b;
	float	c;
	float	discriminant;
	float	t;

    a = DotProduct(light_ray->direction, light_ray->direction);
    b = 2 * DotProduct(light_ray->direction, vecSubstract(light_ray->origin, object->position));
    c = DotProduct(vecSubstract(light_ray->origin, object->position), vecSubstract(light_ray->origin, object->position)) - powf(object->diameter, 2);
    discriminant = powf(b, 2) - (4 * a * c);

    if (discriminant >= 0)
    {
        if ((-b + sqrtf(discriminant)) / (2 * a) < (-b - sqrtf(discriminant)) / (2 * a))
			t = (-b + sqrtf(discriminant)) / (2 * a);
		else
			t = (-b - sqrtf(discriminant)) / (2 * a);
        if (t > 0)
        {
            float distance_to_light = vecMagnitude(vecSubstract(vecAdd(light_ray->origin, vecMultiplyFloat(light_ray->direction, t)), light_ray->origin));
            if (distance_to_light < light_distance) //check si intersection point est plus proche que la light source
                return true; //rien sur le chemin du rayon
        }
    }
    return false; //qq chose bloque le chemin du rayon
}

void sphere_light_intersection(t_data *data, t_objs *object)
{
    t_objs *light;
    t_ray light_ray;
	float light_distance; // = norme

	light = get_node(data->scene.objs, LIGHT);
    light_ray.origin = light->position;
    light_ray.direction = vecSubstract(light->position, data->intersection_point);

	light_distance = sqrtf(light_ray.direction.x * light_ray.direction.x + \
				light_ray.direction.y * light_ray.direction.y + \
				light_ray.direction.z * light_ray.direction.z);
	if (light_distance > 0)
	{
		light_ray.direction.x /= light_distance;
		light_ray.direction.y /= light_distance;
		light_ray.direction.z /= light_distance;
	}
	else
	{
		light_ray.direction.x = 0.0f;
		light_ray.direction.y = 0.0f;
		light_ray.direction.z = 0.0f;
	}

	if (sphere_intersection_check(&light_ray, object, light_distance) == true) //modifier sphere_intersection pour que ça marche avec ça aussi
	{
		printf("YES 1\n"); //N'ENTRE JAMAIS DANS CETTE CONDITION
		data->direct_light = true;
	}
}

/*Checks if the current ray intersects with each of the objects
present in the scene. Every time an intersection occurs, 
data->z_index is updated to keep in memory what is the closest
object that intersects with the ray, so that our image doesn't show
the "hidden" objects.*/
bool	intersection(t_data *data)
{
	bool	intersection;
	t_objs	*object;

	object = data->scene.objs;
	intersection = false;
	data->direct_light = false;
	// data->z_index = FLT_MAX; //ne marche pas à la maison
	data->z_index = 100000; //`à 42 mettre FLT_MAX
	while (object)
	{
		if (object->type == SPHERE)
		{
			camera_sphere_intersection(&intersection, data, object);
			sphere_light_intersection(data, object); //calcul specific aux spheres!
			if (data->direct_light == true) //N'ENTRE JAMAIS DANS CETTE CONDITION!!
				printf("YES 2\n");
		}
		/* else if (object->type == CYLINDER)
			cylinder_intersection(&intersection, data);
		else if (object->type == PLANE)
			plane_intersection(&intersection, data); */
		object = object->next;
	}

/////////////////////////////TEST
/* 	if (data->front_object_color == 16777215)
		printf("FRONT SPHERE = sphere 1\n");
	else if (data->front_object_color == 6579300)
		printf("FRONT SPHERE = sphere 2\n");
	else
		printf("background\n"); */
/////////////////////////////TEST
	
	// printf("intersection = %d\n", intersection); //0 = false

	return (intersection);
}