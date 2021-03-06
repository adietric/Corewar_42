/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flafonso <flafonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/27 18:27:24 by flafonso          #+#    #+#             */
/*   Updated: 2020/01/09 18:52:28 by flafonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VM_H
# define VM_H

# include <stdio.h>
# include "../libft/ft_printf.h"
# include "../libft/libft.h"
# include "op.h"
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <limits.h>

typedef	struct			s_champs
{
	char				*path;
	int					id;
	unsigned char		*name;
	unsigned char		*comment;
	size_t				len_exec_code;
	unsigned char		*exec_code;
	int					start;
	int					last_live;
	int					pc;
}						t_champs;

typedef struct			s_all
{
	struct s_champs		champs[MAX_PLAYERS];
	int					flag_dump;
	int					flag_n;
	int					flag_v;
	int					total_champ;
	unsigned char		*arena;
	int					last_alive;
	int					last_alive_cycle;
	int					cycles;
	int					nbr_live_since_check;
	int					cycles_to_die;
	int					total_checks;
	int					check_mode;
	int					total_process;
	int					*order;
	int					ac;
	int					dead;

}						t_all;

typedef struct			s_process
{
	int					id;
	int					pid;
	int					r[REG_NUMBER];
	int					pc;
	char				carry;
	int					live_calls;
	int					start;
	int					exec_cycle;
	int					bytes;
	int					*decode;
	int					opc;
	unsigned char		op;
	int					op_fail;
	struct s_process	*next;
}						t_process;

typedef	struct			s_op
{
	void				(*inst)(t_all *vm, t_process *p);
}						t_op;

typedef struct			s_op_check
{
	char				*name;
	int					nb_param;
	int					ocp_value[9];
	int					op_code;
	int					nb_cycle;
	int					dir_size;
}						t_op_check;

/*
** OPERATIONS
*/
void					op_add(t_all *vm, t_process *p);
void					op_aff(t_all *vm, t_process *p);
void					op_and(t_all *vm, t_process *p);
void					op_fork(t_all *vm, t_process *p);
void					op_ld(t_all *vm, t_process *p);
void					op_ldi(t_all *vm, t_process *p);
void					op_lfork(t_all *vm, t_process *p);
void					op_live(t_all *vm, t_process *p);
void					op_lld(t_all *vm, t_process *p);
void					op_lldi(t_all *vm, t_process *p);
void					op_or(t_all *vm, t_process *p);
void					op_st(t_all *vm, t_process *p);
void					op_sti(t_all *vm, t_process *p);
void					op_sub(t_all *vm, t_process *p);
void					op_xor(t_all *vm, t_process *p);
void					op_zjmp(t_all *vm, t_process *p);

/*
** DASM
*/

size_t					dasm_get_data(t_all *all, char *cor_file,
							uint8_t **cor_content);
int						dasm_get_exec_code(uint8_t *cor_content, int len);
void					dasm_get_header(t_all *all, size_t cor_size,
							uint8_t **cor_content, t_champs *champs);
t_all					*dasm_init(t_all *vm, int ac, char **av);
int						ft_jump_flags(int ac, char **av, int *i, t_all *all);
void					dasm_input(int ac, char **av, t_all *all);
int						dasm_is_it_cor(char *path);
void					dasm_free(t_all *all);
void					error_size(t_all *all, char *path, size_t cor_size);
void					error(t_all *all, char *str);
void					print_usage(t_all *all);
int						just_number(char *str);
int						is_in(int i, int value[9]);
int						just_nb_atoi(const char *str, t_all *all);
void					*dasm_memmalloccopy(void *content, size_t prev_size,
							size_t all_size);

/*
** VM
*/

t_op					*init_op_check(t_all *vm, t_op *op);
t_process				**kill_dead_process(t_all *vm,
							t_process **p, t_process *prev);
int						check_alive(t_all *vm, t_process **p);
int						is_operation(int op);
int						check_op_block(t_all *vm, t_process *process);
t_op_check				g_op_check_tab[17];
void					calc_bytes(t_process *p, int *bytes);
t_process				*ft_decode_byte(unsigned char c, t_process *p);
int						if_no_opcode(t_process *p);
t_process				*error_process(t_process *p);
void					error_arena(t_all *vm, t_process *p);
int						error_run_vm(t_all *vm, t_op *op_table);
void					end_prog(t_all *vm, t_process *head, t_op *op);
void					free_all_process(t_all *vm, t_process *p);
void					free_process(t_all *vm, t_process *p);
void					free_vm(t_all *vm);
int						free_all(t_all *vm, t_process *p);
void					free_dasm_header(t_all *all, uint8_t *cor_content,
							char *s);
long					get_reg_val(t_all *vm, t_process *p, int bytes_read);
long					get_ind(t_all *vm, t_process *p, long bytes_read,
							int idx);
long					get_val_at_ind(t_all *vm, t_process *p, long bytes_read,
							int idx);
long					get_unspecified_val(t_all *vm, t_process *p,
							long *bytes, int param);
long					get_unspecified_val_2(t_all *vm, t_process *p,
							long *bytes, int param);
int						get_duration(t_all *vm, int opc);
t_process				*init_process(t_all *vm, t_champs *c, t_process *p);
t_all					*init_arena(t_all *vm);
t_process				*load_processes(t_all *vm, t_process *head);
void					load_new_process(t_all *vm, t_process *p);
t_process				**exec_process(t_all *vm, t_process **p, t_op *op,
							t_process **head);
void					load_value(t_all *vm, int add, int len, long val);
void					load_val_in_reg(t_all *vm, t_process *p, int val,
							int bytes_read);
void					ft_print_arena(t_all *vm, int len);
void					vm_print_intro(t_all *vm);
int						recalc_bytes(t_process *p, int *bytes);
void					check_carry(t_process *p, int param);
long					get_next_bytes(t_all *vm, t_process *p, int len,
							long bytes_read);
long					check_neg_address(long val);
void					define_check(void);
int						run_vm(t_all *vm, t_process *process);

#endif
