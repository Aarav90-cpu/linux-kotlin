// SPDX-License-Identifier: GPL-2.0 or MIT

<<<<<<< HEAD
use kernel::{
    drm::gem,
    prelude::*, //
};

use crate::driver::{
    TyrDrmDevice,
    TyrDrmDriver, //
};
=======
use crate::driver::TyrDevice;
use crate::driver::TyrDriver;
use kernel::drm::gem;
use kernel::prelude::*;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

/// GEM Object inner driver data
#[pin_data]
pub(crate) struct TyrObject {}

impl gem::DriverObject for TyrObject {
<<<<<<< HEAD
    type Driver = TyrDrmDriver;
    type Args = ();

    fn new(_dev: &TyrDrmDevice, _size: usize, _args: ()) -> impl PinInit<Self, Error> {
=======
    type Driver = TyrDriver;

    fn new(_dev: &TyrDevice, _size: usize) -> impl PinInit<Self, Error> {
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
        try_pin_init!(TyrObject {})
    }
}
