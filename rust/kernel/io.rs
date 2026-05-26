// SPDX-License-Identifier: GPL-2.0

//! Memory-mapped IO.
//!
//! C header: [`include/asm-generic/io.h`](srctree/include/asm-generic/io.h)

use crate::{
    bindings,
    prelude::*, //
};

pub mod mem;
pub mod poll;
<<<<<<< HEAD
pub mod register;
pub mod resource;

pub use crate::register;
pub use resource::Resource;

use register::LocatedRegister;

=======
pub mod resource;

pub use resource::Resource;

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
/// Physical address type.
///
/// This is a type alias to either `u32` or `u64` depending on the config option
/// `CONFIG_PHYS_ADDR_T_64BIT`, and it can be a u64 even on 32-bit architectures.
pub type PhysAddr = bindings::phys_addr_t;

/// Resource Size type.
///
/// This is a type alias to either `u32` or `u64` depending on the config option
/// `CONFIG_PHYS_ADDR_T_64BIT`, and it can be a u64 even on 32-bit architectures.
pub type ResourceSize = bindings::resource_size_t;

/// Raw representation of an MMIO region.
///
/// By itself, the existence of an instance of this structure does not provide any guarantees that
/// the represented MMIO region does exist or is properly mapped.
///
/// Instead, the bus specific MMIO implementation must convert this raw representation into an
/// `Mmio` instance providing the actual memory accessors. Only by the conversion into an `Mmio`
/// structure any guarantees are given.
pub struct MmioRaw<const SIZE: usize = 0> {
    addr: usize,
    maxsize: usize,
}

impl<const SIZE: usize> MmioRaw<SIZE> {
    /// Returns a new `MmioRaw` instance on success, an error otherwise.
    pub fn new(addr: usize, maxsize: usize) -> Result<Self> {
        if maxsize < SIZE {
            return Err(EINVAL);
        }

        Ok(Self { addr, maxsize })
    }

    /// Returns the base address of the MMIO region.
    #[inline]
    pub fn addr(&self) -> usize {
        self.addr
    }

    /// Returns the maximum size of the MMIO region.
    #[inline]
    pub fn maxsize(&self) -> usize {
        self.maxsize
    }
}

/// IO-mapped memory region.
///
/// The creator (usually a subsystem / bus such as PCI) is responsible for creating the
/// mapping, performing an additional region request etc.
///
/// # Invariant
///
/// `addr` is the start and `maxsize` the length of valid I/O mapped memory region of size
/// `maxsize`.
///
/// # Examples
///
/// ```no_run
/// use kernel::{
///     bindings,
///     ffi::c_void,
///     io::{
///         Io,
///         IoKnownSize,
///         Mmio,
///         MmioRaw,
///         PhysAddr,
///     },
/// };
/// use core::ops::Deref;
///
/// // See also `pci::Bar` for a real example.
/// struct IoMem<const SIZE: usize>(MmioRaw<SIZE>);
///
/// impl<const SIZE: usize> IoMem<SIZE> {
///     /// # Safety
///     ///
///     /// [`paddr`, `paddr` + `SIZE`) must be a valid MMIO region that is mappable into the CPUs
///     /// virtual address space.
///     unsafe fn new(paddr: usize) -> Result<Self>{
///         // SAFETY: By the safety requirements of this function [`paddr`, `paddr` + `SIZE`) is
///         // valid for `ioremap`.
///         let addr = unsafe { bindings::ioremap(paddr as PhysAddr, SIZE) };
///         if addr.is_null() {
///             return Err(ENOMEM);
///         }
///
///         Ok(IoMem(MmioRaw::new(addr as usize, SIZE)?))
///     }
/// }
///
/// impl<const SIZE: usize> Drop for IoMem<SIZE> {
///     fn drop(&mut self) {
///         // SAFETY: `self.0.addr()` is guaranteed to be properly mapped by `Self::new`.
///         unsafe { bindings::iounmap(self.0.addr() as *mut c_void); };
///     }
/// }
///
/// impl<const SIZE: usize> Deref for IoMem<SIZE> {
///    type Target = Mmio<SIZE>;
///
///    fn deref(&self) -> &Self::Target {
///         // SAFETY: The memory range stored in `self` has been properly mapped in `Self::new`.
///         unsafe { Mmio::from_raw(&self.0) }
///    }
/// }
///
///# fn no_run() -> Result<(), Error> {
/// // SAFETY: Invalid usage for example purposes.
/// let iomem = unsafe { IoMem::<{ core::mem::size_of::<u32>() }>::new(0xBAAAAAAD)? };
/// iomem.write32(0x42, 0x0);
/// assert!(iomem.try_write32(0x42, 0x0).is_ok());
/// assert!(iomem.try_write32(0x42, 0x4).is_err());
/// # Ok(())
/// # }
/// ```
#[repr(transparent)]
pub struct Mmio<const SIZE: usize = 0>(MmioRaw<SIZE>);

