// SPDX-License-Identifier: GPL-2.0 or MIT

//! Arm Mali Tyr DRM driver.
//!
//! The name "Tyr" is inspired by Norse mythology, reflecting Arm's tradition of
//! naming their GPUs after Nordic mythological figures and places.

<<<<<<< HEAD
use crate::driver::TyrPlatformDriverData;
=======
use crate::driver::TyrDriver;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

mod driver;
mod file;
mod gem;
mod gpu;
mod regs;

kernel::module_platform_driver! {
<<<<<<< HEAD
    type: TyrPlatformDriverData,
=======
    type: TyrDriver,
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
    name: "tyr",
    authors: ["The Tyr driver authors"],
    description: "Arm Mali Tyr DRM driver",
    license: "Dual MIT/GPL",
}
