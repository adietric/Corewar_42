/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm_check_op_block.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adietric <adietric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 16:50:21 by adietric          #+#    #+#             */
/*   Updated: 2020/01/06 12:34:35 by adietric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm.h"

static int	check_comb_op_ocp(t_all *vm, t_process *process)
{
	int		val_ocp;
	int		op;

	op = process->op;
	val_ocp = (int)get_next_bytes(vm, process, 1, 0);
	if (if_no_opcode(process) == 1)
		return (1);
	if (val_ocp == 0
		|| is_in(val_ocp, g_op_check_tab[process->op - 1].ocp_value) != 1)
		return (0);
	return (1);
}

int			check_op_block(t_all *vm, t_process *process)
{
	int jump;
	int bytes;

	jump = 0;
	if (process->op < 1 || process->op > 16)
		return (0);
	if ((check_comb_op_ocp(vm, process)) == 0)
	{
		recalc_bytes(process, &bytes);
		process->pc += bytes;
		return (0);
	}
	return (1);
}

int			is_operation(int op)
{
	if (op < 1 || op > 16)
		return (0);
	return (1);
}
