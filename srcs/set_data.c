/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amugnier <amugnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 15:04:36 by amugnier          #+#    #+#             */
/*   Updated: 2024/01/12 15:05:04 by amugnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool	get_tvec_from_str(char *str, t_vec *v)
{
	char	**split;

	split = ft_split(str, ',');
	if (!split)
		return (false);
	v->x = ft_atof(split[0]);
	v->y = ft_atof(split[1]);
	v->z = ft_atof(split[2]);
	ft_free_split(split);
	return (true);
}

bool	get_trgb_from_str(char *str, t_rgb *rgb, t_scene *scene)
{
	char	**split;

	split = ft_split(str, ',');
	if (!split)
		return (false);
	rgb->full = 0;
	if ((ft_atoi(split[0]) > 255 || ft_atoi(split[0]) < 0)\
		|| (ft_atoi(split[1]) > 255 || ft_atoi(split[1]) < 0)\
		|| (ft_atoi(split[2]) > 255 || ft_atoi(split[2]) < 0))
	{
		ft_dprintf(2, ERROR_MSG1 "%s:%d: " ERROR_MSG2
			"Color must be between 0 and 255\n\x1B[0m", scene->fname,\
			scene->line);
		ft_free_split(split);
		return (false);
	}
	rgb->argb[0] = (t_uint8)ft_atoi(split[0]);
	rgb->argb[1] = (t_uint8)ft_atoi(split[1]);
	rgb->argb[2] = (t_uint8)ft_atoi(split[2]);
	ft_free_split(split);
	return (true);
}