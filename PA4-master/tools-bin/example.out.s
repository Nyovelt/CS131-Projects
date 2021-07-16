# start of generated code
# start of global data
	.data
	.align	2
# Class name table
	.globl	class_nameTab
# 
# 	Prototype object of Main
	.globl	Main_protObj
# 	Prototype object of Int
	.globl	Int_protObj
# 	Prototype object of String
	.globl	String_protObj
# 	Bool const False
	.globl	bool_const0
# 	Bool const True
	.globl	bool_const1
# 	Tags
	.globl	_int_tag
	.globl	_bool_tag
	.globl	_string_tag
# 
# 	Int, Bool and String tags
_int_tag:
	.word	3
_bool_tag:
	.word	2
_string_tag:
	.word	1
# end of global data
# 
# start choose GC: 0
	.globl	_MemMgr_INITIALIZER
_MemMgr_INITIALIZER:
	.word	_NoGC_Init
	.globl	_MemMgr_COLLECTOR
_MemMgr_COLLECTOR:
	.word	_NoGC_Collect
	.globl	_MemMgr_TEST
_MemMgr_TEST:
	.word	0
# end choose GC
# 
# define str_const 
	.word	-1
str_const17:
	.word	1	# tag
	.word	5	# size
	.word	String_dispTab	# dispatch table
	.word	int_const1	# string length
	.byte	0	
	.align	2
# define str_const Main
	.word	-1
str_const16:
	.word	1	# tag
	.word	6	# size
	.word	String_dispTab	# dispatch table
	.word	int_const3	# string length
	.ascii	"Main"
	.byte	0	
	.align	2
# define str_const CellularAutomaton
	.word	-1
str_const15:
	.word	1	# tag
	.word	9	# size
	.word	String_dispTab	# dispatch table
	.word	int_const4	# string length
	.ascii	"CellularAutomaton"
	.byte	0	
	.align	2
# define str_const String
	.word	-1
str_const14:
	.word	1	# tag
	.word	6	# size
	.word	String_dispTab	# dispatch table
	.word	int_const5	# string length
	.ascii	"String"
	.byte	0	
	.align	2
# define str_const Bool
	.word	-1
str_const13:
	.word	1	# tag
	.word	6	# size
	.word	String_dispTab	# dispatch table
	.word	int_const3	# string length
	.ascii	"Bool"
	.byte	0	
	.align	2
# define str_const Int
	.word	-1
str_const12:
	.word	1	# tag
	.word	5	# size
	.word	String_dispTab	# dispatch table
	.word	int_const6	# string length
	.ascii	"Int"
	.byte	0	
	.align	2
# define str_const IO
	.word	-1
str_const11:
	.word	1	# tag
	.word	5	# size
	.word	String_dispTab	# dispatch table
	.word	int_const7	# string length
	.ascii	"IO"
	.byte	0	
	.align	2
# define str_const Object
	.word	-1
str_const10:
	.word	1	# tag
	.word	6	# size
	.word	String_dispTab	# dispatch table
	.word	int_const5	# string length
	.ascii	"Object"
	.byte	0	
	.align	2
# define str_const _prim_slot
	.word	-1
str_const9:
	.word	1	# tag
	.word	7	# size
	.word	String_dispTab	# dispatch table
	.word	int_const8	# string length
	.ascii	"_prim_slot"
	.byte	0	
	.align	2
# define str_const SELF_TYPE
	.word	-1
str_const8:
	.word	1	# tag
	.word	7	# size
	.word	String_dispTab	# dispatch table
	.word	int_const9	# string length
	.ascii	"SELF_TYPE"
	.byte	0	
	.align	2
# define str_const _no_class
	.word	-1
str_const7:
	.word	1	# tag
	.word	7	# size
	.word	String_dispTab	# dispatch table
	.word	int_const9	# string length
	.ascii	"_no_class"
	.byte	0	
	.align	2
# define str_const <basic class>
	.word	-1
str_const6:
	.word	1	# tag
	.word	8	# size
	.word	String_dispTab	# dispatch table
	.word	int_const10	# string length
	.ascii	"<basic class>"
	.byte	0	
	.align	2
# define str_const          X         
	.word	-1
str_const5:
	.word	1	# tag
	.word	9	# size
	.word	String_dispTab	# dispatch table
	.word	int_const11	# string length
	.ascii	"         X         "
	.byte	0	
	.align	2
# define str_const 123
	.word	-1
str_const4:
	.word	1	# tag
	.word	5	# size
	.word	String_dispTab	# dispatch table
	.word	int_const6	# string length
	.ascii	"123"
	.byte	0	
	.align	2
# define str_const .
	.word	-1
