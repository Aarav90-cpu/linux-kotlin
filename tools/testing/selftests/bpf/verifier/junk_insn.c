{
	"junk insn",
	.insns = {
	BPF_RAW_INSN(0, 0, 0, 0, 0),
	BPF_EXIT_INSN(),
	},
	.errstr = "unknown opcode 00",
	.result = REJECT,
},
{
	"junk insn2",
	.insns = {
<<<<<<< HEAD
	BPF_RAW_INSN(BPF_LDX | BPF_MEM | BPF_W, 0, 0, 0, 1),
=======
	BPF_RAW_INSN(1, 0, 0, 0, 0),
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	BPF_EXIT_INSN(),
	},
	.errstr = "BPF_LDX uses reserved fields",
	.result = REJECT,
},
{
	"junk insn3",
	.insns = {
	BPF_RAW_INSN(-1, 0, 0, 0, 0),
	BPF_EXIT_INSN(),
	},
	.errstr = "unknown opcode ff",
	.result = REJECT,
},
{
	"junk insn4",
	.insns = {
<<<<<<< HEAD
	BPF_RAW_INSN(-1, 0, 0, -1, -1),
=======
	BPF_RAW_INSN(-1, -1, -1, -1, -1),
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	BPF_EXIT_INSN(),
	},
	.errstr = "unknown opcode ff",
	.result = REJECT,
},
{
	"junk insn5",
	.insns = {
<<<<<<< HEAD
	BPF_RAW_INSN(0x7f, 0, 0, -1, -1),
=======
	BPF_RAW_INSN(0x7f, -1, -1, -1, -1),
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	BPF_EXIT_INSN(),
	},
	.errstr = "BPF_ALU uses reserved fields",
	.result = REJECT,
},
