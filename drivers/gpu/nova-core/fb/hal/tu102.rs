// SPDX-License-Identifier: GPL-2.0

<<<<<<< HEAD
use kernel::{
    io::Io,
    prelude::*, //
};
=======
use kernel::prelude::*;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

use crate::{
    driver::Bar0,
    fb::hal::FbHal,
    regs, //
};

/// Shift applied to the sysmem address before it is written into `NV_PFB_NISO_FLUSH_SYSMEM_ADDR`,
/// to be used by HALs.
pub(super) const FLUSH_SYSMEM_ADDR_SHIFT: u32 = 8;

pub(super) fn read_sysmem_flush_page_gm107(bar: &Bar0) -> u64 {
<<<<<<< HEAD
    u64::from(bar.read(regs::NV_PFB_NISO_FLUSH_SYSMEM_ADDR).adr_39_08()) << FLUSH_SYSMEM_ADDR_SHIFT
=======
    u64::from(regs::NV_PFB_NISO_FLUSH_SYSMEM_ADDR::read(bar).adr_39_08()) << FLUSH_SYSMEM_ADDR_SHIFT
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

pub(super) fn write_sysmem_flush_page_gm107(bar: &Bar0, addr: u64) -> Result {
    // Check that the address doesn't overflow the receiving 32-bit register.
    u32::try_from(addr >> FLUSH_SYSMEM_ADDR_SHIFT)
        .map_err(|_| EINVAL)
        .map(|addr| {
<<<<<<< HEAD
            bar.write_reg(regs::NV_PFB_NISO_FLUSH_SYSMEM_ADDR::zeroed().with_adr_39_08(addr))
=======
            regs::NV_PFB_NISO_FLUSH_SYSMEM_ADDR::default()
                .set_adr_39_08(addr)
                .write(bar)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
        })
}

pub(super) fn display_enabled_gm107(bar: &Bar0) -> bool {
<<<<<<< HEAD
    !bar.read(regs::gm107::NV_FUSE_STATUS_OPT_DISPLAY)
        .display_disabled()
}

pub(super) fn vidmem_size_gp102(bar: &Bar0) -> u64 {
    bar.read(regs::NV_PFB_PRI_MMU_LOCAL_MEMORY_RANGE)
        .usable_fb_size()
=======
    !regs::gm107::NV_FUSE_STATUS_OPT_DISPLAY::read(bar).display_disabled()
}

pub(super) fn vidmem_size_gp102(bar: &Bar0) -> u64 {
    regs::NV_PFB_PRI_MMU_LOCAL_MEMORY_RANGE::read(bar).usable_fb_size()
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

struct Tu102;

impl FbHal for Tu102 {
    fn read_sysmem_flush_page(&self, bar: &Bar0) -> u64 {
        read_sysmem_flush_page_gm107(bar)
    }

    fn write_sysmem_flush_page(&self, bar: &Bar0, addr: u64) -> Result {
        write_sysmem_flush_page_gm107(bar, addr)
    }

    fn supports_display(&self, bar: &Bar0) -> bool {
        display_enabled_gm107(bar)
    }

    fn vidmem_size(&self, bar: &Bar0) -> u64 {
        vidmem_size_gp102(bar)
    }
}

const TU102: Tu102 = Tu102;
pub(super) const TU102_HAL: &dyn FbHal = &TU102;