str_const3:
	.word	1	# tag
	.word	5	# size
	.word	String_dispTab	# dispatch table
	.word	int_const0	# string length
	.ascii	"."
	.byte	0	
	.align	2
# define str_const X
	.word	-1
str_const2:
	.word	1	# tag
	.word	5	# size
	.word	String_dispTab	# dispatch table
	.word	int_const0	# string length
	.ascii	"X"
	.byte	0	
	.align	2
# define str_const 

	.word	-1
str_const1:
	.word	1	# tag
	.word	5	# size
	.word	String_dispTab	# dispatch table
	.word	int_const0	# string length
	.ascii	"\n"
	.byte	0	
	.align	2
# define str_const /home/xa0082249956/Homeworks/CS131/PA4/tools-bin/example.cl
	.word	-1
str_const0:
	.word	1	# tag
	.word	19	# size
	.word	String_dispTab	# dispatch table
	.word	int_const12	# string length
	.ascii	"/home/xa0082249956/Homeworks/CS131/PA4/tools-bin/example.cl"
	.byte	0	
	.align	2
	.word	-1
int_const12:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	59	# integer value
	.word	-1
int_const11:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	19	# integer value
	.word	-1
int_const10:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	13	# integer value
	.word	-1
int_const9:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	9	# integer value
	.word	-1
int_const8:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	10	# integer value
	.word	-1
int_const7:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	2	# integer value
	.word	-1
int_const6:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	3	# integer value
	.word	-1
int_const5:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	6	# integer value
	.word	-1
int_const4:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	17	# integer value
	.word	-1
int_const3:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	4	# integer value
	.word	-1
int_const2:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	20	# integer value
	.word	-1
int_const1:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	0	# integer value
	.word	-1
int_const0:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	1	# integer value
	.word	-1
bool_const0:
	.word	2	# class tag
	.word	4	# object size
	.word	Bool_dispTab	# dispatch table
	.word	0	# value (0 or 1)
	.word	-1
bool_const1:
	.word	2	# class tag
	.word	4	# object size
	.word	Bool_dispTab	# dispatch table
	.word	1	# value (0 or 1)
# start class name table
# 	A table, which at index (class tag) ∗ 4 contains a pointer Data
# 	to a String object containing the name of the class associated.
class_nameTab:
	.word	str_const10	# Object (0)
	.word	str_const14	# String (1)
	.word	str_const13	# Bool (2)
	.word	str_const12	# Int (3)
	.word	str_const11	# IO (4)
	.word	str_const16	# Main (5)
	.word	str_const15	# CellularAutomaton (6)
# end class name table
# start class object table
# 	A table, which at index (class tag) ∗ 8 contains a pointer to
# 	the prototype object and at index (class tag) ∗ 8 + 4 contains
# 	a pointer to the initialization method for that class.
class_objTab:
# 	Object (0)
	.word	Object_protObj	# prototype table
	.word	Object_init	# init method
# 	String (1)
	.word	String_protObj	# prototype table
	.word	String_init	# init method
# 	Bool (2)
	.word	Bool_protObj	# prototype table
	.word	Bool_init	# init method
# 	Int (3)
	.word	Int_protObj	# prototype table
	.word	Int_init	# init method
# 	IO (4)
	.word	IO_protObj	# prototype table
	.word	IO_init	# init method
# 	Main (5)
	.word	Main_protObj	# prototype table
	.word	Main_init	# init method
# 	CellularAutomaton (6)
	.word	CellularAutomaton_protObj	# prototype table
	.word	CellularAutomaton_init	# init method
# end class object table
# start of dispatch table
Object_dispTab:
	.word	Object.abort	# 0
	.word	Object.type_name	# 1
	.word	Object.copy	# 2
String_dispTab:
	.word	Object.abort	# 0
	.word	Object.type_name	# 1
	.word	Object.copy	# 2
	.word	String.length	# 3
	.word	String.concat	# 4
	.word	String.substr	# 5
Bool_dispTab:
	.word	Object.abort	# 0
	.word	Object.type_name	# 1
	.word	Object.copy	# 2
Int_dispTab:
	.word	Object.abort	# 0
	.word	Object.type_name	# 1
	.word	Object.copy	# 2
IO_dispTab:
	.word	Object.abort	# 0
	.word	Object.type_name	# 1
	.word	Object.copy	# 2
	.word	IO.out_string	# 3
	.word	IO.out_int	# 4
	.word	IO.in_string	# 5
	.word	IO.in_int	# 6
