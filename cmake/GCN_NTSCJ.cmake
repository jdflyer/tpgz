# ASM values
set(TPGZ_CFG_ARENALO "0x8033c54c")
set(TPGZ_CFG_ARENALO_VAL "0x3c60804D")
set(TPGZ_CFG_MAIN "0x80006454")
set(TPGZ_CFG_MAIN_TRAMPOLINE "0x80375c44")
set(TPGZ_CFG_FIRST_INSTRUCTION "0x9421fff0")
set(TPGZ_CFG_RETURN_TO_MAIN "0x4BC90810")
set(TPGZ_CFG_SET_WATER_DROP_EFFECT1 "0x801244f0")
set(TPGZ_CFG_SET_WATER_DROP_EFFECT2 "0x801244f4")
set(TPGZ_CFG_SET_WATER_DROP_EFFECT3 "0x801244f8")
set(TPGZ_CFG_WATER_DROP_COLOR "0x800CCDE0")
set(TPGZ_CFG_DEBUG "0x8000b8a4")
set(TPGZ_CFG_DEBUG_INSTRUCTION "0x48000014")

# TOML values
set(TPGZ_CFG_SRC_ISO "../isos/GZ2J01.iso")
set(TPGZ_CFG_SRC_MAP "map/Final/Release/frameworkF.map")
set(TPGZ_CFG_BLD_ISO "tpgz.iso")
set(TPGZ_CFG_BLD_MAP "GZ2J01.map")
set(TPGZ_CFG_LINK_BASE "0x8046_0000")
set(TPGZ_CFG_SAVE_ANY_PATH "res/save_files")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/patch.asm.in ${CMAKE_CURRENT_BINARY_DIR}/patch.asm)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/RomHack.toml.in ${CMAKE_CURRENT_BINARY_DIR}/RomHack.toml)