import sys
import json
registers = ['b', 'c', 'd', 'e', 'h', 'l', 'a']


f = open("output.c", 'w')

f.write("/* load instructions */")
for ro in registers:
    for rs in registers:
        f.write(f"LD_X_Y({ro}, {rs}) ")
    f.write("\n")
f.write('\n')

f.write("/* arithmetic/logic instructions */")
for rs in registers: 
    f.write(f"ADD_X({rs}) ")
f.write("\n")

for rs in registers:
    f.write(f"SUB_X({rs}) ")
f.write("\n")

for rs in registers:
    f.write(f"AND_X({rs}) ")
f.write("\n")

for rs in registers:
    f.write(f"XOR_X({rs}) ")
f.write("\n")

for rs in registers:
    f.write(f"OR_X({rs}) ")
f.write("\n")

for rs in registers:
    f.write(f"CP_X({rs}) ")
f.write("\n")

fj = open('instructions.json')
data = json.load(fj)

f.write("static opcode_t opcodes[256] = {\n     ")

for opcode in data['unprefixed']:
    if data['unprefixed'][opcode]['mnemonic'] == "ADD":
        pass
    else: 
        f.write("nop")
# for opcode in data['unprefixed']:
#     str = data['unprefixed'][opcode]["mnemonic"]
#     operands = data['unprefixed'][opcode]["operands"]
#     if len(operands) == 2:
#         if operands[0]["immediate"] == False:
#             str += f"_d{operands[0]["name"]}"
#         else:
#             str += f"_{operands[0]["name"]}"
#         
#         if operands[1]["immediate"] == False:
#             str += f"_d{operands[1]["name"]}"
#         else:
#             str += f"_{operands[1]["name"]}"
#     if len(operands) == 1:
#         if operands[0]["immediate"] == False:
#             str += f"_d{operands[0]["name"]}"
#         else: 
#             str += f"_{operands[0]["name"]}"
# 
#     if (int(opcode, base=16)+1)%8 == 0:
#         f.write(f' {str.lower()},\n      ')
#     else: 
#         f.write(f' {str.lower()}, ')

f.close()
fj.close()