Main_dispTab:
	.word	Object.abort	# 0
	.word	Object.type_name	# 1
	.word	Object.copy	# 2
	.word	IO.out_string	# 3
	.word	IO.out_int	# 4
	.word	IO.in_string	# 5
	.word	IO.in_int	# 6
	.word	Main.main	# 7
CellularAutomaton_dispTab:
	.word	Object.abort	# 0
	.word	Object.type_name	# 1
	.word	Object.copy	# 2
	.word	IO.out_string	# 3
	.word	IO.out_int	# 4
	.word	IO.in_string	# 5
	.word	IO.in_int	# 6
	.word	CellularAutomaton.init	# 7
	.word	CellularAutomaton.print	# 8
	.word	CellularAutomaton.num_cells	# 9
	.word	CellularAutomaton.cell	# 10
	.word	CellularAutomaton.cell_left_neighbor	# 11
	.word	CellularAutomaton.cell_right_neighbor	# 12
	.word	CellularAutomaton.cell_at_next_evolution	# 13
	.word	CellularAutomaton.evolve	# 14
# end of dispatch table)
# start of prototype table
	.word	-1
Object_protObj:
	.word	0	# 0 class tag
	.word	3	# 1 length
	.word	Object_dispTab	# 2 dispatch table
	.word	-1
String_protObj:
	.word	1	# 0 class tag
	.word	5	# 1 length
	.word	String_dispTab	# 2 dispatch table
	.word	int_const1	# 3 String._val: Int
	.word	0	# 4 String._str_field: _prim_slot
	.word	-1
Bool_protObj:
	.word	2	# 0 class tag
	.word	4	# 1 length
	.word	Bool_dispTab	# 2 dispatch table
	.word	0	# 3 Bool._val: _prim_slot
	.word	-1
Int_protObj:
	.word	3	# 0 class tag
	.word	4	# 1 length
	.word	Int_dispTab	# 2 dispatch table
	.word	0	# 3 Int._val: _prim_slot
	.word	-1
IO_protObj:
	.word	4	# 0 class tag
	.word	3	# 1 length
	.word	IO_dispTab	# 2 dispatch table
	.word	-1
Main_protObj:
	.word	5	# 0 class tag
	.word	4	# 1 length
	.word	Main_dispTab	# 2 dispatch table
	.word	0	# 3 Main.cells: CellularAutomaton
	.word	-1
CellularAutomaton_protObj:
	.word	6	# 0 class tag
	.word	4	# 1 length
	.word	CellularAutomaton_dispTab	# 2 dispatch table
	.word	str_const17	# 3 CellularAutomaton.population_map: String
# end of prototype table
# start global text
	.globl	heap_start
heap_start:
	.word	0
	.text
	.globl	Main_init
	.globl	Int_init
	.globl	String_init
	.globl	Bool_init
	.globl	Main.main
# end global text
# start of initializer
Object_init:
# activate with 0 temps
# 	push activation record
# 		push $fp
	addiu	$sp $sp -4
	sw	$fp 4($sp)
# 		push finish
# 		push $s0
	addiu	$sp $sp -4
	sw	$s0 4($sp)
# 		push finish
# 		push $ra
	addiu	$sp $sp -4
	sw	$ra 4($sp)
# 		push finish
# 	advance frame pointer
	addiu	$fp $sp 4
# 	self = accumulator
	move	$s0 $a0
# init attributes
# 	accumulator = self
	move	$a0 $s0
# deactivate with 0 args and 0 temps
# 	pop activation record
# 		pop to $ra
	lw	$ra 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $s0
	lw	$s0 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $fp
	lw	$fp 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 	return
	jr	$ra	
String_init:
# activate with 0 temps
# 	push activation record
# 		push $fp
	addiu	$sp $sp -4
	sw	$fp 4($sp)
# 		push finish
# 		push $s0
	addiu	$sp $sp -4
	sw	$s0 4($sp)
# 		push finish
# 		push $ra
	addiu	$sp $sp -4
	sw	$ra 4($sp)
# 		push finish
# 	advance frame pointer
	addiu	$fp $sp 4
# 	self = accumulator
	move	$s0 $a0
# call parent init
	jal	Object_init
# init attributes
# - 3 _val: Int no init
# - 4 _str_field: _prim_slot no init
# 	accumulator = self
	move	$a0 $s0
# deactivate with 0 args and 0 temps
# 	pop activation record
# 		pop to $ra
	lw	$ra 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $s0
	lw	$s0 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $fp
	lw	$fp 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 	return
	jr	$ra	
Bool_init:
# activate with 0 temps
# 	push activation record
# 		push $fp
	addiu	$sp $sp -4
	sw	$fp 4($sp)
# 		push finish
# 		push $s0
	addiu	$sp $sp -4
	sw	$s0 4($sp)
