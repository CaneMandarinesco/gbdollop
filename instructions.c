/* load instructions */LD_X_Y(b, b) LD_X_Y(b, c) LD_X_Y(b, d) LD_X_Y(b, e) LD_X_Y(b, h) LD_X_Y(b, l) LD_X_Y(b, a) 
LD_X_Y(c, b) LD_X_Y(c, c) LD_X_Y(c, d) LD_X_Y(c, e) LD_X_Y(c, h) LD_X_Y(c, l) LD_X_Y(c, a) 
LD_X_Y(d, b) LD_X_Y(d, c) LD_X_Y(d, d) LD_X_Y(d, e) LD_X_Y(d, h) LD_X_Y(d, l) LD_X_Y(d, a) 
LD_X_Y(e, b) LD_X_Y(e, c) LD_X_Y(e, d) LD_X_Y(e, e) LD_X_Y(e, h) LD_X_Y(e, l) LD_X_Y(e, a) 
LD_X_Y(h, b) LD_X_Y(h, c) LD_X_Y(h, d) LD_X_Y(h, e) LD_X_Y(h, h) LD_X_Y(h, l) LD_X_Y(h, a) 
LD_X_Y(l, b) LD_X_Y(l, c) LD_X_Y(l, d) LD_X_Y(l, e) LD_X_Y(l, h) LD_X_Y(l, l) LD_X_Y(l, a) 
LD_X_Y(a, b) LD_X_Y(a, c) LD_X_Y(a, d) LD_X_Y(a, e) LD_X_Y(a, h) LD_X_Y(a, l) LD_X_Y(a, a) 

/* arithmetic/logic instructions */ADD_X(b) ADD_X(c) ADD_X(d) ADD_X(e) ADD_X(h) ADD_X(l) ADD_X(a) 
SUB_X(b) SUB_X(c) SUB_X(d) SUB_X(e) SUB_X(h) SUB_X(l) SUB_X(a) 
AND_X(b) AND_X(c) AND_X(d) AND_X(e) AND_X(h) AND_X(l) AND_X(a) 
XOR_X(b) XOR_X(c) XOR_X(d) XOR_X(e) XOR_X(h) XOR_X(l) XOR_X(a) 
OR_X(b) OR_X(c) OR_X(d) OR_X(e) OR_X(h) OR_X(l) OR_X(a) 
CP_X(b) CP_X(c) CP_X(d) CP_X(e) CP_X(h) CP_X(l) CP_X(a) 
static opcode_t opcodes[256] = {
     