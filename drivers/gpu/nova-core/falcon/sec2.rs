// SPDX-License-Identifier: GPL-2.0

<<<<<<< HEAD
use kernel::io::register::RegisterBase;

use crate::falcon::{
    FalconEngine,
    PFalcon2Base,
    PFalconBase, //
=======
use crate::{
    falcon::{
        FalconEngine,
        PFalcon2Base,
        PFalconBase, //
    },
    regs::macros::RegisterBase,
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
};

/// Type specifying the `Sec2` falcon engine. Cannot be instantiated.
pub(crate) struct Sec2(());

impl RegisterBase<PFalconBase> for Sec2 {
    const BASE: usize = 0x00840000;
}

impl RegisterBase<PFalcon2Base> for Sec2 {
    const BASE: usize = 0x00841000;
}

<<<<<<< HEAD
impl FalconEngine for Sec2 {}
=======
impl FalconEngine for Sec2 {
    const ID: Self = Sec2(());
}
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