# 		push finish
# 		push $ra
	addiu	$sp $sp -4
	sw	$ra 4($sp)
# 		push finish
# 	advance frame pointer
	addiu	$fp $sp 4
# 	self = accumulator
	move	$s0 $a0
# call parent init
	jal	Object_init
# init attributes
# - 3 _val: _prim_slot no init
# 	accumulator = self
	move	$a0 $s0
# deactivate with 0 args and 0 temps
# 	pop activation record
# 		pop to $ra
	lw	$ra 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $s0
	lw	$s0 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $fp
	lw	$fp 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 	return
	jr	$ra	
Int_init:
# activate with 0 temps
# 	push activation record
# 		push $fp
	addiu	$sp $sp -4
	sw	$fp 4($sp)
# 		push finish
# 		push $s0
	addiu	$sp $sp -4
	sw	$s0 4($sp)
# 		push finish
# 		push $ra
	addiu	$sp $sp -4
	sw	$ra 4($sp)
# 		push finish
# 	advance frame pointer
	addiu	$fp $sp 4
# 	self = accumulator
	move	$s0 $a0
# call parent init
	jal	Object_init
# init attributes
# - 3 _val: _prim_slot no init
# 	accumulator = self
	move	$a0 $s0
# deactivate with 0 args and 0 temps
# 	pop activation record
# 		pop to $ra
	lw	$ra 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $s0
	lw	$s0 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $fp
	lw	$fp 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 	return
	jr	$ra	
IO_init:
# activate with 0 temps
# 	push activation record
# 		push $fp
	addiu	$sp $sp -4
	sw	$fp 4($sp)
# 		push finish
# 		push $s0
	addiu	$sp $sp -4
	sw	$s0 4($sp)
# 		push finish
# 		push $ra
	addiu	$sp $sp -4
	sw	$ra 4($sp)
# 		push finish
# 	advance frame pointer
	addiu	$fp $sp 4
# 	self = accumulator
	move	$s0 $a0
# call parent init
	jal	Object_init
# init attributes
# 	accumulator = self
	move	$a0 $s0
# deactivate with 0 args and 0 temps
# 	pop activation record
# 		pop to $ra
	lw	$ra 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $s0
	lw	$s0 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $fp
	lw	$fp 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 	return
	jr	$ra	
Main_init:
# activate with 0 temps
# 	push activation record
# 		push $fp
	addiu	$sp $sp -4
	sw	$fp 4($sp)
# 		push finish
# 		push $s0
	addiu	$sp $sp -4
	sw	$s0 4($sp)
# 		push finish
# 		push $ra
	addiu	$sp $sp -4
	sw	$ra 4($sp)
# 		push finish
# 	advance frame pointer
	addiu	$fp $sp 4
# 	self = accumulator
	move	$s0 $a0
# call parent init
	jal	IO_init
# init attributes
# - 3 cells: CellularAutomaton no init
# 	accumulator = self
	move	$a0 $s0
# deactivate with 0 args and 0 temps
# 	pop activation record
# 		pop to $ra
	lw	$ra 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $s0
	lw	$s0 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $fp
	lw	$fp 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 	return
	jr	$ra	
CellularAutomaton_init:
# activate with 0 temps
# 	push activation record
# 		push $fp
	addiu	$sp $sp -4
	sw	$fp 4($sp)
# 		push finish
# 		push $s0
	addiu	$sp $sp -4
	sw	$s0 4($sp)
# 		push finish
# 		push $ra
	addiu	$sp $sp -4
	sw	$ra 4($sp)
# 		push finish
# 	advance frame pointer
	addiu	$fp $sp 4
# 	self = accumulator
	move	$s0 $a0
# call parent init
	jal	IO_init
# init attributes
# - 3 population_map: String no init
# 	accumulator = self
	move	$a0 $s0
# deactivate with 0 args and 0 temps
# 	pop activation record
# 		pop to $ra
	lw	$ra 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $s0
	lw	$s0 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $fp
	lw	$fp 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 	return
	jr	$ra	
# end of initializer
Main.main:
# activate with 0 temps
# 	push activation record
# 		push $fp
	addiu	$sp $sp -4
	sw	$fp 4($sp)
# 		push finish
# 		push $s0
	addiu	$sp $sp -4
	sw	$s0 4($sp)
# 		push finish
# 		push $ra
	addiu	$sp $sp -4
	sw	$ra 4($sp)
# 		push finish
# 	advance frame pointer
	addiu	$fp $sp 4
# 	self = accumulator
	move	$s0 $a0
	# block_class::code
	# assign_class::code
