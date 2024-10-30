/* load instructions */LD_X_Y(b, b) LD_X_Y(b, c) LD_X_Y(b, d) LD_X_Y(b, e) LD_X_Y(b, h) LD_X_Y(b, l) LD_X_Y(b, a) 
LD_X_Y(c, b) LD_X_Y(c, c) LD_X_Y(c, d) LD_X_Y(c, e) LD_X_Y(c, h) LD_X_Y(c, l) LD_X_Y(c, a) 
LD_X_Y(d, b) LD_X_Y(d, c) LD_X_Y(d, d) LD_X_Y(d, e) LD_X_Y(d, h) LD_X_Y(d, l) LD_X_Y(d, a) 
LD_X_Y(e, b) LD_X_Y(e, c) LD_X_Y(e, d) LD_X_Y(e, e) LD_X_Y(e, h) LD_X_Y(e, l) LD_X_Y(e, a) 
LD_X_Y(h, b) LD_X_Y(h, c) LD_X_Y(h, d) LD_X_Y(h, e) LD_X_Y(h, h) LD_X_Y(h, l) LD_X_Y(h, a) 
LD_X_Y(l, b) LD_X_Y(l, c) LD_X_Y(l, d) LD_X_Y(l, e) LD_X_Y(l, h) LD_X_Y(l, l) LD_X_Y(l, a) 
LD_X_Y(a, b) LD_X_Y(a, c) LD_X_Y(a, d) LD_X_Y(a, e) LD_X_Y(a, h) LD_X_Y(a, l) LD_X_Y(a, a) 

/* arithmetic/logic instructions */
ADD_X(b) ADD_X(c) ADD_X(d) ADD_X(e) ADD_X(h) ADD_X(l) ADD_X(a) 
SUB_X(b) SUB_X(c) SUB_X(d) SUB_X(e) SUB_X(h) SUB_X(l) SUB_X(a) 
AND_X(b) AND_X(c) AND_X(d) AND_X(e) AND_X(h) AND_X(l) AND_X(a) 
XOR_X(b) XOR_X(c) XOR_X(d) XOR_X(e) XOR_X(h) XOR_X(l) XOR_X(a) 
OR_X(b) OR_X(c) OR_X(d) OR_X(e) OR_X(h) OR_X(l) OR_X(a) 
CP_X(b) CP_X(c) CP_X(d) CP_X(e) CP_X(h) CP_X(l) CP_X(a) 
static opcode_t opcodes[256] = {
      nop,  ld_bc_n16,  ld_dbc_a,  inc_bc,  inc_b,  dec_b,  ld_b_n8,  nop,
      ld_da16_sp,  add_hl_bc,  ld_a_dbc,  dec_bc,  inc_c,  dec_c,  ld_c_n8,  nop,
      stop_n8,  ld_de_n16,  ld_dde_a,  inc_de,  inc_d,  dec_d,  ld_d_n8,  nop,
      nop,  add_hl_de,  ld_a_dde,  dec_de,  inc_e,  dec_e,  ld_e_n8,  nop,
      nop,  ld_hl_n16,  ld_dhl_a,  inc_hl,  inc_h,  dec_h,  ld_h_n8,  nop,
      nop,  add_hl_hl,  ld_a_dhl,  dec_hl,  inc_l,  dec_l,  ld_l_n8,  nop,
      nop,  ld_sp_n16,  ld_dhl_a,  inc_sp,  inc_dhl,  dec_dhl,  ld_dhl_n8,  nop,
      nop,  add_hl_sp,  ld_a_dhl,  dec_sp,  inc_a,  dec_a,  ld_a_n8,  nop,
      ld_b_b,  ld_b_c,  ld_b_d,  ld_b_e,  ld_b_h,  ld_b_l,  ld_b_dhl,  ld_b_a,
      ld_c_b,  ld_c_c,  ld_c_d,  ld_c_e,  ld_c_h,  ld_c_l,  ld_c_dhl,  ld_c_a,
      ld_d_b,  ld_d_c,  ld_d_d,  ld_d_e,  ld_d_h,  ld_d_l,  ld_d_dhl,  ld_d_a,
      ld_e_b,  ld_e_c,  ld_e_d,  ld_e_e,  ld_e_h,  ld_e_l,  ld_e_dhl,  ld_e_a,
      ld_h_b,  ld_h_c,  ld_h_d,  ld_h_e,  ld_h_h,  ld_h_l,  ld_h_dhl,  ld_h_a,
      ld_l_b,  ld_l_c,  ld_l_d,  ld_l_e,  ld_l_h,  ld_l_l,  ld_l_dhl,  ld_l_a,
      ld_dhl_b,  ld_dhl_c,  ld_dhl_d,  ld_dhl_e,  ld_dhl_h,  ld_dhl_l,  halt,  ld_dhl_a,
      ld_a_b,  ld_a_c,  ld_a_d,  ld_a_e,  ld_a_h,  ld_a_l,  ld_a_dhl,  ld_a_a,
      add_a_b,  add_a_c,  add_a_d,  add_a_e,  add_a_h,  add_a_l,  add_a_dhl,  add_a_a,
      adc_a_b,  adc_a_c,  adc_a_d,  adc_a_e,  adc_a_h,  adc_a_l,  adc_a_dhl,  adc_a_a,
      sub_a_b,  sub_a_c,  sub_a_d,  sub_a_e,  sub_a_h,  sub_a_l,  sub_a_dhl,  sub_a_a,
      sbc_a_b,  sbc_a_c,  sbc_a_d,  sbc_a_e,  sbc_a_h,  sbc_a_l,  sbc_a_dhl,  sbc_a_a,
      and_a_b,  and_a_c,  and_a_d,  and_a_e,  and_a_h,  and_a_l,  and_a_dhl,  and_a_a,
      xor_a_b,  xor_a_c,  xor_a_d,  xor_a_e,  xor_a_h,  xor_a_l,  xor_a_dhl,  xor_a_a,
      or_a_b,  or_a_c,  or_a_d,  or_a_e,  or_a_h,  or_a_l,  or_a_dhl,  or_a_a,
      cp_a_b,  cp_a_c,  cp_a_d,  cp_a_e,  cp_a_h,  cp_a_l,  cp_a_dhl,  cp_a_a,
      nop, nop, nop, nop, nop, nop, nop, nop,
      nop, nop, nop, nop, nop, nop, nop, nop,
      nop, nop, nop, nop, nop, nop, nop, nop,
      nop, nop, nop, nop, nop, nop, nop, nop,
      nop, nop, nop, nop, nop, nop, nop, nop,
      nop, nop, nop, nop, nop, nop, nop, nop,
      nop, nop, nop, nop, nop, nop, nop, nop,
      nop, nop, nop, nop, nop, nop, nop, nop
};