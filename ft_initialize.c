/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_initialize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfieux <jfieux@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 16:07:04 by jfieux            #+#    #+#             */
/*   Updated: 2021/03/17 11:32:14 by jfieux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_init_text(t_struct *info)
{
	int		cnt;
	char	*tmp;
	int		i;

	if (info->data[info->cnt] != '\0' && info->data[info->cnt] != '%')
	{
		cnt = 0;
		while (info->data[info->cnt] != '\0' && info->data[info->cnt] != '%')
		{
			info->cnt++;
			cnt++;
		}
		info->cnt = info->cnt - cnt;
		if (!(tmp = malloc(sizeof(char) * (cnt + 1))))
			return (0);
		i = 0;
		while (i < cnt)
			tmp[i++] = info->data[info->cnt++];
		tmp[i] = '\0';
		if (!(info->res = ft_strjoin(info->res, tmp, info)))
			return (0);
	}
	return (1);
}

char	*ft_init_flag(int len_flag, t_struct *info)
{
	char	*flag;

	if (!(flag = malloc(sizeof(char) * (len_flag + 1))))
		return (NULL);
	flag[len_flag--] = '\0';
	if (info->letter == 'c')
		if (ft_verif_c(&flag, len_flag, info) != 0)
		{
			free(flag);
			return (NULL);
		}
	if (info->letter == 'i' || info->letter == 'd' || info->letter == '%')
		if (ft_verif_id(&flag, len_flag, info, 1) != 0)
		{
			free(flag);
			return (NULL);
		}
	if (info->letter == 's' || info->letter == 'p' || info->letter == 'x' ||
	info->letter == 'X' || info->letter == 'u')
		if (ft_verif_spuxx(&flag, len_flag, info, 1) != 0)
		{
			free(flag);
			return (NULL);
		}
	return (flag);
}

int		ft_treat_cspdi(t_struct *info, int i, char **arg, va_list param)
{
	if (info->letter == 'c')
	{
		if (info->l == 1)
			i = ft_treat_char(va_arg(param, long int), arg);
		else if (info->l == 2)
			i = ft_treat_char(va_arg(param, long long int), arg);
		else
			i = ft_treat_char(va_arg(param, int), arg);
	}
	if (info->letter == 's')
		i = ft_treat_str(va_arg(param, char *), arg);
	if (info->letter == 'p')
		i = ft_treat_pointer(va_arg(param, unsigned long long), arg);
	if (info->letter == 'd' || info->letter == 'i')
	{
		if (info->l == 1)
			i = ft_treat_int(va_arg(param, long int), arg);
		else if (info->l == 2)
			i = ft_treat_int(va_arg(param, long long int), arg);
		else
			i = ft_treat_int(va_arg(param, int), arg);
	}
	return (i);
}

int		ft_treat_xx(t_struct *info, char **arg, int i, va_list param)
{
	if (info->letter == 'x')
	{
		if (info->l == 1)
			i = ft_treat_hexa(va_arg(param, unsigned long int), arg, 0);
		else if (info->l == 2)
			i = ft_treat_hexa(va_arg(param, unsigned long long int), arg, 0);
		else
			i = ft_treat_hexa(va_arg(param, unsigned int), arg, 0);
	}
	if (info->letter == 'X')
	{
		if (info->l == 1)
			i = ft_treat_hexa(va_arg(param, unsigned long int), arg, 1);
		else if (info->l == 2)
			i = ft_treat_hexa(va_arg(param, unsigned long long int), arg, 1);
		else
			i = ft_treat_hexa(va_arg(param, unsigned int), arg, 1);
	}
	return (i);
}

char	*ft_init_arg(t_struct *info, va_list param)
{
	char	*arg;
	int		i;

	i = 0;
	i = ft_treat_cspdi(info, i, &arg, param);
	if (info->letter == 'u')
	{
		if (info->l == 1)
			i = ft_treat_uint(va_arg(param, unsigned long int), &arg);
		else if (info->l == 2)
			i = ft_treat_uint(va_arg(param, unsigned long long int), &arg);
		else
			i = ft_treat_uint(va_arg(param, unsigned int), &arg);
	}
	i = ft_treat_xx(info, &arg, i, param);
	if (info->letter == '%')
		i = ft_treat_char('%', &arg);
	if (!i)
		return (NULL);
	return (arg);
}

int		ft_nb_arg(t_struct *info, char *flag, va_list param, char **arg)
{
	int s;

	s = 0;
	info->i = 0;
	info->nbz = 0;
	while (flag[s] == '-' || flag[s] == '+' || flag[s] == ' ')
		s++;
	if ((info->nbs = ft_nb_space(flag, info, s, param)) < 0)
		return (-1);
	if (flag[info->i + s] == '.')
	{
		info->i++;
		if ((info->nbz = ft_nb_zero(flag, info, s, param)) < 0)
			return (-1);
		info->i--;
	}
	if (!(*arg = ft_init_arg(info, param)))
		return (-1);
	return (s);
}

char	*ft_fillin_all(t_struct *info, char *arg)
{
	char	*tmp;

	if (info->letter == 'c' && arg[0] == '\0' && info->nbs > 0)
		info->nbs--;
	if (!(tmp = ft_malloc_tmp(info, arg)))
		return (NULL);
	if (info->minus == 1)
	{
		if (info->letter == 's')
			tmp = ft_fillin_strmin(info, tmp, arg);
		else if (info->letter == 'i' || info->letter == 'd')
			tmp = ft_fillin_intmin(info, tmp, arg);
		else
			tmp = ft_fillin_othermin(info, tmp, arg);
	}
	else
	{
		if (info->letter == 's')
			tmp = ft_fillin_str(info, tmp, arg);
		else if (info->letter == 'i' || info->letter == 'd')
			tmp = ft_fillin_int(info, tmp, arg);
		else
			tmp = ft_fillin_other(info, tmp, arg);
	}
	return (tmp);
}

void	ft_nbz_equal_nbs(char *flag, t_struct *info, int s, char *arg)
{
	if (flag[info->i + s] != '.')
	{
		if (info->zero == 1 && info->minus == 0 && (info->letter == 'd' ||
		info->letter == 'i' || info->letter == 'u' || info->letter == 'x' ||
		info->letter == 'X' || info->letter == '%'))
		{
			if (info->sharp == 0)
				info->nbz = info->nbs;
			else
			{
				if (arg[0] == '0')
					info->nbz = info->nbs;
			}
		}
	}
}

int		ft_init_res(t_struct *info, char *flag, va_list param)
{
	int		s;
	char	*tmp;
	char	*arg;

	if ((s = ft_nb_arg(info, flag, param, &arg)) < 0)
		return (0);
	ft_nbz_equal_nbs(flag, info, s, arg);
	if (!(tmp = ft_fillin_all(info, arg)))
		return (0);
	if (info->letter == 'c' && !arg[0])
		if (!(ft_z_co(info)))
			return (0);
	free(arg);
	if (!(info->res = ft_strjoin(info->res, tmp, info)))
		return (0);
	return (1);
}