# dispatch_class::code
	la	$a0 str_const5
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
	# new__class::code
	la	$a0 CellularAutomaton_protObj
	jal	Object.copy
	jal	CellularAutomaton_init
	bne	$a0 $zero label1
	la	$a0 str_const0
	li	$t1 88
	jal	_dispatch_abort
label1:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of CellularAutomaton.init
	lw	$t1 28($t1)
	jalr		$t1
	sw	$a0 12($s0)
# dispatch_class::code
# object_class::code load cells
	lw	$a0 12($s0)
	bne	$a0 $zero label2
	la	$a0 str_const0
	li	$t1 89
	jal	_dispatch_abort
label2:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of CellularAutomaton.print
	lw	$t1 32($t1)
	jalr		$t1
	# let_class::code
	la	$a0 int_const2	# load int
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
	# block_class::code
# dispatch_class::code
# object_class::code load cells
	lw	$a0 12($s0)
	bne	$a0 $zero label3
	la	$a0 str_const0
	li	$t1 93
	jal	_dispatch_abort
label3:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of CellularAutomaton.evolve
	lw	$t1 56($t1)
	jalr		$t1
# dispatch_class::code
# object_class::code load cells
	lw	$a0 12($s0)
	bne	$a0 $zero label4
	la	$a0 str_const0
	li	$t1 94
	jal	_dispatch_abort
label4:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of CellularAutomaton.print
	lw	$t1 32($t1)
	jalr		$t1
# 		pop
	addiu	$sp $sp 4
# 		pop finish
# object_class::code load self
	move	$a0 $s0
# deactivate with 0 args and 0 temps
# 	pop activation record
# 		pop to $ra
	lw	$ra 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $s0
	lw	$s0 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $fp
	lw	$fp 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 	return
	jr	$ra	
CellularAutomaton.init:
# activate with 0 temps
# 	push activation record
# 		push $fp
	addiu	$sp $sp -4
	sw	$fp 4($sp)
# 		push finish
# 		push $s0
	addiu	$sp $sp -4
	sw	$s0 4($sp)
# 		push finish
# 		push $ra
	addiu	$sp $sp -4
	sw	$ra 4($sp)
# 		push finish
# 	advance frame pointer
	addiu	$fp $sp 4
# 	self = accumulator
	move	$s0 $a0
	# block_class::code
	# assign_class::code
# object_class::code load map
	lw	$a0 12($fp)
	sw	$a0 12($s0)
# object_class::code load self
	move	$a0 $s0
# deactivate with 1 args and 0 temps
# 	pop activation record
# 		pop to $ra
	lw	$ra 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $s0
	lw	$s0 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $fp
	lw	$fp 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 	pop space for arguments
	addiu	$sp $sp 4
# 	return
	jr	$ra	
CellularAutomaton.print:
# activate with 0 temps
# 	push activation record
# 		push $fp
	addiu	$sp $sp -4
	sw	$fp 4($sp)
# 		push finish
# 		push $s0
	addiu	$sp $sp -4
	sw	$s0 4($sp)
# 		push finish
# 		push $ra
	addiu	$sp $sp -4
	sw	$ra 4($sp)
# 		push finish
# 	advance frame pointer
	addiu	$fp $sp 4
# 	self = accumulator
	move	$s0 $a0
	# block_class::code
# dispatch_class::code
# dispatch_class::code
	la	$a0 str_const1
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
# object_class::code load population_map
	lw	$a0 12($s0)
	bne	$a0 $zero label5
	la	$a0 str_const0
	li	$t1 17
	jal	_dispatch_abort
label5:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of String.concat
	lw	$t1 16($t1)
	jalr		$t1
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
# object_class::code load self
	move	$a0 $s0
	bne	$a0 $zero label6
	la	$a0 str_const0
	li	$t1 17
	jal	_dispatch_abort
label6:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of SELF_TYPE.out_string
	lw	$t1 12($t1)
	jalr		$t1
# object_class::code load self
	move	$a0 $s0
# deactivate with 0 args and 0 temps
# 	pop activation record
# 		pop to $ra
	lw	$ra 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $s0
	lw	$s0 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $fp
	lw	$fp 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 	return
	jr	$ra	
CellularAutomaton.num_cells:
# activate with 0 temps
# 	push activation record
# 		push $fp
	addiu	$sp $sp -4
	sw	$fp 4($sp)
# 		push finish
# 		push $s0
	addiu	$sp $sp -4
	sw	$s0 4($sp)
# 		push finish
# 		push $ra
	addiu	$sp $sp -4
	sw	$ra 4($sp)
# 		push finish
# 	advance frame pointer
	addiu	$fp $sp 4
