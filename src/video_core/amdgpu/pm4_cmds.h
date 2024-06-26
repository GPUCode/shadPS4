// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <cstring>
#include "common/bit_field.h"
#include "common/types.h"
#include "video_core/amdgpu/pm4_opcodes.h"

namespace AmdGpu {

/// This enum defines the Shader types supported in PM4 type 3 header
enum class PM4ShaderType : u32 {
    ShaderGraphics = 0, ///< Graphics shader
    ShaderCompute = 1   ///< Compute shader
};

/// This enum defines the predicate value supported in PM4 type 3 header
enum class PM4Predicate : u32 {
    PredDisable = 0, ///< Predicate disabled
    PredEnable = 1   ///< Predicate enabled
};

union PM4Type0Header {
    u32 raw;
    BitField<0, 16, u32> base;   ///< DWORD Memory-mapped address
    BitField<16, 14, u32> count; ///< Count of DWORDs in the *information* body (N - 1 for N dwords)
    BitField<30, 2, u32> type;   ///< Packet identifier. It should be 0 for type 0 packets.

    u32 NumWords() const {
        return count + 1;
    }
};

union PM4Type3Header {
    constexpr PM4Type3Header(PM4ItOpcode code, u32 num_words_min_one,
                             PM4ShaderType stype = PM4ShaderType::ShaderGraphics,
                             PM4Predicate pred = PM4Predicate::PredDisable) {
        raw = 0;
        predicate.Assign(pred);
        shader_type.Assign(stype);
        opcode.Assign(code);
        count.Assign(num_words_min_one);
        type.Assign(3);
    }

    u32 NumWords() const {
        return count + 1;
    }

