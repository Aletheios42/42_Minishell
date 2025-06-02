/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:58:43 by elorente          #+#    #+#             */
/*   Updated: 2025/05/25 13:58:43 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../libft/libft.h"

// Helper function to print string arrays nicely
int	ft_print_string_array(char **array)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	if (!array)
		return (ft_printf("(null)"));
	count += ft_printf("[");
	while (array[i])
	{
		count += ft_printf("\"%s\"", array[i]);
		if (array[i + 1])
			count += ft_printf(", ");
		i++;
	}
	count += ft_printf("]");
	return (count);
}

// Print the node's hierarchical ID
int	print_node_id(char *prefix)
{
	return (ft_printf("[%s] ", prefix));
}

// Print connection lines for the tree
void	print_tree_lines(int depth, int is_last)
{
	int	i;

	i = 0;
	while (i < depth - 1)
	{
		ft_printf("│   ");
		i++;
	}
	if (depth > 0)
	{
		if (is_last)
			ft_printf("└── ");
		else
			ft_printf("├── ");
	}
}

// Recursive tree printing function
int	ft_print_tree_node(t_tree *tree, int dth, int lst, char *pre)
{
	int		count;
	char	left_prefix[32];
	char	right_prefix[32];

	count = 0;
	if (!tree)
		return (0);
	print_tree_lines(dth, lst);
	count += print_node_id(pre);
	count += print_tree_type_info(tree);
	count += ft_printf("\n");
	ft_strlcpy(left_prefix, pre, sizeof(left_prefix));
	ft_strlcat(left_prefix, ".1", sizeof(left_prefix));
	ft_strlcpy(right_prefix, pre, sizeof(right_prefix));
	ft_strlcat(right_prefix, ".2", sizeof(right_prefix));
	if (tree->left)
		count += ft_print_tree_node(
				tree->left,
				dth + 1,
				(tree->right == NULL),
				left_prefix
				);
	if (tree->right)
		count += ft_print_tree_node(tree->right, dth + 1, 1, right_prefix);
	return (count);
}

// Main tree printing function - fixed to use the unused parameter
int	ft_print_syntax_tree(t_tree *tree, int depth)
{
	(void)depth;
	return (ft_print_tree_node(tree, 0, 1, "1"));
}
