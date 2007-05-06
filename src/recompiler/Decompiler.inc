#include "../CPU.inc"

#include "CPUMacros.inc"

private VECTOR(SimpleAssign) Decompile(int PC) {
	VECTOR(SimpleAssign) result = new VECTOR(SimpleAssign)();
	boolean done = false;
	int max = 100;
	int cycles = 0;
	while (!done) {
		int opcode = FETCH;
		cycles += Tables.cycles[opcode]; /*CYCLE*/
		switch (opcode) {
			case 0x00: break;

			case 0xea: ASS_M1_S(EIMM16, A); break;                  // LD   (nn), A
			case 0xfa: ASS_S_M1(A, EIMM16); break;                  // LD   A   , (nn)
			case 0xe0: ASS_M2_S(ECONST(0xff), EIMM8, A); break;     // LDH  (n) , A
			case 0xf0: ASS_S_M2(A, ECONST(0xff), EIMM8); break;     // LDH  A   , (n)
			case 0xe2: ASS_M2_S(ECONST(0xff), EREG(C), A); break;   // LDH  (C) , A
			case 0xf2: ASS_S_M2(A, ECONST(0xff), EREG(C)); break;   // LDH  A   , (C)

			case 0xc1: DO_POP2(EREG(B), EREG(C)); break;            // POP  BC
			case 0xd1: DO_POP2(EREG(D), EREG(E)); break;            // POP  DE
			case 0xe1: DO_POP2(EREG(H), EREG(L)); break;            // POP  HL
			case 0xf1: DO_POP2(EREG(A), EREG(F)); break;            // POP  AF
			case 0xc5: DO_PUSH2(EREG(B), EREG(C)); break;           // PUSH BC
			case 0xd5: DO_PUSH2(EREG(D), EREG(E)); break;           // PUSH DE
			case 0xe5: DO_PUSH2(EREG(H), EREG(L)); break;           // PUSH HL
			case 0xf5: DO_PUSH2(EREG(A), EREG(F)); break;           // PUSH AF

			case 0xcd: {                                            // CALL (nn)
				DO_PUSH2(ECONST((PC+2)>>8), ECONST((PC+2)&0xff));
				DO_JP_NN;
			}; break;
			//case 0xc9: DO_RET; break;                               // RET

			case 0xc3: DO_JP_NN; break;                             // JP   (nn)
			case 0x18: DO_JR_D; break;                              // JR   (d)
			

			CASES_LD_XX
			//CASES_ALU_OP(0xb8, 0xfe, CP)                            // CP   x            x = r, imm8, (HL)
			CASES_ALU_OP(0xa0, 0xe6, AND)                           // AND  x            x = r, imm8, (HL)
			CASES_ALU_OP(0xa8, 0xee, XOR)                           // XOR  x            x = r, imm8, (HL)
			CASES_ALU_OP(0xb0, 0xf6, OR)                            // OR   x            x = r, imm8, (HL)
			//CASES_ALU_OP(0x80, 0xC6, ADD)
			//CASES_ALU_OP(0x88, 0xCE, ADC)
			//CASES_ALU_OP(0x90, 0xD6, SUB)
			//CASES_ALU_OP(0x98, 0xDE, SBC)
			default: {
				--PC;
				cycles -= Tables.cycles[opcode]; /*CYCLE*/
				done = true;
			}
		}

		if (PC > 0x4000) done = true;
		if (cycles > max) done = true;
	}
	ASS_S_I(PC, PC);

	ASS_V_E(new ReturnVariable(), ECONST(cycles));
	return result;
}