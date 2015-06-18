#include "algorithms.h"
#include "disassembler.h"
#include "memory.h"
#include "table.h"
#include "opcode_misc.h"
#include "opcode_pc.h"
#include "opcode_read.h"
#include "opcode_rmw.h"
#include "opcode_write.h"

#define jump_flags()                                        \
    if(cpu->regs.e) {                                       \
        goto *jump_table_EM[r65816_op_readpc(cpu)];         \
    } else if(cpu->regs.p.m) {                              \
        if(cpu->regs.p.x) {                                 \
            goto *jump_table_MX[r65816_op_readpc(cpu)];     \
        } else {                                            \
            goto *jump_table_Mx[r65816_op_readpc(cpu)];     \
        }                                                   \
    } else {                                                \
        if(cpu->regs.p.x) {                                 \
            goto *jump_table_mX[r65816_op_readpc(cpu)];     \
        } else {                                            \
            goto *jump_table_mx[r65816_op_readpc(cpu)];     \
        }                                                   \
    }

#define opA(id, name)                               \
    EM_##id: r65816_op_##name(cpu); return;         \
    MX_##id: r65816_op_##name(cpu); return;         \
    Mx_##id: r65816_op_##name(cpu); return;         \
    mX_##id: r65816_op_##name(cpu); return;         \
    mx_##id: r65816_op_##name(cpu); return;

#define opE(id, name)                               \
    EM_##id: r65816_op_##name##_e(cpu); return;     \
    MX_##id: r65816_op_##name##_n(cpu); return;     \
    Mx_##id: r65816_op_##name##_n(cpu); return;     \
    mX_##id: r65816_op_##name##_n(cpu); return;     \
    mx_##id: r65816_op_##name##_n(cpu); return;

#define opM(id, name)                               \
    EM_##id: r65816_op_##name##_b(cpu); return;     \
    MX_##id: r65816_op_##name##_b(cpu); return;     \
    Mx_##id: r65816_op_##name##_b(cpu); return;     \
    mX_##id: r65816_op_##name##_w(cpu); return;     \
    mx_##id: r65816_op_##name##_w(cpu); return;

#define opX(id, name)                               \
    EM_##id: r65816_op_##name##_b(cpu); return;     \
    MX_##id: r65816_op_##name##_b(cpu); return;     \
    Mx_##id: r65816_op_##name##_w(cpu); return;     \
    mX_##id: r65816_op_##name##_b(cpu); return;     \
    mx_##id: r65816_op_##name##_w(cpu); return;

#define opA_flags(id, name)                         \
    EM_##id: r65816_op_##name(cpu); return;         \
    MX_##id: r65816_op_##name(cpu); return;         \
    Mx_##id: r65816_op_##name(cpu); return;         \
    mX_##id: r65816_op_##name(cpu); return;         \
    mx_##id: r65816_op_##name(cpu); return;

#define opE_flags(id, name)                         \
    EM_##id: r65816_op_##name##_e(cpu); return;     \
    MX_##id: r65816_op_##name##_n(cpu); return;     \
    Mx_##id: r65816_op_##name##_n(cpu); return;     \
    mX_##id: r65816_op_##name##_n(cpu); return;     \
    mx_##id: r65816_op_##name##_n(cpu); return;

