Register_Symbol :=> <!>

Token_Separater :=> < >

Register_Names  :=> < (0,acc) | (1,r1) | (2,r2) | (3,r3) | (4,r4) | (5,r5) | (6,r6) | (7,r7) | (8,r8) | (9,r9) >

Pseudo_Instruction::
{
	addi reg int -> <set int , add reg !acc , mv !acc reg>
	double reg -> <set 2 , mul reg !acc , mv !acc reg>
}