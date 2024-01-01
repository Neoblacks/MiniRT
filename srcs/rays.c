/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 15:53:12 by casomarr          #+#    #+#             */
/*   Updated: 2024/01/01 17:28:39 by octonaute        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*Initializes the parameters needed for ray generation each
time the camera moves*/
/* void	ray_init(t_data *data)
{
	t_ray	*ray;

	ray = data->ray;

	// int	aspect_ratio = 16.0 / 9.0; //ajoute pour ray_generation
	// data->img.height = data->img.width / aspect_ratio; //ajoute pour ray_generation
	
	//Camera
	float	viewport_height = 2.0;
	float	viewport_width = viewport_height * (data->img.width /data->img.height);
	
	//on calcule l'espace entre chaque pixel (a quel point les rayons seront proches)
	ray.pixel_delta_w = 1 / WIN_WIDTH; //soit 1 soit viewport
	ray.pixel_delta_h = 1 / WIN_HEIGHT; //soit 1 soit viewport
	
	//zone (=viewport) correspondant au pixel en haut a gauche
    viewport_current_pixel.x -= viewport_width;
    viewport_current_pixel.y -= viewport_height;

	//viewport pour chaque pixel
	t_vec	vector_pixel_delta = {0.5f * (ray.pixel_delta_w + ray.pixel_delta_h), \
	0.5f * (ray.pixel_delta_w + ray.pixel_delta_h), 0.0f}; //c'est bien pour l'offset qu on le multiplie par 0.5?

	//localisation du premier viewport en haut a gauche
	//ray.current_pixel_location = vecAdd(viewport_current_pixel, vector_pixel_delta);
} */

/*Calculates the norm of the angle of the ray from
camera to object and object to light source and then
calculates the norm of this angle.*/
void	get_norm(t_ray ray)
{
	ray.norm = sqrtf(ray.direction.x * ray.direction.x + \
				ray.direction.y * ray.direction.y + \
				ray.direction.z * ray.direction.z);
}

void	normalize_direction_vector(t_ray ray)
{
	ray.direction.x /= ray.norm;
	ray.direction.y /= ray.norm;
	ray.direction.z /= ray.norm;
}

/*Generates each ray. They all have the same origin (the camera center)
but their direction changes (they reach a different pixel on the canevas
and continue in that direction into the scene)*/
void	generate_current_ray(t_data *data)
{
	t_objs	*camera;

	//segfault ligne suivante : une seule node dans objs
	camera = get_node(data->scene.objs, CAMERA);
	if (camera == NULL)
	{
		ft_dprintf(2, "Error\nCrash getting camera node\n");
		return ;
	}
	data->ray.origin = camera->position;
	// printf("ray.origin = %f, %f, %f\n", camera->position.x, camera->position.y, camera->position.z);
	data->ray.current_pixel = create_vec(data->x, data->y, 1); //1 = focal length
	// printf("ray.current_pixel = %f, %f, %f\n", ray.current_pixel.x, ray.current_pixel.y, ray.current_pixel.z);
	data->ray.direction = vecSubstract(data->ray.current_pixel, data->ray.origin);
	// printf("ray.direction = %f, %f, %f\n", ray.direction.x, ray.direction.y, ray.direction.z);
	get_norm(data->ray);
	normalize_direction_vector(data->ray);
}

/*Calculates each ray's direction.*/
void	ray_generation(t_data *data)
{
	//ray_init(data);
	
	data->y = 0;
	while (data->y < WIN_HEIGHT)
	{
		data->x = 0;
		while (data->x < WIN_WIDTH)
		{
			generate_current_ray(data);
			if (intersection(data) == true)
			{
				//determine_pixel(); //necessaire?
				img_pix_put(data, data->intersection_point.x, data->intersection_point.y, determine_pixel_color(data));
				// mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.mlx_img, 0, 0);
			}
			// else
			// 	img_pix_put(data, data->x, data->y, 25600);
			data->x++;
		}
		data->y++;
	}
	printf("FINI WHILE\n");
}