# 	self = accumulator
	move	$s0 $a0
# dispatch_class::code
# object_class::code load population_map
	lw	$a0 12($s0)
	bne	$a0 $zero label7
	la	$a0 str_const0
	li	$t1 23
	jal	_dispatch_abort
label7:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of String.length
	lw	$t1 12($t1)
	jalr		$t1
# deactivate with 0 args and 0 temps
# 	pop activation record
# 		pop to $ra
	lw	$ra 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $s0
	lw	$s0 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $fp
	lw	$fp 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 	return
	jr	$ra	
CellularAutomaton.cell:
# activate with 0 temps
# 	push activation record
# 		push $fp
	addiu	$sp $sp -4
	sw	$fp 4($sp)
# 		push finish
# 		push $s0
	addiu	$sp $sp -4
	sw	$s0 4($sp)
# 		push finish
# 		push $ra
	addiu	$sp $sp -4
	sw	$ra 4($sp)
# 		push finish
# 	advance frame pointer
	addiu	$fp $sp 4
# 	self = accumulator
	move	$s0 $a0
# dispatch_class::code
# object_class::code load position
	lw	$a0 12($fp)
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
	la	$a0 int_const0	# load int
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
# object_class::code load population_map
	lw	$a0 12($s0)
	bne	$a0 $zero label8
	la	$a0 str_const0
	li	$t1 27
	jal	_dispatch_abort
label8:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of String.substr
	lw	$t1 20($t1)
	jalr		$t1
# deactivate with 1 args and 0 temps
# 	pop activation record
# 		pop to $ra
	lw	$ra 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $s0
	lw	$s0 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $fp
	lw	$fp 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 	pop space for arguments
	addiu	$sp $sp 4
# 	return
	jr	$ra	
CellularAutomaton.cell_left_neighbor:
# activate with 0 temps
# 	push activation record
# 		push $fp
	addiu	$sp $sp -4
	sw	$fp 4($sp)
# 		push finish
# 		push $s0
	addiu	$sp $sp -4
	sw	$s0 4($sp)
# 		push finish
# 		push $ra
	addiu	$sp $sp -4
	sw	$ra 4($sp)
# 		push finish
# 	advance frame pointer
	addiu	$fp $sp 4
# 	self = accumulator
	move	$s0 $a0
	# cond_class::code
	# eq_class::code (unfinished)
# object_class::code load position
	lw	$a0 12($fp)
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
	la	$a0 int_const1	# load int
# 		pop to $t1
	lw	$t1 4($sp)
	addiu	$sp $sp 4
# 		pop finish
	move	$t2 $a0
	la	$a0 bool_const1
	beq	$t1 $t2 label9
	la	$a0 bool_const0
	jal	equality_test
label9:
	lw	$t1 12($a0)
	beqz	$t1 label10
# dispatch_class::code
	# sub_class::code
# dispatch_class::code
# object_class::code load self
	move	$a0 $s0
	bne	$a0 $zero label12
	la	$a0 str_const0
	li	$t1 32
	jal	_dispatch_abort
label12:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of SELF_TYPE.num_cells
	lw	$t1 36($t1)
	jalr		$t1
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
	la	$a0 int_const0	# load int
	jal	Object.copy
# 		pop to $t1
	lw	$t1 4($sp)
	addiu	$sp $sp 4
# 		pop finish
	lw	$t1 12($t1)
	lw	$t2 12($a0)
	sub	$t1 $t1 $t2
	sw	$t1 12($a0)
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
# object_class::code load self
	move	$a0 $s0
	bne	$a0 $zero label13
	la	$a0 str_const0
	li	$t1 32
	jal	_dispatch_abort
label13:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of SELF_TYPE.cell
	lw	$t1 40($t1)
	jalr		$t1
	b	label11
label10:
# dispatch_class::code
	# sub_class::code
# object_class::code load position
	lw	$a0 12($fp)
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
	la	$a0 int_const0	# load int
	jal	Object.copy
# 		pop to $t1
	lw	$t1 4($sp)
	addiu	$sp $sp 4
# 		pop finish
	lw	$t1 12($t1)
	lw	$t2 12($a0)
	sub	$t1 $t1 $t2
	sw	$t1 12($a0)
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
# object_class::code load self
	move	$a0 $s0
	bne	$a0 $zero label14
	la	$a0 str_const0
	li	$t1 34
	jal	_dispatch_abort
label14:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of SELF_TYPE.cell
	lw	$t1 40($t1)
	jalr		$t1
label11:
# deactivate with 1 args and 0 temps
# 	pop activation record
# 		pop to $ra
	lw	$ra 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $s0
	lw	$s0 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $fp
	lw	$fp 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 	pop space for arguments
	addiu	$sp $sp 4
