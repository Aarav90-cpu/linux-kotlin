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

fn vidmem_size_ga102(bar: &Bar0) -> u64 {
<<<<<<< HEAD
    bar.read(regs::NV_USABLE_FB_SIZE_IN_MB).usable_fb_size()
=======
    regs::NV_USABLE_FB_SIZE_IN_MB::read(bar).usable_fb_size()
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

struct Ga102;

impl FbHal for Ga102 {
    fn read_sysmem_flush_page(&self, bar: &Bar0) -> u64 {
        super::ga100::read_sysmem_flush_page_ga100(bar)
    }

    fn write_sysmem_flush_page(&self, bar: &Bar0, addr: u64) -> Result {
        super::ga100::write_sysmem_flush_page_ga100(bar, addr);

        Ok(())
    }

    fn supports_display(&self, bar: &Bar0) -> bool {
        super::ga100::display_enabled_ga100(bar)
    }

    fn vidmem_size(&self, bar: &Bar0) -> u64 {
        vidmem_size_ga102(bar)
    }
}

const GA102: Ga102 = Ga102;
pub(super) const GA102_HAL: &dyn FbHal = &GA102;
