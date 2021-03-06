/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm_run_vm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adietric <adietric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/06 16:40:12 by adietric          #+#    #+#             */
/*   Updated: 2020/01/13 08:57:46 by adietric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm.h"

static t_process	*check_is_rewritten(t_all *vm, t_process *p)
{
	if (vm->arena[p->pc % MEM_SIZE] != p->op)
	{
		p->pc -= 1;
		load_new_process(vm, p);
		--p->exec_cycle;
	}
	return (p);
}

static int			run_processes(t_all *vm, t_process **head, t_op *op_table)
{
	t_process	*tracker;
	int			live;

	live = vm->cycles_to_die;
	while (live > 0)
	{
		tracker = (*head);
		++vm->cycles;
		if (vm->flag_v == 2)
			ft_printf("It is now cycle %d\n", vm->cycles);
		while (tracker != NULL)
		{
			tracker = check_is_rewritten(vm, tracker);
			if (vm->cycles == tracker->exec_cycle)
				head = exec_process(vm, &tracker, op_table, head);
			tracker = tracker->next;
		}
		--live;
		if (vm->flag_dump != -1 && vm->cycles >= vm->flag_dump)
		{
			free_all_process(vm, (*head));
			return (-2);
		}
	}
	return (0);
}

static void			declare_winner(t_all *vm)
{
	int			i;
	int			last;

	i = -1;
	last = vm->order[vm->total_champ - 1];
	while (i++ < vm->total_champ)
	{
		if (vm->champs[i].id == vm->last_alive && vm->last_alive != 0)
		{
			ft_printf("Contestant %d, \"%s\", has won !\n",
			vm->champs[i].id, vm->champs[i].name);
			return ;
		}
	}
	ft_printf("Contestant %d, \"%s\", has won !\n",
		vm->champs[last].id, vm->champs[last].name);
}

static int			dump(t_all *vm, t_process *p, t_op *op)
{
	ft_print_arena(vm, 32);
	free(op);
	return (0);
}

int					run_vm(t_all *vm, t_process *process)
{
	t_op		*op_table;

	if (!(op_table = (t_op*)malloc(sizeof(t_op) * 16)))
	{
		free(vm->order);
		free(vm->arena);
		error(vm, "Malloc error run_vm.\n");
	}
	op_table = init_op_check(vm, op_table);
	vm->cycles_to_die = CYCLE_TO_DIE;
	process = load_processes(vm, process);
	if (process->start == -1)
		return (error_run_vm(vm, op_table));
	while (check_alive(vm, &process) == 1)
	{
		if ((run_processes(vm, &process, op_table)) == -2)
			return (dump(vm, process, op_table));
	}
	declare_winner(vm);
	free(op_table);
	return (0);
}