# 	return
	jr	$ra	
CellularAutomaton.cell_right_neighbor:
# activate with 0 temps
# 	push activation record
# 		push $fp
	addiu	$sp $sp -4
	sw	$fp 4($sp)
# 		push finish
# 		push $s0
	addiu	$sp $sp -4
	sw	$s0 4($sp)
# 		push finish
# 		push $ra
	addiu	$sp $sp -4
	sw	$ra 4($sp)
# 		push finish
# 	advance frame pointer
	addiu	$fp $sp 4
# 	self = accumulator
	move	$s0 $a0
	# cond_class::code
	# eq_class::code (unfinished)
# object_class::code load position
	lw	$a0 12($fp)
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
	# sub_class::code
# dispatch_class::code
# object_class::code load self
	move	$a0 $s0
	bne	$a0 $zero label15
	la	$a0 str_const0
	li	$t1 39
	jal	_dispatch_abort
label15:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of SELF_TYPE.num_cells
	lw	$t1 36($t1)
	jalr		$t1
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
	la	$a0 int_const0	# load int
	jal	Object.copy
# 		pop to $t1
	lw	$t1 4($sp)
	addiu	$sp $sp 4
# 		pop finish
	lw	$t1 12($t1)
	lw	$t2 12($a0)
	sub	$t1 $t1 $t2
	sw	$t1 12($a0)
# 		pop to $t1
	lw	$t1 4($sp)
	addiu	$sp $sp 4
# 		pop finish
	move	$t2 $a0
	la	$a0 bool_const1
	beq	$t1 $t2 label16
	la	$a0 bool_const0
	jal	equality_test
label16:
	lw	$t1 12($a0)
	beqz	$t1 label17
# dispatch_class::code
	la	$a0 int_const1	# load int
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
# object_class::code load self
	move	$a0 $s0
	bne	$a0 $zero label19
	la	$a0 str_const0
	li	$t1 40
	jal	_dispatch_abort
label19:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of SELF_TYPE.cell
	lw	$t1 40($t1)
	jalr		$t1
	b	label18
label17:
# dispatch_class::code
	# plus_class::code
# object_class::code load position
	lw	$a0 12($fp)
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
	la	$a0 int_const0	# load int
	jal	Object.copy
# 		pop to $t1
	lw	$t1 4($sp)
	addiu	$sp $sp 4
# 		pop finish
	lw	$t1 12($t1)
	lw	$t2 12($a0)
	add	$t1 $t1 $t2
	sw	$t1 12($a0)
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
# object_class::code load self
	move	$a0 $s0
	bne	$a0 $zero label20
	la	$a0 str_const0
	li	$t1 42
	jal	_dispatch_abort
label20:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of SELF_TYPE.cell
	lw	$t1 40($t1)
	jalr		$t1
label18:
# deactivate with 1 args and 0 temps
# 	pop activation record
# 		pop to $ra
	lw	$ra 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $s0
	lw	$s0 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $fp
	lw	$fp 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 	pop space for arguments
	addiu	$sp $sp 4
# 	return
	jr	$ra	
CellularAutomaton.cell_at_next_evolution:
# activate with 0 temps
# 	push activation record
# 		push $fp
	addiu	$sp $sp -4
	sw	$fp 4($sp)
# 		push finish
# 		push $s0
	addiu	$sp $sp -4
	sw	$s0 4($sp)
# 		push finish
# 		push $ra
	addiu	$sp $sp -4
	sw	$ra 4($sp)
# 		push finish
# 	advance frame pointer
	addiu	$fp $sp 4
# 	self = accumulator
	move	$s0 $a0
	# cond_class::code
	# eq_class::code (unfinished)
	# plus_class::code
	# plus_class::code
	# cond_class::code
	# eq_class::code (unfinished)
# dispatch_class::code
# object_class::code load position
	lw	$a0 12($fp)
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
# object_class::code load self
	move	$a0 $s0
	bne	$a0 $zero label21
	la	$a0 str_const0
	li	$t1 49
	jal	_dispatch_abort
label21:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of SELF_TYPE.cell
	lw	$t1 40($t1)
	jalr		$t1
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
	la	$a0 str_const2
# 		pop to $t1
	lw	$t1 4($sp)
	addiu	$sp $sp 4
# 		pop finish
	move	$t2 $a0
	la	$a0 bool_const1
	beq	$t1 $t2 label22
	la	$a0 bool_const0
	jal	equality_test
label22:
	lw	$t1 12($a0)
	beqz	$t1 label23
	la	$a0 int_const0	# load int
	b	label24
