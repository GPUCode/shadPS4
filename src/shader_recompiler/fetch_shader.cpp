#include "shader_recompiler/decoder.h"
#include "shader_recompiler/fetch_shader.h"

namespace Shader::Gcn {

GcnFetchShader::GcnFetchShader(const u8* code) {
    parseVsInputSemantic(code);
}

GcnFetchShader::~GcnFetchShader() = default;

void GcnFetchShader::parseVsInputSemantic(const u8* code) {
    const u32* start = reinterpret_cast<const u32*>(code);
    const u32* end = reinterpret_cast<const u32*>(code + std::numeric_limits<u32>::max());

    GcnCodeSlice codeSlice(start, end);
    GcnDecodeContext decoder;

    u32 semanticIndex = 0;
    while (!codeSlice.atEnd()) {
        decoder.decodeInstruction(codeSlice);

        // s_load_dwordx4 s[8:11], s[2:3], 0x00                      // 00000000: C0840300
        // s_load_dwordx4 s[12:15], s[2:3], 0x04                     // 00000004: C0860304
        // s_load_dwordx4 s[16:19], s[2:3], 0x08                     // 00000008: C0880308
        // s_waitcnt     lgkmcnt(0)                                  // 0000000C: BF8C007F
        // buffer_load_format_xyzw v[4:7], v0, s[8:11], 0 idxen      // 00000010: E00C2000 80020400
        // buffer_load_format_xyz v[8:10], v0, s[12:15], 0 idxen     // 00000018: E0082000 80030800
        // buffer_load_format_xy v[12:13], v0, s[16:19], 0 idxen     // 00000020: E0042000 80040C00
        // s_waitcnt     0                                           // 00000028: BF8C0000
        // s_setpc_b64   s[0:1]                                      // 0000002C: BE802000

        // A normal fetch shader looks like the above, the instructions are generated
        // using input semantics on cpu side.
        // We take the reverse way, extract the original input semantics from these instructions.

        const auto& ins = decoder.getInstruction();
        if (ins.opcode == GcnOpcode::S_SETPC_B64) {
            break;
        }

        if (ins.opClass != GcnInstClass::VectorMemBufFmt) {
            // We only care about the buffer_load_format_xxx instructions
            continue;
        }

        GcnShaderInstMUBUF mubuf = gcnInstructionAs<GcnShaderInstMUBUF>(ins);

        VertexInputSemantic semantic = {0};
        semantic.m_semantic = semanticIndex++;
        semantic.m_vgpr = mubuf.vdata.code;
        semantic.m_sizeInElements = mubuf.control.count;
        semantic.m_reserved = 0;

        m_vsInputSemanticTable.push_back(semantic);
    }
}

} // namespace Shader::Gcn