void r65816_cpu_step(r65816_cpu_t* cpu) {
#ifdef DEBUG_65816
    char output[256];
    r65816_cpu_disassemble_opcode(cpu, output, cpu->regs.pc.d);
    printf("%s\n", output);
#endif //DEBUG_65816
    
    static const void *jump_table_EM[] = {
        &&EM_0x00, &&EM_0x01, &&EM_0x02, &&EM_0x03, &&EM_0x04, &&EM_0x05, &&EM_0x06, &&EM_0x07, &&EM_0x08, &&EM_0x09, &&EM_0x0a, &&EM_0x0b, &&EM_0x0c, &&EM_0x0d, &&EM_0x0e, &&EM_0x0f, &&EM_0x10, &&EM_0x11, &&EM_0x12, &&EM_0x13, &&EM_0x14, &&EM_0x15, &&EM_0x16, &&EM_0x17, &&EM_0x18, &&EM_0x19, &&EM_0x1a, &&EM_0x1b, &&EM_0x1c, &&EM_0x1d, &&EM_0x1e, &&EM_0x1f, &&EM_0x20, &&EM_0x21, &&EM_0x22, &&EM_0x23, &&EM_0x24, &&EM_0x25, &&EM_0x26, &&EM_0x27, &&EM_0x28, &&EM_0x29, &&EM_0x2a, &&EM_0x2b, &&EM_0x2c, &&EM_0x2d, &&EM_0x2e, &&EM_0x2f, &&EM_0x30, &&EM_0x31, &&EM_0x32, &&EM_0x33, &&EM_0x34, &&EM_0x35, &&EM_0x36, &&EM_0x37, &&EM_0x38, &&EM_0x39, &&EM_0x3a, &&EM_0x3b, &&EM_0x3c, &&EM_0x3d, &&EM_0x3e, &&EM_0x3f, &&EM_0x40, &&EM_0x41, &&EM_0x42, &&EM_0x43, &&EM_0x44, &&EM_0x45, &&EM_0x46, &&EM_0x47, &&EM_0x48, &&EM_0x49, &&EM_0x4a, &&EM_0x4b, &&EM_0x4c, &&EM_0x4d, &&EM_0x4e, &&EM_0x4f, &&EM_0x50, &&EM_0x51, &&EM_0x52, &&EM_0x53, &&EM_0x54, &&EM_0x55, &&EM_0x56, &&EM_0x57, &&EM_0x58, &&EM_0x59, &&EM_0x5a, &&EM_0x5b, &&EM_0x5c, &&EM_0x5d, &&EM_0x5e, &&EM_0x5f, &&EM_0x60, &&EM_0x61, &&EM_0x62, &&EM_0x63, &&EM_0x64, &&EM_0x65, &&EM_0x66, &&EM_0x67, &&EM_0x68, &&EM_0x69, &&EM_0x6a, &&EM_0x6b, &&EM_0x6c, &&EM_0x6d, &&EM_0x6e, &&EM_0x6f, &&EM_0x70, &&EM_0x71, &&EM_0x72, &&EM_0x73, &&EM_0x74, &&EM_0x75, &&EM_0x76, &&EM_0x77, &&EM_0x78, &&EM_0x79, &&EM_0x7a, &&EM_0x7b, &&EM_0x7c, &&EM_0x7d, &&EM_0x7e, &&EM_0x7f, &&EM_0x80, &&EM_0x81, &&EM_0x82, &&EM_0x83, &&EM_0x84, &&EM_0x85, &&EM_0x86, &&EM_0x87, &&EM_0x88, &&EM_0x89, &&EM_0x8a, &&EM_0x8b, &&EM_0x8c, &&EM_0x8d, &&EM_0x8e, &&EM_0x8f, &&EM_0x90, &&EM_0x91, &&EM_0x92, &&EM_0x93, &&EM_0x94, &&EM_0x95, &&EM_0x96, &&EM_0x97, &&EM_0x98, &&EM_0x99, &&EM_0x9a, &&EM_0x9b, &&EM_0x9c, &&EM_0x9d, &&EM_0x9e, &&EM_0x9f, &&EM_0xa0, &&EM_0xa1, &&EM_0xa2, &&EM_0xa3, &&EM_0xa4, &&EM_0xa5, &&EM_0xa6, &&EM_0xa7, &&EM_0xa8, &&EM_0xa9, &&EM_0xaa, &&EM_0xab, &&EM_0xac, &&EM_0xad, &&EM_0xae, &&EM_0xaf, &&EM_0xb0, &&EM_0xb1, &&EM_0xb2, &&EM_0xb3, &&EM_0xb4, &&EM_0xb5, &&EM_0xb6, &&EM_0xb7, &&EM_0xb8, &&EM_0xb9, &&EM_0xba, &&EM_0xbb, &&EM_0xbc, &&EM_0xbd, &&EM_0xbe, &&EM_0xbf, &&EM_0xc0, &&EM_0xc1, &&EM_0xc2, &&EM_0xc3, &&EM_0xc4, &&EM_0xc5, &&EM_0xc6, &&EM_0xc7, &&EM_0xc8, &&EM_0xc9, &&EM_0xca, &&EM_0xcb, &&EM_0xcc, &&EM_0xcd, &&EM_0xce, &&EM_0xcf, &&EM_0xd0, &&EM_0xd1, &&EM_0xd2, &&EM_0xd3, &&EM_0xd4, &&EM_0xd5, &&EM_0xd6, &&EM_0xd7, &&EM_0xd8, &&EM_0xd9, &&EM_0xda, &&EM_0xdb, &&EM_0xdc, &&EM_0xdd, &&EM_0xde, &&EM_0xdf, &&EM_0xe0, &&EM_0xe1, &&EM_0xe2, &&EM_0xe3, &&EM_0xe4, &&EM_0xe5, &&EM_0xe6, &&EM_0xe7, &&EM_0xe8, &&EM_0xe9, &&EM_0xea, &&EM_0xeb, &&EM_0xec, &&EM_0xed, &&EM_0xee, &&EM_0xef, &&EM_0xf0, &&EM_0xf1, &&EM_0xf2, &&EM_0xf3, &&EM_0xf4, &&EM_0xf5, &&EM_0xf6, &&EM_0xf7, &&EM_0xf8, &&EM_0xf9, &&EM_0xfa, &&EM_0xfb, &&EM_0xfc, &&EM_0xfd, &&EM_0xfe, &&EM_0xff
    };

    static const void *jump_table_MX[] = {
        &&MX_0x00, &&MX_0x01, &&MX_0x02, &&MX_0x03, &&MX_0x04, &&MX_0x05, &&MX_0x06, &&MX_0x07, &&MX_0x08, &&MX_0x09, &&MX_0x0a, &&MX_0x0b, &&MX_0x0c, &&MX_0x0d, &&MX_0x0e, &&MX_0x0f, &&MX_0x10, &&MX_0x11, &&MX_0x12, &&MX_0x13, &&MX_0x14, &&MX_0x15, &&MX_0x16, &&MX_0x17, &&MX_0x18, &&MX_0x19, &&MX_0x1a, &&MX_0x1b, &&MX_0x1c, &&MX_0x1d, &&MX_0x1e, &&MX_0x1f, &&MX_0x20, &&MX_0x21, &&MX_0x22, &&MX_0x23, &&MX_0x24, &&MX_0x25, &&MX_0x26, &&MX_0x27, &&MX_0x28, &&MX_0x29, &&MX_0x2a, &&MX_0x2b, &&MX_0x2c, &&MX_0x2d, &&MX_0x2e, &&MX_0x2f, &&MX_0x30, &&MX_0x31, &&MX_0x32, &&MX_0x33, &&MX_0x34, &&MX_0x35, &&MX_0x36, &&MX_0x37, &&MX_0x38, &&MX_0x39, &&MX_0x3a, &&MX_0x3b, &&MX_0x3c, &&MX_0x3d, &&MX_0x3e, &&MX_0x3f, &&MX_0x40, &&MX_0x41, &&MX_0x42, &&MX_0x43, &&MX_0x44, &&MX_0x45, &&MX_0x46, &&MX_0x47, &&MX_0x48, &&MX_0x49, &&MX_0x4a, &&MX_0x4b, &&MX_0x4c, &&MX_0x4d, &&MX_0x4e, &&MX_0x4f, &&MX_0x50, &&MX_0x51, &&MX_0x52, &&MX_0x53, &&MX_0x54, &&MX_0x55, &&MX_0x56, &&MX_0x57, &&MX_0x58, &&MX_0x59, &&MX_0x5a, &&MX_0x5b, &&MX_0x5c, &&MX_0x5d, &&MX_0x5e, &&MX_0x5f, &&MX_0x60, &&MX_0x61, &&MX_0x62, &&MX_0x63, &&MX_0x64, &&MX_0x65, &&MX_0x66, &&MX_0x67, &&MX_0x68, &&MX_0x69, &&MX_0x6a, &&MX_0x6b, &&MX_0x6c, &&MX_0x6d, &&MX_0x6e, &&MX_0x6f, &&MX_0x70, &&MX_0x71, &&MX_0x72, &&MX_0x73, &&MX_0x74, &&MX_0x75, &&MX_0x76, &&MX_0x77, &&MX_0x78, &&MX_0x79, &&MX_0x7a, &&MX_0x7b, &&MX_0x7c, &&MX_0x7d, &&MX_0x7e, &&MX_0x7f, &&MX_0x80, &&MX_0x81, &&MX_0x82, &&MX_0x83, &&MX_0x84, &&MX_0x85, &&MX_0x86, &&MX_0x87, &&MX_0x88, &&MX_0x89, &&MX_0x8a, &&MX_0x8b, &&MX_0x8c, &&MX_0x8d, &&MX_0x8e, &&MX_0x8f, &&MX_0x90, &&MX_0x91, &&MX_0x92, &&MX_0x93, &&MX_0x94, &&MX_0x95, &&MX_0x96, &&MX_0x97, &&MX_0x98, &&MX_0x99, &&MX_0x9a, &&MX_0x9b, &&MX_0x9c, &&MX_0x9d, &&MX_0x9e, &&MX_0x9f, &&MX_0xa0, &&MX_0xa1, &&MX_0xa2, &&MX_0xa3, &&MX_0xa4, &&MX_0xa5, &&MX_0xa6, &&MX_0xa7, &&MX_0xa8, &&MX_0xa9, &&MX_0xaa, &&MX_0xab, &&MX_0xac, &&MX_0xad, &&MX_0xae, &&MX_0xaf, &&MX_0xb0, &&MX_0xb1, &&MX_0xb2, &&MX_0xb3, &&MX_0xb4, &&MX_0xb5, &&MX_0xb6, &&MX_0xb7, &&MX_0xb8, &&MX_0xb9, &&MX_0xba, &&MX_0xbb, &&MX_0xbc, &&MX_0xbd, &&MX_0xbe, &&MX_0xbf, &&MX_0xc0, &&MX_0xc1, &&MX_0xc2, &&MX_0xc3, &&MX_0xc4, &&MX_0xc5, &&MX_0xc6, &&MX_0xc7, &&MX_0xc8, &&MX_0xc9, &&MX_0xca, &&MX_0xcb, &&MX_0xcc, &&MX_0xcd, &&MX_0xce, &&MX_0xcf, &&MX_0xd0, &&MX_0xd1, &&MX_0xd2, &&MX_0xd3, &&MX_0xd4, &&MX_0xd5, &&MX_0xd6, &&MX_0xd7, &&MX_0xd8, &&MX_0xd9, &&MX_0xda, &&MX_0xdb, &&MX_0xdc, &&MX_0xdd, &&MX_0xde, &&MX_0xdf, &&MX_0xe0, &&MX_0xe1, &&MX_0xe2, &&MX_0xe3, &&MX_0xe4, &&MX_0xe5, &&MX_0xe6, &&MX_0xe7, &&MX_0xe8, &&MX_0xe9, &&MX_0xea, &&MX_0xeb, &&MX_0xec, &&MX_0xed, &&MX_0xee, &&MX_0xef, &&MX_0xf0, &&MX_0xf1, &&MX_0xf2, &&MX_0xf3, &&MX_0xf4, &&MX_0xf5, &&MX_0xf6, &&MX_0xf7, &&MX_0xf8, &&MX_0xf9, &&MX_0xfa, &&MX_0xfb, &&MX_0xfc, &&MX_0xfd, &&MX_0xfe, &&MX_0xff
    };

    static const void *jump_table_Mx[] = {
        &&Mx_0x00, &&Mx_0x01, &&Mx_0x02, &&Mx_0x03, &&Mx_0x04, &&Mx_0x05, &&Mx_0x06, &&Mx_0x07, &&Mx_0x08, &&Mx_0x09, &&Mx_0x0a, &&Mx_0x0b, &&Mx_0x0c, &&Mx_0x0d, &&Mx_0x0e, &&Mx_0x0f, &&Mx_0x10, &&Mx_0x11, &&Mx_0x12, &&Mx_0x13, &&Mx_0x14, &&Mx_0x15, &&Mx_0x16, &&Mx_0x17, &&Mx_0x18, &&Mx_0x19, &&Mx_0x1a, &&Mx_0x1b, &&Mx_0x1c, &&Mx_0x1d, &&Mx_0x1e, &&Mx_0x1f, &&Mx_0x20, &&Mx_0x21, &&Mx_0x22, &&Mx_0x23, &&Mx_0x24, &&Mx_0x25, &&Mx_0x26, &&Mx_0x27, &&Mx_0x28, &&Mx_0x29, &&Mx_0x2a, &&Mx_0x2b, &&Mx_0x2c, &&Mx_0x2d, &&Mx_0x2e, &&Mx_0x2f, &&Mx_0x30, &&Mx_0x31, &&Mx_0x32, &&Mx_0x33, &&Mx_0x34, &&Mx_0x35, &&Mx_0x36, &&Mx_0x37, &&Mx_0x38, &&Mx_0x39, &&Mx_0x3a, &&Mx_0x3b, &&Mx_0x3c, &&Mx_0x3d, &&Mx_0x3e, &&Mx_0x3f, &&Mx_0x40, &&Mx_0x41, &&Mx_0x42, &&Mx_0x43, &&Mx_0x44, &&Mx_0x45, &&Mx_0x46, &&Mx_0x47, &&Mx_0x48, &&Mx_0x49, &&Mx_0x4a, &&Mx_0x4b, &&Mx_0x4c, &&Mx_0x4d, &&Mx_0x4e, &&Mx_0x4f, &&Mx_0x50, &&Mx_0x51, &&Mx_0x52, &&Mx_0x53, &&Mx_0x54, &&Mx_0x55, &&Mx_0x56, &&Mx_0x57, &&Mx_0x58, &&Mx_0x59, &&Mx_0x5a, &&Mx_0x5b, &&Mx_0x5c, &&Mx_0x5d, &&Mx_0x5e, &&Mx_0x5f, &&Mx_0x60, &&Mx_0x61, &&Mx_0x62, &&Mx_0x63, &&Mx_0x64, &&Mx_0x65, &&Mx_0x66, &&Mx_0x67, &&Mx_0x68, &&Mx_0x69, &&Mx_0x6a, &&Mx_0x6b, &&Mx_0x6c, &&Mx_0x6d, &&Mx_0x6e, &&Mx_0x6f, &&Mx_0x70, &&Mx_0x71, &&Mx_0x72, &&Mx_0x73, &&Mx_0x74, &&Mx_0x75, &&Mx_0x76, &&Mx_0x77, &&Mx_0x78, &&Mx_0x79, &&Mx_0x7a, &&Mx_0x7b, &&Mx_0x7c, &&Mx_0x7d, &&Mx_0x7e, &&Mx_0x7f, &&Mx_0x80, &&Mx_0x81, &&Mx_0x82, &&Mx_0x83, &&Mx_0x84, &&Mx_0x85, &&Mx_0x86, &&Mx_0x87, &&Mx_0x88, &&Mx_0x89, &&Mx_0x8a, &&Mx_0x8b, &&Mx_0x8c, &&Mx_0x8d, &&Mx_0x8e, &&Mx_0x8f, &&Mx_0x90, &&Mx_0x91, &&Mx_0x92, &&Mx_0x93, &&Mx_0x94, &&Mx_0x95, &&Mx_0x96, &&Mx_0x97, &&Mx_0x98, &&Mx_0x99, &&Mx_0x9a, &&Mx_0x9b, &&Mx_0x9c, &&Mx_0x9d, &&Mx_0x9e, &&Mx_0x9f, &&Mx_0xa0, &&Mx_0xa1, &&Mx_0xa2, &&Mx_0xa3, &&Mx_0xa4, &&Mx_0xa5, &&Mx_0xa6, &&Mx_0xa7, &&Mx_0xa8, &&Mx_0xa9, &&Mx_0xaa, &&Mx_0xab, &&Mx_0xac, &&Mx_0xad, &&Mx_0xae, &&Mx_0xaf, &&Mx_0xb0, &&Mx_0xb1, &&Mx_0xb2, &&Mx_0xb3, &&Mx_0xb4, &&Mx_0xb5, &&Mx_0xb6, &&Mx_0xb7, &&Mx_0xb8, &&Mx_0xb9, &&Mx_0xba, &&Mx_0xbb, &&Mx_0xbc, &&Mx_0xbd, &&Mx_0xbe, &&Mx_0xbf, &&Mx_0xc0, &&Mx_0xc1, &&Mx_0xc2, &&Mx_0xc3, &&Mx_0xc4, &&Mx_0xc5, &&Mx_0xc6, &&Mx_0xc7, &&Mx_0xc8, &&Mx_0xc9, &&Mx_0xca, &&Mx_0xcb, &&Mx_0xcc, &&Mx_0xcd, &&Mx_0xce, &&Mx_0xcf, &&Mx_0xd0, &&Mx_0xd1, &&Mx_0xd2, &&Mx_0xd3, &&Mx_0xd4, &&Mx_0xd5, &&Mx_0xd6, &&Mx_0xd7, &&Mx_0xd8, &&Mx_0xd9, &&Mx_0xda, &&Mx_0xdb, &&Mx_0xdc, &&Mx_0xdd, &&Mx_0xde, &&Mx_0xdf, &&Mx_0xe0, &&Mx_0xe1, &&Mx_0xe2, &&Mx_0xe3, &&Mx_0xe4, &&Mx_0xe5, &&Mx_0xe6, &&Mx_0xe7, &&Mx_0xe8, &&Mx_0xe9, &&Mx_0xea, &&Mx_0xeb, &&Mx_0xec, &&Mx_0xed, &&Mx_0xee, &&Mx_0xef, &&Mx_0xf0, &&Mx_0xf1, &&Mx_0xf2, &&Mx_0xf3, &&Mx_0xf4, &&Mx_0xf5, &&Mx_0xf6, &&Mx_0xf7, &&Mx_0xf8, &&Mx_0xf9, &&Mx_0xfa, &&Mx_0xfb, &&Mx_0xfc, &&Mx_0xfd, &&Mx_0xfe, &&Mx_0xff
    };

    static const void *jump_table_mX[] = {
        &&mX_0x00, &&mX_0x01, &&mX_0x02, &&mX_0x03, &&mX_0x04, &&mX_0x05, &&mX_0x06, &&mX_0x07, &&mX_0x08, &&mX_0x09, &&mX_0x0a, &&mX_0x0b, &&mX_0x0c, &&mX_0x0d, &&mX_0x0e, &&mX_0x0f, &&mX_0x10, &&mX_0x11, &&mX_0x12, &&mX_0x13, &&mX_0x14, &&mX_0x15, &&mX_0x16, &&mX_0x17, &&mX_0x18, &&mX_0x19, &&mX_0x1a, &&mX_0x1b, &&mX_0x1c, &&mX_0x1d, &&mX_0x1e, &&mX_0x1f, &&mX_0x20, &&mX_0x21, &&mX_0x22, &&mX_0x23, &&mX_0x24, &&mX_0x25, &&mX_0x26, &&mX_0x27, &&mX_0x28, &&mX_0x29, &&mX_0x2a, &&mX_0x2b, &&mX_0x2c, &&mX_0x2d, &&mX_0x2e, &&mX_0x2f, &&mX_0x30, &&mX_0x31, &&mX_0x32, &&mX_0x33, &&mX_0x34, &&mX_0x35, &&mX_0x36, &&mX_0x37, &&mX_0x38, &&mX_0x39, &&mX_0x3a, &&mX_0x3b, &&mX_0x3c, &&mX_0x3d, &&mX_0x3e, &&mX_0x3f, &&mX_0x40, &&mX_0x41, &&mX_0x42, &&mX_0x43, &&mX_0x44, &&mX_0x45, &&mX_0x46, &&mX_0x47, &&mX_0x48, &&mX_0x49, &&mX_0x4a, &&mX_0x4b, &&mX_0x4c, &&mX_0x4d, &&mX_0x4e, &&mX_0x4f, &&mX_0x50, &&mX_0x51, &&mX_0x52, &&mX_0x53, &&mX_0x54, &&mX_0x55, &&mX_0x56, &&mX_0x57, &&mX_0x58, &&mX_0x59, &&mX_0x5a, &&mX_0x5b, &&mX_0x5c, &&mX_0x5d, &&mX_0x5e, &&mX_0x5f, &&mX_0x60, &&mX_0x61, &&mX_0x62, &&mX_0x63, &&mX_0x64, &&mX_0x65, &&mX_0x66, &&mX_0x67, &&mX_0x68, &&mX_0x69, &&mX_0x6a, &&mX_0x6b, &&mX_0x6c, &&mX_0x6d, &&mX_0x6e, &&mX_0x6f, &&mX_0x70, &&mX_0x71, &&mX_0x72, &&mX_0x73, &&mX_0x74, &&mX_0x75, &&mX_0x76, &&mX_0x77, &&mX_0x78, &&mX_0x79, &&mX_0x7a, &&mX_0x7b, &&mX_0x7c, &&mX_0x7d, &&mX_0x7e, &&mX_0x7f, &&mX_0x80, &&mX_0x81, &&mX_0x82, &&mX_0x83, &&mX_0x84, &&mX_0x85, &&mX_0x86, &&mX_0x87, &&mX_0x88, &&mX_0x89, &&mX_0x8a, &&mX_0x8b, &&mX_0x8c, &&mX_0x8d, &&mX_0x8e, &&mX_0x8f, &&mX_0x90, &&mX_0x91, &&mX_0x92, &&mX_0x93, &&mX_0x94, &&mX_0x95, &&mX_0x96, &&mX_0x97, &&mX_0x98, &&mX_0x99, &&mX_0x9a, &&mX_0x9b, &&mX_0x9c, &&mX_0x9d, &&mX_0x9e, &&mX_0x9f, &&mX_0xa0, &&mX_0xa1, &&mX_0xa2, &&mX_0xa3, &&mX_0xa4, &&mX_0xa5, &&mX_0xa6, &&mX_0xa7, &&mX_0xa8, &&mX_0xa9, &&mX_0xaa, &&mX_0xab, &&mX_0xac, &&mX_0xad, &&mX_0xae, &&mX_0xaf, &&mX_0xb0, &&mX_0xb1, &&mX_0xb2, &&mX_0xb3, &&mX_0xb4, &&mX_0xb5, &&mX_0xb6, &&mX_0xb7, &&mX_0xb8, &&mX_0xb9, &&mX_0xba, &&mX_0xbb, &&mX_0xbc, &&mX_0xbd, &&mX_0xbe, &&mX_0xbf, &&mX_0xc0, &&mX_0xc1, &&mX_0xc2, &&mX_0xc3, &&mX_0xc4, &&mX_0xc5, &&mX_0xc6, &&mX_0xc7, &&mX_0xc8, &&mX_0xc9, &&mX_0xca, &&mX_0xcb, &&mX_0xcc, &&mX_0xcd, &&mX_0xce, &&mX_0xcf, &&mX_0xd0, &&mX_0xd1, &&mX_0xd2, &&mX_0xd3, &&mX_0xd4, &&mX_0xd5, &&mX_0xd6, &&mX_0xd7, &&mX_0xd8, &&mX_0xd9, &&mX_0xda, &&mX_0xdb, &&mX_0xdc, &&mX_0xdd, &&mX_0xde, &&mX_0xdf, &&mX_0xe0, &&mX_0xe1, &&mX_0xe2, &&mX_0xe3, &&mX_0xe4, &&mX_0xe5, &&mX_0xe6, &&mX_0xe7, &&mX_0xe8, &&mX_0xe9, &&mX_0xea, &&mX_0xeb, &&mX_0xec, &&mX_0xed, &&mX_0xee, &&mX_0xef, &&mX_0xf0, &&mX_0xf1, &&mX_0xf2, &&mX_0xf3, &&mX_0xf4, &&mX_0xf5, &&mX_0xf6, &&mX_0xf7, &&mX_0xf8, &&mX_0xf9, &&mX_0xfa, &&mX_0xfb, &&mX_0xfc, &&mX_0xfd, &&mX_0xfe, &&mX_0xff
    };
    static const void *jump_table_mx[] = {
        &&mx_0x00, &&mx_0x01, &&mx_0x02, &&mx_0x03, &&mx_0x04, &&mx_0x05, &&mx_0x06, &&mx_0x07, &&mx_0x08, &&mx_0x09, &&mx_0x0a, &&mx_0x0b, &&mx_0x0c, &&mx_0x0d, &&mx_0x0e, &&mx_0x0f, &&mx_0x10, &&mx_0x11, &&mx_0x12, &&mx_0x13, &&mx_0x14, &&mx_0x15, &&mx_0x16, &&mx_0x17, &&mx_0x18, &&mx_0x19, &&mx_0x1a, &&mx_0x1b, &&mx_0x1c, &&mx_0x1d, &&mx_0x1e, &&mx_0x1f, &&mx_0x20, &&mx_0x21, &&mx_0x22, &&mx_0x23, &&mx_0x24, &&mx_0x25, &&mx_0x26, &&mx_0x27, &&mx_0x28, &&mx_0x29, &&mx_0x2a, &&mx_0x2b, &&mx_0x2c, &&mx_0x2d, &&mx_0x2e, &&mx_0x2f, &&mx_0x30, &&mx_0x31, &&mx_0x32, &&mx_0x33, &&mx_0x34, &&mx_0x35, &&mx_0x36, &&mx_0x37, &&mx_0x38, &&mx_0x39, &&mx_0x3a, &&mx_0x3b, &&mx_0x3c, &&mx_0x3d, &&mx_0x3e, &&mx_0x3f, &&mx_0x40, &&mx_0x41, &&mx_0x42, &&mx_0x43, &&mx_0x44, &&mx_0x45, &&mx_0x46, &&mx_0x47, &&mx_0x48, &&mx_0x49, &&mx_0x4a, &&mx_0x4b, &&mx_0x4c, &&mx_0x4d, &&mx_0x4e, &&mx_0x4f, &&mx_0x50, &&mx_0x51, &&mx_0x52, &&mx_0x53, &&mx_0x54, &&mx_0x55, &&mx_0x56, &&mx_0x57, &&mx_0x58, &&mx_0x59, &&mx_0x5a, &&mx_0x5b, &&mx_0x5c, &&mx_0x5d, &&mx_0x5e, &&mx_0x5f, &&mx_0x60, &&mx_0x61, &&mx_0x62, &&mx_0x63, &&mx_0x64, &&mx_0x65, &&mx_0x66, &&mx_0x67, &&mx_0x68, &&mx_0x69, &&mx_0x6a, &&mx_0x6b, &&mx_0x6c, &&mx_0x6d, &&mx_0x6e, &&mx_0x6f, &&mx_0x70, &&mx_0x71, &&mx_0x72, &&mx_0x73, &&mx_0x74, &&mx_0x75, &&mx_0x76, &&mx_0x77, &&mx_0x78, &&mx_0x79, &&mx_0x7a, &&mx_0x7b, &&mx_0x7c, &&mx_0x7d, &&mx_0x7e, &&mx_0x7f, &&mx_0x80, &&mx_0x81, &&mx_0x82, &&mx_0x83, &&mx_0x84, &&mx_0x85, &&mx_0x86, &&mx_0x87, &&mx_0x88, &&mx_0x89, &&mx_0x8a, &&mx_0x8b, &&mx_0x8c, &&mx_0x8d, &&mx_0x8e, &&mx_0x8f, &&mx_0x90, &&mx_0x91, &&mx_0x92, &&mx_0x93, &&mx_0x94, &&mx_0x95, &&mx_0x96, &&mx_0x97, &&mx_0x98, &&mx_0x99, &&mx_0x9a, &&mx_0x9b, &&mx_0x9c, &&mx_0x9d, &&mx_0x9e, &&mx_0x9f, &&mx_0xa0, &&mx_0xa1, &&mx_0xa2, &&mx_0xa3, &&mx_0xa4, &&mx_0xa5, &&mx_0xa6, &&mx_0xa7, &&mx_0xa8, &&mx_0xa9, &&mx_0xaa, &&mx_0xab, &&mx_0xac, &&mx_0xad, &&mx_0xae, &&mx_0xaf, &&mx_0xb0, &&mx_0xb1, &&mx_0xb2, &&mx_0xb3, &&mx_0xb4, &&mx_0xb5, &&mx_0xb6, &&mx_0xb7, &&mx_0xb8, &&mx_0xb9, &&mx_0xba, &&mx_0xbb, &&mx_0xbc, &&mx_0xbd, &&mx_0xbe, &&mx_0xbf, &&mx_0xc0, &&mx_0xc1, &&mx_0xc2, &&mx_0xc3, &&mx_0xc4, &&mx_0xc5, &&mx_0xc6, &&mx_0xc7, &&mx_0xc8, &&mx_0xc9, &&mx_0xca, &&mx_0xcb, &&mx_0xcc, &&mx_0xcd, &&mx_0xce, &&mx_0xcf, &&mx_0xd0, &&mx_0xd1, &&mx_0xd2, &&mx_0xd3, &&mx_0xd4, &&mx_0xd5, &&mx_0xd6, &&mx_0xd7, &&mx_0xd8, &&mx_0xd9, &&mx_0xda, &&mx_0xdb, &&mx_0xdc, &&mx_0xdd, &&mx_0xde, &&mx_0xdf, &&mx_0xe0, &&mx_0xe1, &&mx_0xe2, &&mx_0xe3, &&mx_0xe4, &&mx_0xe5, &&mx_0xe6, &&mx_0xe7, &&mx_0xe8, &&mx_0xe9, &&mx_0xea, &&mx_0xeb, &&mx_0xec, &&mx_0xed, &&mx_0xee, &&mx_0xef, &&mx_0xf0, &&mx_0xf1, &&mx_0xf2, &&mx_0xf3, &&mx_0xf4, &&mx_0xf5, &&mx_0xf6, &&mx_0xf7, &&mx_0xf8, &&mx_0xf9, &&mx_0xfa, &&mx_0xfb, &&mx_0xfc, &&mx_0xfd, &&mx_0xfe, &&mx_0xff
    };


    jump_flags();

    opE(0x00, interrupt_brk);
    opM(0x01, read_idpx_ora);
    opE(0x02, interrupt_cop);
    opM(0x03, read_sr_ora);
    opM(0x04, adjust_dp_tsb);
    opM(0x05, read_dp_ora);
    opM(0x06, adjust_dp_asl);
    opM(0x07, read_ildp_ora);
    opA(0x08, php);
    opM(0x09, read_const_ora);
    opM(0x0a, asl_imm);
    opE(0x0b, phd);
    opM(0x0c, adjust_addr_tsb);
    opM(0x0d, read_addr_ora);
    opM(0x0e, adjust_addr_asl);
    opM(0x0f, read_long_ora);
    opA(0x10, branch_bpl);
    opM(0x11, read_idpy_ora);
    opM(0x12, read_idp_ora);
    opM(0x13, read_isry_ora);
    opM(0x14, adjust_dp_trb);
    opM(0x15, read_dpr_ora);
    opM(0x16, adjust_dpx_asl);
    opM(0x17, read_ildpy_ora);
    opA(0x18, flag_clc);
    opM(0x19, read_addry_ora);
    opM(0x1a, adjust_imm_inca);
    opE(0x1b, tcs);
    opM(0x1c, adjust_addr_trb);
    opM(0x1d, read_addrx_ora);
    opM(0x1e, adjust_addrx_asl);
    opM(0x1f, read_longx_ora);
    opA(0x20, jsr_addr);
    opM(0x21, read_idpx_and);
    opE(0x22, jsr_long);
    opM(0x23, read_sr_and);
    opM(0x24, read_dp_bit);
    opM(0x25, read_dp_and);
    opM(0x26, adjust_dp_rol);
    opM(0x27, read_ildpy_and);
    opE_flags(0x28, plp);
    opM(0x29, read_const_and);
    opM(0x2a, rol_imm);
    opE(0x2b, pld);
    opM(0x2c, read_addr_bit);
    opM(0x2d, read_addr_and);
    opM(0x2e, adjust_addr_rol);
    opM(0x2f, read_long_and);
    opA(0x30, branch_bmi);
    opM(0x31, read_idpy_and);
    opM(0x32, read_idp_and);
    opM(0x33, read_isry_and);
    opM(0x34, read_dpr_bit);
    opM(0x35, read_dpr_and);
    opM(0x36, adjust_dpx_rol);
    opM(0x37, read_ildpy_and);
    opA(0x38, flag_sec);
    opM(0x39, read_addry_and);
    opM(0x3a, adjust_imm_deca);
    opA(0x3b, transfer_tsc_w);
    opM(0x3c, read_addrx_bit);
    opM(0x3d, read_addrx_and);
    opM(0x3e, adjust_addrx_rol);
    opM(0x3f, read_longx_and);
    opE_flags(0x40, rti);
    opM(0x41, read_idpx_eor);
    opA(0x42, wdm);
    opM(0x43, read_sr_eor);
    opX(0x44, move_mvp);
    opM(0x45, read_dp_eor);
    opM(0x46, adjust_dp_lsr);
    opM(0x47, read_ildpy_eor);
    opM(0x48, push_pha);
    opM(0x49, read_const_eor);
    opM(0x4a, lsr_imm);
    opA(0x4b, phk);
    opA(0x4c, jmp_addr);
    opM(0x4d, read_addr_eor);
    opM(0x4e, adjust_addr_lsr);
    opM(0x4f, read_long_eor);
    opA(0x50, branch_bvc);
    opM(0x51, read_idpy_eor);
    opM(0x52, read_idp_eor);
    opM(0x53, read_isry_eor);
    opX(0x54, move_mvn);
    opM(0x55, read_dpr_eor);
    opM(0x56, adjust_dpx_lsr);
    opM(0x57, read_ildpy_eor);
    opA(0x58, flag_cli);
    opM(0x59, read_addry_eor);
    opX(0x5a, push_phy);
    opA(0x5b, transfer_tcd_w);
    opA(0x5c, jmp_long);
    opM(0x5d, read_addrx_eor);
    opM(0x5e, adjust_addrx_lsr);
    opM(0x5f, read_longx_eor);
    opA(0x60, rts);
    opM(0x61, read_idpx_adc);
    opE(0x62, per);
    opM(0x63, read_sr_adc);
    opM(0x64, write_dp_stz);
    opM(0x65, read_dp_adc);
    opM(0x66, adjust_dp_ror);
    opM(0x67, read_ildpy_adc);
    opM(0x68, pull_pla);
    opM(0x69, read_const_adc);
    opM(0x6a, ror_imm);
    opE(0x6b, rtl);
    opA(0x6c, jmp_iaddr);
    opM(0x6d, read_addr_adc);
    opM(0x6e, adjust_addr_ror);
    opM(0x6f, read_long_adc);
    opA(0x70, branch_bvs);
    opM(0x71, read_idpy_adc);
    opM(0x72, read_idp_adc);
    opM(0x73, read_isry_adc);
    opM(0x74, write_dpr_stz);
    opM(0x75, read_dpr_adc);
    opM(0x76, adjust_dpx_ror);
    opM(0x77, read_ildpy_adc);
    opA(0x78, flag_sli);
    opM(0x79, read_addry_adc);
    opX(0x7a, pull_ply);
    opA(0x7b, transfer_tdc_w);
    opA(0x7c, jmp_iaddrx);
    opM(0x7d, read_addrx_adc);
    opM(0x7e, adjust_addrx_ror);
    opM(0x7f, read_longx_adc);
    opA(0x80, bra);
    opM(0x81, sta_idpx);
    opA(0x82, brl);
    opM(0x83, sta_sr);
    opX(0x84, write_dp_sty);
    opM(0x85, write_dp_sta);
    opX(0x86, write_dp_stx);
    opM(0x87, sta_ildp);
    opX(0x88, adjust_imm_decy);
    opM(0x89, read_bit_const);
    opM(0x8a, transfer_txa);
    opA(0x8b, phb);
    opX(0x8c, write_addr_sty);
    opM(0x8d, write_addr_sta);
    opX(0x8e, write_addr_stx);
    opM(0x8f, write_longr_sta);
    opA(0x90, branch_bcc);
    opM(0x91, sta_idpy);
    opM(0x92, sta_idp);
    opM(0x93, sta_isry);
    opX(0x94, write_dpr_sty);
    opM(0x95, write_dpr_sta);
    opX(0x96, write_dpr_stx);
    opM(0x97, sta_ildpy);
    opM(0x98, transfer_tya);
    opM(0x99, write_addrr_stay);
    opE(0x9a, txs);
    opX(0x9b, transfer_txy);
    opM(0x9c, write_addr_stz);
    opM(0x9d, write_addrr_stax);
    opM(0x9e, write_addrr_stz);
    opM(0x9f, write_longr_stax);
    opX(0xa0, read_const_ldy);
    opM(0xa1, read_idpx_lda);
    opX(0xa2, read_const_ldx);
    opM(0xa3, read_sr_lda);
    opX(0xa4, read_dp_ldy);
    opM(0xa5, read_dp_lda);
    opX(0xa6, read_dp_ldx);
    opM(0xa7, read_ildpy_lda);
    opX(0xa8, transfer_tay);
    opM(0xa9, read_const_lda);
    opX(0xaa, transfer_tax);
    opA(0xab, plb);
    opX(0xac, read_addr_ldy);
    opM(0xad, read_addr_lda);
    opX(0xae, read_addr_ldx);
    opM(0xaf, read_long_lda);
    opA(0xb0, branch_bcs);
    opM(0xb1, read_idpy_lda);
    opM(0xb2, read_idp_lda);
    opM(0xb3, read_isry_lda);
    opX(0xb4, read_dpr_ldy);
    opM(0xb5, read_dpr_lda);
    opX(0xb6, read_dpr_ldx);
    opM(0xb7, read_ildpy_lda);
    opA(0xb8, flag_clv);
    opM(0xb9, read_addry_lda);
    opX(0xba, tsx);
    opX(0xbb, transfer_tyx);
    opX(0xbc, read_addrx_ldy);
    opM(0xbd, read_addrx_lda);
    opX(0xbe, read_addry_ldx);
    opM(0xbf, read_longx_lda);
    opX(0xc0, read_const_cpy);
    opM(0xc1, read_idpx_cmp);
    opE(0xc2, pflag_rep);
    opM(0xc3, read_sr_cmp);
    opX(0xc4, read_dp_cpy);
    opM(0xc5, read_dp_cmp);
    opM(0xc6, adjust_dp_dec);
    opM(0xc7, read_ildpy_cmp);
    opX(0xc8, adjust_imm_incy);
    opM(0xc9, read_const_cmp);
    opX(0xca, adjust_imm_decx);
    opA(0xcb, wai);
    opX(0xcc, read_addr_cpy);
    opM(0xcd, read_addr_cmp);
    opM(0xce, adjust_addr_dec);
    opM(0xcf, read_long_cmp);
    opA(0xd0, branch_bne);
    opM(0xd1, read_idpy_cmp);
    opM(0xd2, read_idp_cmp);
    opM(0xd3, read_isry_cmp);
    opE(0xd4, pei);
    opM(0xd5, read_dpr_cmp);
    opM(0xd6, adjust_dpx_dec);
    opM(0xd7, read_ildpy_cmp);
    opA(0xd8, flag_cld);
    opM(0xd9, read_addry_cmp);
    opX(0xda, push_phx);
    opA(0xdb, stp);
    opA(0xdc, jmp_iladdr);
    opM(0xdd, read_addrx_cmp);
    opM(0xde, adjust_addrx_dec);
    opM(0xdf, read_longx_cmp);
    opX(0xe0, read_const_cpx);
    opM(0xe1, read_idpx_sbc);
    opE(0xe2, pflag_sep);
    opM(0xe3, read_sr_sbc);
    opX(0xe4, read_dp_cpx);
    opM(0xe5, read_dp_sbc);
    opM(0xe6, adjust_dp_inc);
    opM(0xe7, read_ildpy_sbc);
    opX(0xe8, adjust_imm_incx);
    opM(0xe9, read_const_sbc);
    opA(0xea, nop);
    opA(0xeb, xba);
    opX(0xec, read_addr_cpx);
    opM(0xed, read_addr_sbc);
    opM(0xee, adjust_addr_inc);
    opM(0xef, read_long_sbc);
    opA(0xf0, branch_beq);
    opM(0xf1, read_idpy_sbc);
    opM(0xf2, read_idp_sbc);
    opM(0xf3, read_isry_sbc);
    opE(0xf4, pea);
    opM(0xf5, read_dpr_sbc);
    opM(0xf6, adjust_dpx_inc);
    opM(0xf7, read_ildpy_sbc);
    opA(0xf8, flag_sed);
    opM(0xf9, read_addry_sbc);
    opX(0xfa, pull_plx);
    opA_flags(0xfb, xce);
    opE(0xfc, jsr_iaddrx);
    opM(0xfd, read_addrx_sbc);
    opM(0xfe, adjust_addrx_inc);
    opM(0xff, read_longx_sbc);

#undef opA
#undef opE
#undef opM
#undef opX

}