label23:
	la	$a0 int_const1	# load int
label24:
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
	# cond_class::code
	# eq_class::code (unfinished)
# dispatch_class::code
# object_class::code load position
	lw	$a0 12($fp)
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
# object_class::code load self
	move	$a0 $s0
	bne	$a0 $zero label25
	la	$a0 str_const0
	li	$t1 50
	jal	_dispatch_abort
label25:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of SELF_TYPE.cell_left_neighbor
	lw	$t1 44($t1)
	jalr		$t1
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
	la	$a0 str_const2
# 		pop to $t1
	lw	$t1 4($sp)
	addiu	$sp $sp 4
# 		pop finish
	move	$t2 $a0
	la	$a0 bool_const1
	beq	$t1 $t2 label26
	la	$a0 bool_const0
	jal	equality_test
label26:
	lw	$t1 12($a0)
	beqz	$t1 label27
	la	$a0 int_const0	# load int
	b	label28
label27:
	la	$a0 int_const1	# load int
label28:
	jal	Object.copy
# 		pop to $t1
	lw	$t1 4($sp)
	addiu	$sp $sp 4
# 		pop finish
	lw	$t1 12($t1)
	lw	$t2 12($a0)
	add	$t1 $t1 $t2
	sw	$t1 12($a0)
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
	# cond_class::code
	# eq_class::code (unfinished)
# dispatch_class::code
# object_class::code load position
	lw	$a0 12($fp)
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
# object_class::code load self
	move	$a0 $s0
	bne	$a0 $zero label29
	la	$a0 str_const0
	li	$t1 51
	jal	_dispatch_abort
label29:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of SELF_TYPE.cell_right_neighbor
	lw	$t1 48($t1)
	jalr		$t1
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
	la	$a0 str_const2
# 		pop to $t1
	lw	$t1 4($sp)
	addiu	$sp $sp 4
# 		pop finish
	move	$t2 $a0
	la	$a0 bool_const1
	beq	$t1 $t2 label30
	la	$a0 bool_const0
	jal	equality_test
label30:
	lw	$t1 12($a0)
	beqz	$t1 label31
	la	$a0 int_const0	# load int
	b	label32
label31:
	la	$a0 int_const1	# load int
label32:
	jal	Object.copy
# 		pop to $t1
	lw	$t1 4($sp)
	addiu	$sp $sp 4
# 		pop finish
	lw	$t1 12($t1)
	lw	$t2 12($a0)
	add	$t1 $t1 $t2
	sw	$t1 12($a0)
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
	la	$a0 int_const0	# load int
# 		pop to $t1
	lw	$t1 4($sp)
	addiu	$sp $sp 4
# 		pop finish
	move	$t2 $a0
	la	$a0 bool_const1
	beq	$t1 $t2 label33
	la	$a0 bool_const0
	jal	equality_test
label33:
	lw	$t1 12($a0)
	beqz	$t1 label34
	la	$a0 str_const2
	b	label35
label34:
	la	$a0 str_const3
label35:
# deactivate with 1 args and 0 temps
# 	pop activation record
# 		pop to $ra
	lw	$ra 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $s0
	lw	$s0 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $fp
	lw	$fp 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 	pop space for arguments
	addiu	$sp $sp 4
# 	return
	jr	$ra	
CellularAutomaton.evolve:
# activate with 0 temps
# 	push activation record
# 		push $fp
	addiu	$sp $sp -4
	sw	$fp 4($sp)
# 		push finish
# 		push $s0
	addiu	$sp $sp -4
	sw	$s0 4($sp)
# 		push finish
# 		push $ra
	addiu	$sp $sp -4
	sw	$ra 4($sp)
# 		push finish
# 	advance frame pointer
	addiu	$fp $sp 4
# 	self = accumulator
	move	$s0 $a0
	# let_class::code
	la	$a0 str_const4
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
# dispatch_class::code
# object_class::code load temp
	lw	$a0 4($fp)
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
	# new__class::code
	la	$a0 IO_protObj
	jal	Object.copy
	jal	IO_init
	bne	$a0 $zero label36
	la	$a0 str_const0
	li	$t1 65
	jal	_dispatch_abort
label36:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of IO.out_string
	lw	$t1 12($t1)
	jalr		$t1
# 		pop
	addiu	$sp $sp 4
# 		pop finish
# deactivate with 0 args and 0 temps
# 	pop activation record
# 		pop to $ra
	lw	$ra 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $s0
	lw	$s0 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 		pop to $fp
	lw	$fp 4($sp)
	addiu	$sp $sp 4
# 		pop finish
# 	return
	jr	$ra	

# end of generated code