<<<<<<< HEAD
=======
/// Internal helper macros used to invoke C MMIO read functions.
///
/// This macro is intended to be used by higher-level MMIO access macros (io_define_read) and
/// provides a unified expansion for infallible vs. fallible read semantics. It emits a direct call
/// into the corresponding C helper and performs the required cast to the Rust return type.
///
/// # Parameters
///
/// * `$c_fn` – The C function performing the MMIO read.
/// * `$self` – The I/O backend object.
/// * `$ty` – The type of the value to be read.
/// * `$addr` – The MMIO address to read.
///
/// This macro does not perform any validation; all invariants must be upheld by the higher-level
/// abstraction invoking it.
macro_rules! call_mmio_read {
    (infallible, $c_fn:ident, $self:ident, $type:ty, $addr:expr) => {
        // SAFETY: By the type invariant `addr` is a valid address for MMIO operations.
        unsafe { bindings::$c_fn($addr as *const c_void) as $type }
    };

    (fallible, $c_fn:ident, $self:ident, $type:ty, $addr:expr) => {{
        // SAFETY: By the type invariant `addr` is a valid address for MMIO operations.
        Ok(unsafe { bindings::$c_fn($addr as *const c_void) as $type })
    }};
}

/// Internal helper macros used to invoke C MMIO write functions.
///
/// This macro is intended to be used by higher-level MMIO access macros (io_define_write) and
/// provides a unified expansion for infallible vs. fallible write semantics. It emits a direct call
/// into the corresponding C helper and performs the required cast to the Rust return type.
///
/// # Parameters
///
/// * `$c_fn` – The C function performing the MMIO write.
/// * `$self` – The I/O backend object.
/// * `$ty` – The type of the written value.
/// * `$addr` – The MMIO address to write.
/// * `$value` – The value to write.
///
/// This macro does not perform any validation; all invariants must be upheld by the higher-level
/// abstraction invoking it.
macro_rules! call_mmio_write {
    (infallible, $c_fn:ident, $self:ident, $ty:ty, $addr:expr, $value:expr) => {
        // SAFETY: By the type invariant `addr` is a valid address for MMIO operations.
        unsafe { bindings::$c_fn($value, $addr as *mut c_void) }
    };

    (fallible, $c_fn:ident, $self:ident, $ty:ty, $addr:expr, $value:expr) => {{
        // SAFETY: By the type invariant `addr` is a valid address for MMIO operations.
        unsafe { bindings::$c_fn($value, $addr as *mut c_void) };
        Ok(())
    }};
}