    u32 raw;
    BitField<0, 1, PM4Predicate> predicate;    ///< Predicated version of packet when set
    BitField<1, 1, PM4ShaderType> shader_type; ///< 0: Graphics, 1: Compute Shader
    BitField<8, 8, PM4ItOpcode> opcode;        ///< IT opcode
    BitField<16, 14, u32> count;               ///< Number of DWORDs - 1 in the information body.
    BitField<30, 2, u32> type; ///< Packet identifier. It should be 3 for type 3 packets
};

union PM4Header {
    u32 raw;
    PM4Type0Header type0;
    PM4Type3Header type3;
    BitField<30, 2, u32> type;
};

// Write the PM4 header
template <PM4ItOpcode opcode>
constexpr u32* WriteHeader(u32* cmdbuf, u32 size,
                           PM4ShaderType type = PM4ShaderType::ShaderGraphics,
                           PM4Predicate predicate = PM4Predicate::PredDisable) {
    PM4Type3Header header{opcode, size - 1, type, predicate};
    std::memcpy(cmdbuf, &header, sizeof(header));
    return ++cmdbuf;
}

// Write arguments
template <typename... Args>
constexpr u32* WriteBody(u32* cmdbuf, Args... data) {
    const std::array<u32, sizeof...(Args)> args{data...};
    std::memcpy(cmdbuf, args.data(), sizeof(args));
    cmdbuf += args.size();
    return cmdbuf;
}

template <PM4ItOpcode opcode, typename... Args>
constexpr u32* WritePacket(u32* cmdbuf, PM4ShaderType type, Args... data) {
    cmdbuf = WriteHeader<opcode>(cmdbuf, sizeof...(Args), type);
    cmdbuf = WriteBody(cmdbuf, data...);
    return cmdbuf;
}

union ContextControlEnable {
    u32 raw;
    BitField<0, 1, u32> enable_single_cntx_config_reg; ///< single context config reg
    BitField<1, 1, u32> enable_multi_cntx_render_reg;  ///< multi context render state reg
    BitField<15, 1, u32> enable_user_config_reg__CI;   ///< User Config Reg on CI(reserved for SI)
    BitField<16, 1, u32> enable_gfx_sh_reg;            ///< Gfx SH Registers
    BitField<24, 1, u32> enable_cs_sh_reg;             ///< CS SH Registers
    BitField<31, 1, u32> enable_dw;                    ///< DW enable
};

struct PM4CmdContextControl {
    PM4Type3Header header;
    ContextControlEnable load_control;  ///< Enable bits for loading
    ContextControlEnable shadow_enable; ///< Enable bits for shadowing
};

union LoadAddressHigh {
    u32 raw;
    BitField<0, 16, u32>
        addr_hi; ///< bits for the block in Memory from where the CP will fetch the state
    BitField<31, 1, u32>
        wait_idle; ///< if set the CP will wait for the graphics pipe to be idle by writing
                   ///< to the GRBM Wait Until register with "Wait for 3D idle"
};

/**
 * PM4CMDLOADDATA can be used with the following opcodes
 * - IT_LOAD_CONFIG_REG
 * - IT_LOAD_CONTEXT_REG
 * - IT_LOAD_SH_REG
 */
struct PM4CmdLoadData {
    PM4Type3Header header;
    u32 addr_lo; ///< low 32 address bits for the block in memory from where the CP will fetch the
                 ///< state
    LoadAddressHigh addr_hi;
    u32 reg_offset; ///< offset in DWords from the register base address
    u32 num_dwords; ///< number of DWords that the CP will fetch and write into the chip. A value of
                    ///< zero will fetch nothing
};

enum class LoadDataIndex : u32 {
    DirectAddress = 0, /// ADDR_LO is direct address
    Offset = 1,        /// ARRD_LO is ignored and memory offset is in addrOffset
};

enum class LoadDataFormat : u32 {
    OffsetAndSize = 0, /// Data is consecutive DWORDs
    OffsetAndData = 1, /// Register offset and data is interleaved
};

union LoadAddressLow {
    u32 raw;
    BitField<0, 1, LoadDataIndex> index;
    BitField<2, 30, u32> addr_lo; ///< bits for the block in Memory from where the CP will fetch the
                                  ///< state. DWORD aligned
};

/**
 * PM4CMDLOADDATAINDEX can be used with the following opcodes (VI+)
 * - IT_LOAD_CONTEXT_REG_INDEX
 * - IT_LOAD_SH_REG_INDEX
 */
struct PM4CmdLoadDataIndex {
    PM4Type3Header header;
    LoadAddressLow addr_lo; ///< low 32 address bits for the block in memory from where the CP will
                            ///< fetch the state
    u32 addr_offset;        ///< addrLo.index = 1 Indexed mode
    union {
        BitField<0, 16, u32> reg_offset; ///< offset in DWords from the register base address
        BitField<31, 1, LoadDataFormat> data_format;
        u32 raw;
    };
    u32 num_dwords; ///< Number of DWords that the CP will fetch and write
                    ///< into the chip. A value of zero will fetch nothing
};

/**
 * PM4CMDSETDATA can be used with the following opcodes:
 *
 * - IT_SET_CONFIG_REG
 * - IT_SET_CONTEXT_REG
 * - IT_SET_CONTEXT_REG_INDIRECT
 * - IT_SET_SH_REG
 * - IT_SET_SH_REG_INDEX
 * - IT_SET_UCONFIG_REG
 */
struct PM4CmdSetData {
    PM4Type3Header header;
    union {
        u32 raw;
        BitField<0, 16, u32> reg_offset; ///< Offset in DWords from the register base address
        BitField<28, 4, u32> index;      ///< Index for UCONFIG/CONTEXT on CI+
                                         ///< Program to zero for other opcodes and on SI
    };

    template <PM4ShaderType type = PM4ShaderType::ShaderGraphics, typename... Args>
    static constexpr u32* SetContextReg(u32* cmdbuf, Args... data) {
        return WritePacket<PM4ItOpcode::SetContextReg>(cmdbuf, type, data...);
    }

    template <PM4ShaderType type = PM4ShaderType::ShaderGraphics, typename... Args>
    static constexpr u32* SetShReg(u32* cmdbuf, Args... data) {
        return WritePacket<PM4ItOpcode::SetShReg>(cmdbuf, type, data...);
    }
};

struct PM4CmdNop {
    PM4Type3Header header;
    u32 data_block[0];

