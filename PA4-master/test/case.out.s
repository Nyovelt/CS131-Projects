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
str_const11:
	.word	1	# tag
	.word	5	# size
	.word	String_dispTab	# dispatch table
	.word	int_const2	# string length
	.byte	0	
	.align	2
# define str_const Main
	.word	-1
str_const10:
	.word	1	# tag
	.word	6	# size
	.word	String_dispTab	# dispatch table
	.word	int_const3	# string length
	.ascii	"Main"
	.byte	0	
	.align	2
# define str_const String
	.word	-1
str_const9:
	.word	1	# tag
	.word	6	# size
	.word	String_dispTab	# dispatch table
	.word	int_const4	# string length
	.ascii	"String"
	.byte	0	
	.align	2
# define str_const Bool
	.word	-1
str_const8:
	.word	1	# tag
	.word	6	# size
	.word	String_dispTab	# dispatch table
	.word	int_const3	# string length
	.ascii	"Bool"
	.byte	0	
	.align	2
# define str_const Int
	.word	-1
str_const7:
	.word	1	# tag
	.word	5	# size
	.word	String_dispTab	# dispatch table
	.word	int_const5	# string length
	.ascii	"Int"
	.byte	0	
	.align	2
# define str_const IO
	.word	-1
str_const6:
	.word	1	# tag
	.word	5	# size
	.word	String_dispTab	# dispatch table
	.word	int_const6	# string length
	.ascii	"IO"
	.byte	0	
	.align	2
# define str_const Object
	.word	-1
str_const5:
	.word	1	# tag
	.word	6	# size
	.word	String_dispTab	# dispatch table
	.word	int_const4	# string length
	.ascii	"Object"
	.byte	0	
	.align	2
# define str_const _prim_slot
	.word	-1
str_const4:
	.word	1	# tag
	.word	7	# size
	.word	String_dispTab	# dispatch table
	.word	int_const7	# string length
	.ascii	"_prim_slot"
	.byte	0	
	.align	2
# define str_const SELF_TYPE
	.word	-1
str_const3:
	.word	1	# tag
	.word	7	# size
	.word	String_dispTab	# dispatch table
	.word	int_const8	# string length
	.ascii	"SELF_TYPE"
	.byte	0	
	.align	2
# define str_const _no_class
	.word	-1
str_const2:
	.word	1	# tag
	.word	7	# size
	.word	String_dispTab	# dispatch table
	.word	int_const8	# string length
	.ascii	"_no_class"
	.byte	0	
	.align	2
# define str_const <basic class>
	.word	-1
str_const1:
	.word	1	# tag
	.word	8	# size
	.word	String_dispTab	# dispatch table
	.word	int_const9	# string length
	.ascii	"<basic class>"
	.byte	0	
	.align	2
# define str_const /home/xa0082249956/Homeworks/CS131/PA4/test/case.cl
	.word	-1
str_const0:
	.word	1	# tag
	.word	17	# size
	.word	String_dispTab	# dispatch table
	.word	int_const10	# string length
	.ascii	"/home/xa0082249956/Homeworks/CS131/PA4/test/case.cl"
	.byte	0	
	.align	2
	.word	-1
int_const10:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	51	# integer value
	.word	-1
int_const9:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	13	# integer value
	.word	-1
int_const8:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	9	# integer value
	.word	-1
int_const7:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	10	# integer value
	.word	-1
int_const6:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	2	# integer value
	.word	-1
int_const5:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	3	# integer value
	.word	-1
int_const4:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	6	# integer value
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
	.word	0	# integer value
	.word	-1
int_const1:
	.word	3	# class tag
	.word	4	# object size
	.word	Int_dispTab	# dispatch table
	.word	20	# integer value
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
	.word	str_const5	# Object (0)
	.word	str_const9	# String (1)
	.word	str_const8	# Bool (2)
	.word	str_const7	# Int (3)
	.word	str_const6	# IO (4)
	.word	str_const10	# Main (5)
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
	.word	int_const2	# 3 String._val: Int
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
	.word	3	# 1 length
	.word	Main_dispTab	# 2 dispatch table
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
# dispatch_class::code
	la	$a0 int_const0	# load int
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
	# new__class::code
	la	$a0 IO_protObj
	jal	Object.copy
	jal	IO_init
	bne	$a0 $zero label1
	la	$a0 str_const0
	li	$t1 9
	jal	_dispatch_abort
label1:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of IO.out_int
	lw	$t1 16($t1)
	jalr		$t1
	# let_class::code
	la	$a0 int_const1	# load int
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
# dispatch_class::code
# object_class::code load countdown
	lw	$a0 -4($fp)
# 		push $a0
	addiu	$sp $sp -4
	sw	$a0 4($sp)
# 		push finish
	# new__class::code
	la	$a0 IO_protObj
	jal	Object.copy
	jal	IO_init
	bne	$a0 $zero label2
	la	$a0 str_const0
	li	$t1 19
	jal	_dispatch_abort
label2:
# 	get dispatch table 
	lw	$t1 8($a0)
# 	get method pointer of IO.out_int
	lw	$t1 16($t1)
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