/// Generates an accessor method for reading from an I/O backend.
///
/// This macro reduces boilerplate by automatically generating either compile-time bounds-checked
/// (infallible) or runtime bounds-checked (fallible) read methods. It abstracts the address
/// calculation and bounds checking, and delegates the actual I/O read operation to a specified
/// helper macro, making it generic over different I/O backends.
///
/// # Parameters
///
/// * `infallible` / `fallible` - Determines the bounds-checking strategy. `infallible` relies on
///   `IoKnownSize` for compile-time checks and returns the value directly. `fallible` performs
///   runtime checks against `maxsize()` and returns a `Result<T>`.
/// * `$(#[$attr:meta])*` - Optional attributes to apply to the generated method (e.g.,
///   `#[cfg(CONFIG_64BIT)]` or inline directives).
/// * `$vis:vis` - The visibility of the generated method (e.g., `pub`).
/// * `$name:ident` / `$try_name:ident` - The name of the generated method (e.g., `read32`,
///   `try_read8`).
/// * `$call_macro:ident` - The backend-specific helper macro used to emit the actual I/O call
///   (e.g., `call_mmio_read`).
/// * `$c_fn:ident` - The backend-specific C function or identifier to be passed into the
///   `$call_macro`.
/// * `$type_name:ty` - The Rust type of the value being read (e.g., `u8`, `u32`).
#[macro_export]
macro_rules! io_define_read {
    (infallible, $(#[$attr:meta])* $vis:vis $name:ident, $call_macro:ident($c_fn:ident) ->
     $type_name:ty) => {
        /// Read IO data from a given offset known at compile time.
        ///
        /// Bound checks are performed on compile time, hence if the offset is not known at compile
        /// time, the build will fail.
        $(#[$attr])*
        // Always inline to optimize out error path of `io_addr_assert`.
        #[inline(always)]
        $vis fn $name(&self, offset: usize) -> $type_name {
            let addr = self.io_addr_assert::<$type_name>(offset);

            // SAFETY: By the type invariant `addr` is a valid address for IO operations.
            $call_macro!(infallible, $c_fn, self, $type_name, addr)
        }
    };

    (fallible, $(#[$attr:meta])* $vis:vis $try_name:ident, $call_macro:ident($c_fn:ident) ->
     $type_name:ty) => {
        /// Read IO data from a given offset.
        ///
        /// Bound checks are performed on runtime, it fails if the offset (plus the type size) is
        /// out of bounds.
        $(#[$attr])*
        $vis fn $try_name(&self, offset: usize) -> Result<$type_name> {
            let addr = self.io_addr::<$type_name>(offset)?;

            // SAFETY: By the type invariant `addr` is a valid address for IO operations.
            $call_macro!(fallible, $c_fn, self, $type_name, addr)
        }
    };
}
pub use io_define_read;

/// Generates an accessor method for writing to an I/O backend.
///
/// This macro reduces boilerplate by automatically generating either compile-time bounds-checked
/// (infallible) or runtime bounds-checked (fallible) write methods. It abstracts the address
/// calculation and bounds checking, and delegates the actual I/O write operation to a specified
/// helper macro, making it generic over different I/O backends.
///
/// # Parameters
///
/// * `infallible` / `fallible` - Determines the bounds-checking strategy. `infallible` relies on
///   `IoKnownSize` for compile-time checks and returns `()`. `fallible` performs runtime checks
///   against `maxsize()` and returns a `Result`.
/// * `$(#[$attr:meta])*` - Optional attributes to apply to the generated method (e.g.,
///   `#[cfg(CONFIG_64BIT)]` or inline directives).
/// * `$vis:vis` - The visibility of the generated method (e.g., `pub`).
/// * `$name:ident` / `$try_name:ident` - The name of the generated method (e.g., `write32`,
///   `try_write8`).
/// * `$call_macro:ident` - The backend-specific helper macro used to emit the actual I/O call
///   (e.g., `call_mmio_write`).
/// * `$c_fn:ident` - The backend-specific C function or identifier to be passed into the
///   `$call_macro`.
/// * `$type_name:ty` - The Rust type of the value being written (e.g., `u8`, `u32`). Note the use
///   of `<-` before the type to denote a write operation.
#[macro_export]
macro_rules! io_define_write {
    (infallible, $(#[$attr:meta])* $vis:vis $name:ident, $call_macro:ident($c_fn:ident) <-
     $type_name:ty) => {
        /// Write IO data from a given offset known at compile time.
        ///
        /// Bound checks are performed on compile time, hence if the offset is not known at compile
        /// time, the build will fail.
        $(#[$attr])*
        // Always inline to optimize out error path of `io_addr_assert`.
        #[inline(always)]
        $vis fn $name(&self, value: $type_name, offset: usize) {
            let addr = self.io_addr_assert::<$type_name>(offset);

            $call_macro!(infallible, $c_fn, self, $type_name, addr, value);
        }
    };

    (fallible, $(#[$attr:meta])* $vis:vis $try_name:ident, $call_macro:ident($c_fn:ident) <-
     $type_name:ty) => {
        /// Write IO data from a given offset.
        ///
        /// Bound checks are performed on runtime, it fails if the offset (plus the type size) is
        /// out of bounds.
        $(#[$attr])*
        $vis fn $try_name(&self, value: $type_name, offset: usize) -> Result {
            let addr = self.io_addr::<$type_name>(offset)?;

            $call_macro!(fallible, $c_fn, self, $type_name, addr, value)
        }
    };
}
pub use io_define_write;

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
/// Checks whether an access of type `U` at the given `offset`
/// is valid within this region.
#[inline]
const fn offset_valid<U>(offset: usize, size: usize) -> bool {
    let type_size = core::mem::size_of::<U>();
    if let Some(end) = offset.checked_add(type_size) {
        end <= size && offset % type_size == 0
    } else {
        false
    }
}

<<<<<<< HEAD
/// Trait indicating that an I/O backend supports operations of a certain type and providing an
/// implementation for these operations.
=======
/// Marker trait indicating that an I/O backend supports operations of a certain type.
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
///
/// Different I/O backends can implement this trait to expose only the operations they support.
///
/// For example, a PCI configuration space may implement `IoCapable<u8>`, `IoCapable<u16>`,
/// and `IoCapable<u32>`, but not `IoCapable<u64>`, while an MMIO region on a 64-bit
/// system might implement all four.
<<<<<<< HEAD
pub trait IoCapable<T> {
    /// Performs an I/O read of type `T` at `address` and returns the result.
    ///
    /// # Safety
    ///
    /// The range `[address..address + size_of::<T>()]` must be within the bounds of `Self`.
    unsafe fn io_read(&self, address: usize) -> T;

    /// Performs an I/O write of `value` at `address`.
    ///
    /// # Safety
    ///
    /// The range `[address..address + size_of::<T>()]` must be within the bounds of `Self`.
    unsafe fn io_write(&self, value: T, address: usize);
}

/// Describes a given I/O location: its offset, width, and type to convert the raw value from and
/// into.
///
/// This trait is the key abstraction allowing [`Io::read`], [`Io::write`], and [`Io::update`] (and
/// their fallible [`try_read`](Io::try_read), [`try_write`](Io::try_write) and
/// [`try_update`](Io::try_update) counterparts) to work uniformly with both raw [`usize`] offsets
/// (for primitive types like [`u32`]) and typed ones (like those generated by the [`register!`]
/// macro).
///
/// An `IoLoc<T>` carries three pieces of information:
///
/// - The offset to access (returned by [`IoLoc::offset`]),
/// - The width of the access (determined by [`IoLoc::IoType`]),
/// - The type `T` in which the raw data is returned or provided.
///
/// `T` and `IoLoc::IoType` may differ: for instance, a typed register has `T` = the register type
/// with its bitfields, and `IoType` = its backing primitive (e.g. `u32`).
pub trait IoLoc<T> {
    /// Size ([`u8`], [`u16`], etc) of the I/O performed on the returned [`offset`](IoLoc::offset).
    type IoType: Into<T> + From<T>;

    /// Consumes `self` and returns the offset of this location.
    fn offset(self) -> usize;
}

/// Implements [`IoLoc<$ty>`] for [`usize`], allowing [`usize`] to be used as a parameter of
/// [`Io::read`] and [`Io::write`].
macro_rules! impl_usize_ioloc {
    ($($ty:ty),*) => {
        $(
            impl IoLoc<$ty> for usize {
                type IoType = $ty;

                #[inline(always)]
                fn offset(self) -> usize {
                    self
                }
            }
        )*
    }
}

// Provide the ability to read any primitive type from a [`usize`].
impl_usize_ioloc!(u8, u16, u32, u64);
=======
pub trait IoCapable<T> {}
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

/// Types implementing this trait (e.g. MMIO BARs or PCI config regions)
/// can perform I/O operations on regions of memory.
///
/// This is an abstract representation to be implemented by arbitrary I/O
/// backends (e.g. MMIO, PCI config space, etc.).
///
/// The [`Io`] trait provides:
/// - Base address and size information
/// - Helper methods for offset validation and address calculation
/// - Fallible (runtime checked) accessors for different data widths
///
/// Which I/O methods are available depends on which [`IoCapable<T>`] traits
/// are implemented for the type.
///
/// # Examples
///
/// For MMIO regions, all widths (u8, u16, u32, and u64 on 64-bit systems) are typically
/// supported. For PCI configuration space, u8, u16, and u32 are supported but u64 is not.
pub trait Io {
    /// Returns the base address of this mapping.
    fn addr(&self) -> usize;

    /// Returns the maximum size of this mapping.
    fn maxsize(&self) -> usize;

    /// Returns the absolute I/O address for a given `offset`,
    /// performing runtime bound checks.
    #[inline]
    fn io_addr<U>(&self, offset: usize) -> Result<usize> {
        if !offset_valid::<U>(offset, self.maxsize()) {
            return Err(EINVAL);
        }

        // Probably no need to check, since the safety requirements of `Self::new` guarantee that
        // this can't overflow.
        self.addr().checked_add(offset).ok_or(EINVAL)
    }

    /// Fallible 8-bit read with runtime bounds check.
    #[inline(always)]
<<<<<<< HEAD
    fn try_read8(&self, offset: usize) -> Result<u8>
    where
        Self: IoCapable<u8>,
    {
        self.try_read(offset)
=======
    fn try_read8(&self, _offset: usize) -> Result<u8>
    where
        Self: IoCapable<u8>,
    {
        build_error!("Backend does not support fallible 8-bit read")
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
    }

    /// Fallible 16-bit read with runtime bounds check.
    #[inline(always)]
<<<<<<< HEAD
    fn try_read16(&self, offset: usize) -> Result<u16>
    where
        Self: IoCapable<u16>,
    {
        self.try_read(offset)
=======
    fn try_read16(&self, _offset: usize) -> Result<u16>
    where
        Self: IoCapable<u16>,
    {
        build_error!("Backend does not support fallible 16-bit read")
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
    }

    /// Fallible 32-bit read with runtime bounds check.
    #[inline(always)]
<<<<<<< HEAD
    fn try_read32(&self, offset: usize) -> Result<u32>
    where
        Self: IoCapable<u32>,
    {
        self.try_read(offset)
=======
    fn try_read32(&self, _offset: usize) -> Result<u32>
    where
        Self: IoCapable<u32>,
    {
        build_error!("Backend does not support fallible 32-bit read")
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
    }

    /// Fallible 64-bit read with runtime bounds check.
    #[inline(always)]
<<<<<<< HEAD
    fn try_read64(&self, offset: usize) -> Result<u64>
    where
        Self: IoCapable<u64>,
    {
        self.try_read(offset)
=======
    fn try_read64(&self, _offset: usize) -> Result<u64>
    where
        Self: IoCapable<u64>,
    {
        build_error!("Backend does not support fallible 64-bit read")
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
    }

    /// Fallible 8-bit write with runtime bounds check.
    #[inline(always)]
<<<<<<< HEAD
    fn try_write8(&self, value: u8, offset: usize) -> Result
    where
        Self: IoCapable<u8>,
    {
        self.try_write(offset, value)
=======
    fn try_write8(&self, _value: u8, _offset: usize) -> Result
    where
        Self: IoCapable<u8>,
    {
        build_error!("Backend does not support fallible 8-bit write")
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
    }

    /// Fallible 16-bit write with runtime bounds check.
    #[inline(always)]
<<<<<<< HEAD
    fn try_write16(&self, value: u16, offset: usize) -> Result
    where
        Self: IoCapable<u16>,
    {
        self.try_write(offset, value)
=======
    fn try_write16(&self, _value: u16, _offset: usize) -> Result
    where
        Self: IoCapable<u16>,
    {
        build_error!("Backend does not support fallible 16-bit write")
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
    }

    /// Fallible 32-bit write with runtime bounds check.
    #[inline(always)]
<<<<<<< HEAD
    fn try_write32(&self, value: u32, offset: usize) -> Result
    where
        Self: IoCapable<u32>,
    {
        self.try_write(offset, value)
=======
    fn try_write32(&self, _value: u32, _offset: usize) -> Result
    where
        Self: IoCapable<u32>,
    {
        build_error!("Backend does not support fallible 32-bit write")
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
    }

    /// Fallible 64-bit write with runtime bounds check.
    #[inline(always)]
<<<<<<< HEAD
    fn try_write64(&self, value: u64, offset: usize) -> Result
    where
        Self: IoCapable<u64>,
    {
        self.try_write(offset, value)
=======
    fn try_write64(&self, _value: u64, _offset: usize) -> Result
    where
        Self: IoCapable<u64>,
    {
        build_error!("Backend does not support fallible 64-bit write")
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
    }

    /// Infallible 8-bit read with compile-time bounds check.
    #[inline(always)]
<<<<<<< HEAD
    fn read8(&self, offset: usize) -> u8
    where
        Self: IoKnownSize + IoCapable<u8>,
    {
        self.read(offset)
=======
    fn read8(&self, _offset: usize) -> u8
    where
        Self: IoKnownSize + IoCapable<u8>,
    {
        build_error!("Backend does not support infallible 8-bit read")
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
    }

    /// Infallible 16-bit read with compile-time bounds check.
    #[inline(always)]
<<<<<<< HEAD
    fn read16(&self, offset: usize) -> u16
    where
        Self: IoKnownSize + IoCapable<u16>,
    {
        self.read(offset)
=======
    fn read16(&self, _offset: usize) -> u16
    where
        Self: IoKnownSize + IoCapable<u16>,
    {
        build_error!("Backend does not support infallible 16-bit read")
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
    }

    /// Infallible 32-bit read with compile-time bounds check.
    #[inline(always)]
<<<<<<< HEAD
    fn read32(&self, offset: usize) -> u32
    where
        Self: IoKnownSize + IoCapable<u32>,
    {
        self.read(offset)
=======
    fn read32(&self, _offset: usize) -> u32
    where
        Self: IoKnownSize + IoCapable<u32>,
    {
        build_error!("Backend does not support infallible 32-bit read")
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
    }

    /// Infallible 64-bit read with compile-time bounds check.
    #[inline(always)]
<<<<<<< HEAD
    fn read64(&self, offset: usize) -> u64
    where
        Self: IoKnownSize + IoCapable<u64>,
    {
        self.read(offset)
=======
    fn read64(&self, _offset: usize) -> u64
    where
        Self: IoKnownSize + IoCapable<u64>,
    {
        build_error!("Backend does not support infallible 64-bit read")
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
    }

    /// Infallible 8-bit write with compile-time bounds check.
    #[inline(always)]
<<<<<<< HEAD
    fn write8(&self, value: u8, offset: usize)
    where
        Self: IoKnownSize + IoCapable<u8>,
    {
        self.write(offset, value)
=======
    fn write8(&self, _value: u8, _offset: usize)
    where
        Self: IoKnownSize + IoCapable<u8>,
    {
        build_error!("Backend does not support infallible 8-bit write")
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
    }

    /// Infallible 16-bit write with compile-time bounds check.
    #[inline(always)]
<<<<<<< HEAD
    fn write16(&self, value: u16, offset: usize)
    where
        Self: IoKnownSize + IoCapable<u16>,
    {
        self.write(offset, value)
=======
    fn write16(&self, _value: u16, _offset: usize)
    where
        Self: IoKnownSize + IoCapable<u16>,
    {
        build_error!("Backend does not support infallible 16-bit write")
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
    }

    /// Infallible 32-bit write with compile-time bounds check.
    #[inline(always)]
<<<<<<< HEAD
    fn write32(&self, value: u32, offset: usize)
    where
        Self: IoKnownSize + IoCapable<u32>,
    {
        self.write(offset, value)
=======
    fn write32(&self, _value: u32, _offset: usize)
    where
        Self: IoKnownSize + IoCapable<u32>,
    {
        build_error!("Backend does not support infallible 32-bit write")
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
    }

    /// Infallible 64-bit write with compile-time bounds check.
    #[inline(always)]
<<<<<<< HEAD
    fn write64(&self, value: u64, offset: usize)
    where
        Self: IoKnownSize + IoCapable<u64>,
    {
        self.write(offset, value)
    }

    /// Generic fallible read with runtime bounds check.
    ///
    /// # Examples
    ///
    /// Read a primitive type from an I/O address:
    ///
    /// ```no_run
    /// use kernel::io::{
    ///     Io,
    ///     Mmio,
    /// };
    ///
    /// fn do_reads(io: &Mmio) -> Result {
    ///     // 32-bit read from address `0x10`.
    ///     let v: u32 = io.try_read(0x10)?;
    ///
    ///     // 8-bit read from address `0xfff`.
    ///     let v: u8 = io.try_read(0xfff)?;
    ///
    ///     Ok(())
    /// }
    /// ```
    #[inline(always)]
    fn try_read<T, L>(&self, location: L) -> Result<T>
    where
        L: IoLoc<T>,
        Self: IoCapable<L::IoType>,
    {
        let address = self.io_addr::<L::IoType>(location.offset())?;

        // SAFETY: `address` has been validated by `io_addr`.
        Ok(unsafe { self.io_read(address) }.into())
    }

    /// Generic fallible write with runtime bounds check.
    ///
    /// # Examples
    ///
    /// Write a primitive type to an I/O address:
    ///
    /// ```no_run
    /// use kernel::io::{
    ///     Io,
    ///     Mmio,
    /// };
    ///
    /// fn do_writes(io: &Mmio) -> Result {
    ///     // 32-bit write of value `1` at address `0x10`.
    ///     io.try_write(0x10, 1u32)?;
    ///
    ///     // 8-bit write of value `0xff` at address `0xfff`.
    ///     io.try_write(0xfff, 0xffu8)?;
    ///
    ///     Ok(())
    /// }
    /// ```
    #[inline(always)]
    fn try_write<T, L>(&self, location: L, value: T) -> Result
    where
        L: IoLoc<T>,
        Self: IoCapable<L::IoType>,
    {
        let address = self.io_addr::<L::IoType>(location.offset())?;
        let io_value = value.into();

        // SAFETY: `address` has been validated by `io_addr`.
        unsafe { self.io_write(io_value, address) }

        Ok(())
    }

    /// Generic fallible write of a fully-located register value.
    ///
    /// # Examples
    ///
    /// Tuples carrying a location and a value can be used with this method:
    ///
    /// ```no_run
    /// use kernel::io::{
    ///     register,
    ///     Io,
    ///     Mmio,
    /// };
    ///
    /// register! {
    ///     VERSION(u32) @ 0x100 {
    ///         15:8 major;
    ///         7:0  minor;
    ///     }
    /// }
    ///
    /// impl VERSION {
    ///     fn new(major: u8, minor: u8) -> Self {
    ///         VERSION::zeroed().with_major(major).with_minor(minor)
    ///     }
    /// }
    ///
    /// fn do_write_reg(io: &Mmio) -> Result {
    ///
    ///     io.try_write_reg(VERSION::new(1, 0))
    /// }
    /// ```
    #[inline(always)]
    fn try_write_reg<T, L, V>(&self, value: V) -> Result
    where
        L: IoLoc<T>,
        V: LocatedRegister<Location = L, Value = T>,
        Self: IoCapable<L::IoType>,
    {
        let (location, value) = value.into_io_op();

        self.try_write(location, value)
    }

    /// Generic fallible update with runtime bounds check.
    ///
    /// Note: this does not perform any synchronization. The caller is responsible for ensuring
    /// exclusive access if required.
    ///
    /// # Examples
    ///
    /// Read the u32 value at address `0x10`, increment it, and store the updated value back:
    ///
    /// ```no_run
    /// use kernel::io::{
    ///     Io,
    ///     Mmio,
    /// };
    ///
    /// fn do_update(io: &Mmio<0x1000>) -> Result {
    ///     io.try_update(0x10, |v: u32| {
    ///         v + 1
    ///     })
    /// }
    /// ```
    #[inline(always)]
    fn try_update<T, L, F>(&self, location: L, f: F) -> Result
    where
        L: IoLoc<T>,
        Self: IoCapable<L::IoType>,
        F: FnOnce(T) -> T,
    {
        let address = self.io_addr::<L::IoType>(location.offset())?;

        // SAFETY: `address` has been validated by `io_addr`.
        let value: T = unsafe { self.io_read(address) }.into();
        let io_value = f(value).into();

        // SAFETY: `address` has been validated by `io_addr`.
        unsafe { self.io_write(io_value, address) }

        Ok(())
    }

    /// Generic infallible read with compile-time bounds check.
    ///
    /// # Examples
    ///
    /// Read a primitive type from an I/O address:
    ///
    /// ```no_run
    /// use kernel::io::{
    ///     Io,
    ///     Mmio,
    /// };
    ///
    /// fn do_reads(io: &Mmio<0x1000>) {
    ///     // 32-bit read from address `0x10`.
    ///     let v: u32 = io.read(0x10);
    ///
    ///     // 8-bit read from the top of the I/O space.
    ///     let v: u8 = io.read(0xfff);
    /// }
    /// ```
    #[inline(always)]
    fn read<T, L>(&self, location: L) -> T
    where
        L: IoLoc<T>,
        Self: IoKnownSize + IoCapable<L::IoType>,
    {
        let address = self.io_addr_assert::<L::IoType>(location.offset());

        // SAFETY: `address` has been validated by `io_addr_assert`.
        unsafe { self.io_read(address) }.into()
    }

    /// Generic infallible write with compile-time bounds check.
    ///
    /// # Examples
    ///
    /// Write a primitive type to an I/O address:
    ///
    /// ```no_run
    /// use kernel::io::{
    ///     Io,
    ///     Mmio,
    /// };
    ///
    /// fn do_writes(io: &Mmio<0x1000>) {
    ///     // 32-bit write of value `1` at address `0x10`.
    ///     io.write(0x10, 1u32);
    ///
    ///     // 8-bit write of value `0xff` at the top of the I/O space.
    ///     io.write(0xfff, 0xffu8);
    /// }
    /// ```
    #[inline(always)]
    fn write<T, L>(&self, location: L, value: T)
    where
        L: IoLoc<T>,
        Self: IoKnownSize + IoCapable<L::IoType>,
    {
        let address = self.io_addr_assert::<L::IoType>(location.offset());
        let io_value = value.into();

        // SAFETY: `address` has been validated by `io_addr_assert`.
        unsafe { self.io_write(io_value, address) }
    }

    /// Generic infallible write of a fully-located register value.
    ///
    /// # Examples
    ///
    /// Tuples carrying a location and a value can be used with this method:
    ///
    /// ```no_run
    /// use kernel::io::{
    ///     register,
    ///     Io,
    ///     Mmio,
    /// };
    ///
    /// register! {
    ///     VERSION(u32) @ 0x100 {
    ///         15:8 major;
    ///         7:0  minor;
    ///     }
    /// }
    ///
    /// impl VERSION {
    ///     fn new(major: u8, minor: u8) -> Self {
    ///         VERSION::zeroed().with_major(major).with_minor(minor)
    ///     }
    /// }
    ///
    /// fn do_write_reg(io: &Mmio<0x1000>) {
    ///     io.write_reg(VERSION::new(1, 0));
    /// }
    /// ```
    #[inline(always)]
    fn write_reg<T, L, V>(&self, value: V)
    where
        L: IoLoc<T>,
        V: LocatedRegister<Location = L, Value = T>,
        Self: IoKnownSize + IoCapable<L::IoType>,
    {
        let (location, value) = value.into_io_op();

        self.write(location, value)
    }

    /// Generic infallible update with compile-time bounds check.
    ///
    /// Note: this does not perform any synchronization. The caller is responsible for ensuring
    /// exclusive access if required.
    ///
    /// # Examples
    ///
    /// Read the u32 value at address `0x10`, increment it, and store the updated value back:
    ///
    /// ```no_run
    /// use kernel::io::{
    ///     Io,
    ///     Mmio,
    /// };
    ///
    /// fn do_update(io: &Mmio<0x1000>) {
    ///     io.update(0x10, |v: u32| {
    ///         v + 1
    ///     })
    /// }
    /// ```
    #[inline(always)]
    fn update<T, L, F>(&self, location: L, f: F)
    where
        L: IoLoc<T>,
        Self: IoKnownSize + IoCapable<L::IoType> + Sized,
        F: FnOnce(T) -> T,
    {
        let address = self.io_addr_assert::<L::IoType>(location.offset());

        // SAFETY: `address` has been validated by `io_addr_assert`.
        let value: T = unsafe { self.io_read(address) }.into();
        let io_value = f(value).into();

        // SAFETY: `address` has been validated by `io_addr_assert`.
        unsafe { self.io_write(io_value, address) }
=======
    fn write64(&self, _value: u64, _offset: usize)
    where
        Self: IoKnownSize + IoCapable<u64>,
    {
        build_error!("Backend does not support infallible 64-bit write")
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
    }
}

/// Trait for types with a known size at compile time.
///
/// This trait is implemented by I/O backends that have a compile-time known size,
/// enabling the use of infallible I/O accessors with compile-time bounds checking.
///
/// Types implementing this trait can use the infallible methods in [`Io`] trait
/// (e.g., `read8`, `write32`), which require `Self: IoKnownSize` bound.
pub trait IoKnownSize: Io {
    /// Minimum usable size of this region.
    const MIN_SIZE: usize;

    /// Returns the absolute I/O address for a given `offset`,
    /// performing compile-time bound checks.
    // Always inline to optimize out error path of `build_assert`.
    #[inline(always)]
    fn io_addr_assert<U>(&self, offset: usize) -> usize {
        build_assert!(offset_valid::<U>(offset, Self::MIN_SIZE));

        self.addr() + offset
    }
}

<<<<<<< HEAD
/// Implements [`IoCapable`] on `$mmio` for `$ty` using `$read_fn` and `$write_fn`.
macro_rules! impl_mmio_io_capable {
    ($mmio:ident, $(#[$attr:meta])* $ty:ty, $read_fn:ident, $write_fn:ident) => {
        $(#[$attr])*
        impl<const SIZE: usize> IoCapable<$ty> for $mmio<SIZE> {
            unsafe fn io_read(&self, address: usize) -> $ty {
                // SAFETY: By the trait invariant `address` is a valid address for MMIO operations.
                unsafe { bindings::$read_fn(address as *const c_void) }
            }

            unsafe fn io_write(&self, value: $ty, address: usize) {
                // SAFETY: By the trait invariant `address` is a valid address for MMIO operations.
                unsafe { bindings::$write_fn(value, address as *mut c_void) }
            }
        }
    };
}

// MMIO regions support 8, 16, and 32-bit accesses.
impl_mmio_io_capable!(Mmio, u8, readb, writeb);
impl_mmio_io_capable!(Mmio, u16, readw, writew);
impl_mmio_io_capable!(Mmio, u32, readl, writel);
// MMIO regions on 64-bit systems also support 64-bit accesses.
impl_mmio_io_capable!(
    Mmio,
    #[cfg(CONFIG_64BIT)]
    u64,
    readq,
    writeq
);
=======
// MMIO regions support 8, 16, and 32-bit accesses.
impl<const SIZE: usize> IoCapable<u8> for Mmio<SIZE> {}
impl<const SIZE: usize> IoCapable<u16> for Mmio<SIZE> {}
impl<const SIZE: usize> IoCapable<u32> for Mmio<SIZE> {}

// MMIO regions on 64-bit systems also support 64-bit accesses.
#[cfg(CONFIG_64BIT)]
impl<const SIZE: usize> IoCapable<u64> for Mmio<SIZE> {}
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

impl<const SIZE: usize> Io for Mmio<SIZE> {
    /// Returns the base address of this mapping.
    #[inline]
    fn addr(&self) -> usize {
        self.0.addr()
    }

    /// Returns the maximum size of this mapping.
    #[inline]
    fn maxsize(&self) -> usize {
        self.0.maxsize()
    }
<<<<<<< HEAD
=======

    io_define_read!(fallible, try_read8, call_mmio_read(readb) -> u8);
    io_define_read!(fallible, try_read16, call_mmio_read(readw) -> u16);
    io_define_read!(fallible, try_read32, call_mmio_read(readl) -> u32);
    io_define_read!(
        fallible,
        #[cfg(CONFIG_64BIT)]
        try_read64,
        call_mmio_read(readq) -> u64
    );

    io_define_write!(fallible, try_write8, call_mmio_write(writeb) <- u8);
    io_define_write!(fallible, try_write16, call_mmio_write(writew) <- u16);
    io_define_write!(fallible, try_write32, call_mmio_write(writel) <- u32);
    io_define_write!(
        fallible,
        #[cfg(CONFIG_64BIT)]
        try_write64,
        call_mmio_write(writeq) <- u64
    );

    io_define_read!(infallible, read8, call_mmio_read(readb) -> u8);
    io_define_read!(infallible, read16, call_mmio_read(readw) -> u16);
    io_define_read!(infallible, read32, call_mmio_read(readl) -> u32);
    io_define_read!(
        infallible,
        #[cfg(CONFIG_64BIT)]
        read64,
        call_mmio_read(readq) -> u64
    );

    io_define_write!(infallible, write8, call_mmio_write(writeb) <- u8);
    io_define_write!(infallible, write16, call_mmio_write(writew) <- u16);
    io_define_write!(infallible, write32, call_mmio_write(writel) <- u32);
    io_define_write!(
        infallible,
        #[cfg(CONFIG_64BIT)]
        write64,
        call_mmio_write(writeq) <- u64
    );
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

impl<const SIZE: usize> IoKnownSize for Mmio<SIZE> {
    const MIN_SIZE: usize = SIZE;
}

impl<const SIZE: usize> Mmio<SIZE> {
    /// Converts an `MmioRaw` into an `Mmio` instance, providing the accessors to the MMIO mapping.
    ///
    /// # Safety
    ///
    /// Callers must ensure that `addr` is the start of a valid I/O mapped memory region of size
    /// `maxsize`.
    pub unsafe fn from_raw(raw: &MmioRaw<SIZE>) -> &Self {
        // SAFETY: `Mmio` is a transparent wrapper around `MmioRaw`.
        unsafe { &*core::ptr::from_ref(raw).cast() }
    }
<<<<<<< HEAD
}

/// [`Mmio`] wrapper using relaxed accessors.
///
/// This type provides an implementation of [`Io`] that uses relaxed I/O MMIO operands instead of
/// the regular ones.
///
/// See [`Mmio::relaxed`] for a usage example.
#[repr(transparent)]
pub struct RelaxedMmio<const SIZE: usize = 0>(Mmio<SIZE>);

impl<const SIZE: usize> Io for RelaxedMmio<SIZE> {
    #[inline]
    fn addr(&self) -> usize {
        self.0.addr()
    }

    #[inline]
    fn maxsize(&self) -> usize {
        self.0.maxsize()
    }
}

impl<const SIZE: usize> IoKnownSize for RelaxedMmio<SIZE> {
    const MIN_SIZE: usize = SIZE;
}

impl<const SIZE: usize> Mmio<SIZE> {
    /// Returns a [`RelaxedMmio`] reference that performs relaxed I/O operations.
    ///
    /// Relaxed accessors do not provide ordering guarantees with respect to DMA or memory accesses
    /// and can be used when such ordering is not required.
    ///
    /// # Examples
    ///
    /// ```no_run
    /// use kernel::io::{
    ///     Io,
    ///     Mmio,
    ///     RelaxedMmio,
    /// };
    ///
    /// fn do_io(io: &Mmio<0x100>) {
    ///     // The access is performed using `readl_relaxed` instead of `readl`.
    ///     let v = io.relaxed().read32(0x10);
    /// }
    ///
    /// ```
    pub fn relaxed(&self) -> &RelaxedMmio<SIZE> {
        // SAFETY: `RelaxedMmio` is `#[repr(transparent)]` over `Mmio`, so `Mmio<SIZE>` and
        // `RelaxedMmio<SIZE>` have identical layout.
        unsafe { core::mem::transmute(self) }
    }
}

// MMIO regions support 8, 16, and 32-bit accesses.
impl_mmio_io_capable!(RelaxedMmio, u8, readb_relaxed, writeb_relaxed);
impl_mmio_io_capable!(RelaxedMmio, u16, readw_relaxed, writew_relaxed);
impl_mmio_io_capable!(RelaxedMmio, u32, readl_relaxed, writel_relaxed);
// MMIO regions on 64-bit systems also support 64-bit accesses.
impl_mmio_io_capable!(
    RelaxedMmio,
    #[cfg(CONFIG_64BIT)]
    u64,
    readq_relaxed,
    writeq_relaxed
);
=======

    io_define_read!(infallible, pub read8_relaxed, call_mmio_read(readb_relaxed) -> u8);
    io_define_read!(infallible, pub read16_relaxed, call_mmio_read(readw_relaxed) -> u16);
    io_define_read!(infallible, pub read32_relaxed, call_mmio_read(readl_relaxed) -> u32);
    io_define_read!(
        infallible,
        #[cfg(CONFIG_64BIT)]
        pub read64_relaxed,
        call_mmio_read(readq_relaxed) -> u64
    );

    io_define_read!(fallible, pub try_read8_relaxed, call_mmio_read(readb_relaxed) -> u8);
    io_define_read!(fallible, pub try_read16_relaxed, call_mmio_read(readw_relaxed) -> u16);
    io_define_read!(fallible, pub try_read32_relaxed, call_mmio_read(readl_relaxed) -> u32);
    io_define_read!(
        fallible,
        #[cfg(CONFIG_64BIT)]
        pub try_read64_relaxed,
        call_mmio_read(readq_relaxed) -> u64
    );

    io_define_write!(infallible, pub write8_relaxed, call_mmio_write(writeb_relaxed) <- u8);
    io_define_write!(infallible, pub write16_relaxed, call_mmio_write(writew_relaxed) <- u16);
    io_define_write!(infallible, pub write32_relaxed, call_mmio_write(writel_relaxed) <- u32);
    io_define_write!(
        infallible,
        #[cfg(CONFIG_64BIT)]
        pub write64_relaxed,
        call_mmio_write(writeq_relaxed) <- u64
    );

    io_define_write!(fallible, pub try_write8_relaxed, call_mmio_write(writeb_relaxed) <- u8);
    io_define_write!(fallible, pub try_write16_relaxed, call_mmio_write(writew_relaxed) <- u16);
    io_define_write!(fallible, pub try_write32_relaxed, call_mmio_write(writel_relaxed) <- u32);
    io_define_write!(
        fallible,
        #[cfg(CONFIG_64BIT)]
        pub try_write64_relaxed,
        call_mmio_write(writeq_relaxed) <- u64
    );
}
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