    enum class PayloadType : u32 {
        DebugMarkerPush = 0x68750001,      ///< Begin of GPU event scope
        DebugMarkerPop = 0x68750002,       ///< End of GPU event scope
        SetVsharpInUdata = 0x68750004,     ///< Indicates that V# will be set in the next packet
        SetTsharpInUdata = 0x68750005,     ///< Indicates that T# will be set in the next packet
        SetSsharpInUdata = 0x68750006,     ///< Indicates that S# will be set in the next packet
        DebugColorMarkerPush = 0x6875000e, ///< Begin of GPU event scope with color
    };
};

struct PM4CmdDrawIndexOffset2 {
    PM4Type3Header header;
    u32 max_size;       ///< Maximum number of indices
    u32 index_offset;   ///< Zero based starting index number in the index buffer
    u32 index_count;    ///< number of indices in the Index Buffer
    u32 draw_initiator; ///< draw Initiator Register
};

struct PM4CmdDrawIndex2 {
    PM4Type3Header header;
    u32 max_size;       ///< maximum number of indices
    u32 index_base_lo;  ///< base Address Lo [31:1] of Index Buffer
                        ///< (Word-Aligned). Written to the VGT_DMA_BASE register.
    u32 index_base_hi;  ///< base Address Hi [39:32] of Index Buffer.
                        ///< Written to the VGT_DMA_BASE_HI register
    u32 index_count;    ///< number of indices in the Index Buffer.
                        ///< Written to the VGT_NUM_INDICES register.
    u32 draw_initiator; ///< written to the VGT_DRAW_INITIATOR register
};

struct PM4CmdDrawIndexType {
    PM4Type3Header header;
    union {
        u32 raw;
        BitField<0, 2, u32> index_type; ///< Select 16 Vs 32bit index
        BitField<2, 2, u32> swap_mode;  ///< DMA swap mode
    };
};

struct PM4CmdDrawIndexAuto {
    PM4Type3Header header;
    u32 index_count;
    u32 draw_initiator;
};

enum class DataSelect : u32 {
    None = 0,
    Data32Low = 1,
    Data64 = 2,
    GpuClock64 = 3,
    PerfCounter = 4,
};

enum class InterruptSelect : u32 {
    None = 0,
    IrqOnly = 1,
    IrqWhenWriteConfirm = 2,
};

struct PM4CmdEventWriteEop {
    PM4Type3Header header;
    union {
        u32 event_control;
        BitField<0, 6, u32> event_type;  ///< Event type written to VGT_EVENT_INITIATOR
        BitField<8, 4, u32> event_index; ///< Event index
    };
    u32 address_lo;
    union {
        u32 data_control;
        BitField<0, 16, u32> address_hi;          ///< High bits of address
        BitField<24, 2, InterruptSelect> int_sel; ///< Selects interrupt action for end-of-pipe
        BitField<29, 3, DataSelect> data_sel;     ///< Selects source of data
    };
    u32 data_lo; ///< Value that will be written to memory when event occurs
    u32 data_hi; ///< Value that will be written to memory when event occurs

    u64* Address() const {
        return reinterpret_cast<u64*>(address_lo | u64(address_hi) << 32);
    }

    u64 DataQWord() const {
        return data_lo | u64(data_hi) << 32;
    }
};

struct PM4DmaData {
    PM4Type3Header header;
    union {
        BitField<0, 1, u32> engine;
        BitField<12, 1, u32> src_atc;
        BitField<13, 2, u32> src_cache_policy;
        BitField<15, 1, u32> src_volatile;
        BitField<20, 2, u32> dst_sel;
        BitField<24, 1, u32> dst_atc;
        BitField<25, 2, u32> dst_cache_policy;
        BitField<27, 1, u32> dst_volatile;
        BitField<29, 2, u32> src_sel;
        BitField<31, 1, u32> cp_sync;
    };
    union {
        u32 src_addr_lo;
        u32 data;
    };
    u32 src_addr_hi;
    u32 dst_addr_lo;
    u32 dst_addr_hi;
    u32 command;
};

struct PM4CmdWaitRegMem {
    PM4Type3Header header;
    union {
        BitField<0, 3, u32> function;
        BitField<4, 1, u32> mem_space;
        BitField<8, 1, u32> engine;
        u32 raw;
    };
    u32 poll_addr_lo;
    u32 poll_addr_hi;
    u32 ref;
    u32 mask;
    u32 poll_interval;
};

} // namespace AmdGpu
