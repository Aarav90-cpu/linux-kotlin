.. SPDX-License-Identifier: GPL-2.0

=========
S/390 PCI
=========

Authors:
        - Pierre Morel
<<<<<<< HEAD
        - Niklas Schnelle
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

Copyright, IBM Corp. 2020


Command line parameters and debugfs entries
===========================================

Command line parameters
-----------------------

* nomio

  Do not use PCI Mapped I/O (MIO) instructions.

* norid

  Ignore the RID field and force use of one PCI domain per PCI function.

debugfs entries
---------------

<<<<<<< HEAD
The S/390 debug feature (s390dbf) generates views to hold various debug results
in sysfs directories of the form:
=======
The S/390 debug feature (s390dbf) generates views to hold various debug results in sysfs directories of the form:
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

 * /sys/kernel/debug/s390dbf/pci_*/

For example:

  - /sys/kernel/debug/s390dbf/pci_msg/sprintf
<<<<<<< HEAD

    holds messages from the processing of PCI events, like machine check handling
=======
    Holds messages from the processing of PCI events, like machine check handling
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
    and setting of global functionality, like UID checking.

  Change the level of logging to be more or less verbose by piping
  a number between 0 and 6 to  /sys/kernel/debug/s390dbf/pci_*/level. For
  details, see the documentation on the S/390 debug feature at
  Documentation/arch/s390/s390dbf.rst.

Sysfs entries
=============

Entries specific to zPCI functions and entries that hold zPCI information.

<<<<<<< HEAD
* /sys/bus/pci/slots/XXXXXXXX:

  The slot entries are set up using the function identifier (FID) of the PCI
  function as slot name. The format depicted as XXXXXXXX above is 8 hexadecimal
  digits with 0 padding and lower case hexadecimal digits.

  - /sys/bus/pci/slots/XXXXXXXX/power

  In addition to using the FID as the name of the slot, the slot directory
  also contains the following s390-specific slot attributes.

  - uid:
    The User-defined identifier (UID) of the function which may be configured
    by this slot. See also the corresponding attribute of the device.

  A physical function that currently supports a virtual function cannot be
  powered off until all virtual functions are removed with:
  echo 0 > /sys/bus/pci/devices/DDDD:BB:dd.f/sriov_numvf

* /sys/bus/pci/devices/DDDD:BB:dd.f/:

  - function_id:
    The zPCI function identifier (FID) is a 32-bit hexadecimal value that
    uniquely identifies the PCI function. Unless the hypervisor provides
    a virtual FID e.g. on KVM this identifier is unique across the machine even
    between different partitions.

  - function_handle:
    This 32-bit hexadecimal value is a low-level identifier used for a PCI
    function. Note that the function handle may be changed and become invalid
    on PCI events and when enabling/disabling the PCI function.

  - pchid:
    This 16-bit hexadecimal value encodes a model-dependent location for
    the PCI function.

  - pfgid:
    PCI function group ID; functions that share identical functionality
    use a common identifier.
    A PCI group defines interrupts, IOMMU, IOTLB, and DMA specifics.

  - vfn:
    The virtual function number, from 1 to N for virtual functions,
    0 for physical functions.

  - pft:
    The PCI function type is an s390-specific type attribute. It indicates
    a more general, usage oriented, type than PCI Specification
    class/vendor/device identifiers. That is PCI functions with the same pft
    value may be backed by different hardware implementations. At the same time
    apart from unclassified functions (pft is 0x00) the same pft value
    generally implies a similar usage model. At the same time the same
    PCI hardware device may appear with different pft values when in a
    different usage model. For example NETD and NETH VFs may be implemented
    by the same PCI hardware device but in NETD the parent Physical Function
    is user managed while with NETH it is platform managed.

    Currently the following PFT values are defined:

    - 0x00 (UNC): Unclassified
    - 0x02 (ROCE): RoCE Express
    - 0x05 (ISM): Internal Shared Memory
    - 0x0a (ROC2): RoCE Express 2
    - 0x0b (NVMe): NVMe
    - 0x0c (NETH): Network Express hybrid
    - 0x0d (CNW): Cloud Network Adapter
    - 0x0f (NETD): Network Express direct

  - port:
    The port is a decimal value corresponding to the physical port the function
    is attached to. Virtual Functions (VFs) share the port with their parent
    Physical Function (PF). A value of 0 indicates that the port attribute is
    not applicable for that PCI function type.

  - uid:
    The user-defined identifier (UID) for a PCI function is a 32-bit
    hexadecimal value. It is defined on a per instance basis as part of the
    partition, KVM guest, or z/VM guest configuration. If UID Checking is
    enabled the platform ensures that the UID is unique within that instance
    and no two PCI functions with the same UID will be visible to the instance.

    Independent of this guarantee and unlike the function ID (FID) the UID may
    be the same in different partitions within the same machine. This allows to
    create PCI configurations in multiple partitions to be identical in the
    UID-namespace.

  - uid_is_unique:
    A 0 or 1 flag indicating whether the user-defined identifier (UID) is
    guaranteed to be and remain unique within this Linux instance. This
    platform feature is called UID Checking.

  - pfip/segmentX:
=======
* /sys/bus/pci/slots/XXXXXXXX

  The slot entries are set up using the function identifier (FID) of the
  PCI function. The format depicted as XXXXXXXX above is 8 hexadecimal digits
  with 0 padding and lower case hexadecimal digits.

  - /sys/bus/pci/slots/XXXXXXXX/power

  A physical function that currently supports a virtual function cannot be
  powered off until all virtual functions are removed with:
  echo 0 > /sys/bus/pci/devices/XXXX:XX:XX.X/sriov_numvf

* /sys/bus/pci/devices/XXXX:XX:XX.X/

  - function_id
    A zPCI function identifier that uniquely identifies the function in the Z server.

  - function_handle
    Low-level identifier used for a configured PCI function.
    It might be useful for debugging.

  - pchid
    Model-dependent location of the I/O adapter.

  - pfgid
    PCI function group ID, functions that share identical functionality
    use a common identifier.
    A PCI group defines interrupts, IOMMU, IOTLB, and DMA specifics.

  - vfn
    The virtual function number, from 1 to N for virtual functions,
    0 for physical functions.

  - pft
    The PCI function type

  - port
    The port corresponds to the physical port the function is attached to.
    It also gives an indication of the physical function a virtual function
    is attached to.

  - uid
    The user identifier (UID) may be defined as part of the machine
    configuration or the z/VM or KVM guest configuration. If the accompanying
    uid_is_unique attribute is 1 the platform guarantees that the UID is unique
    within that instance and no devices with the same UID can be attached
    during the lifetime of the system.

  - uid_is_unique
    Indicates whether the user identifier (UID) is guaranteed to be and remain
    unique within this Linux instance.

  - pfip/segmentX
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
    The segments determine the isolation of a function.
    They correspond to the physical path to the function.
    The more the segments are different, the more the functions are isolated.

<<<<<<< HEAD
  - fidparm:
    Contains an 8-bit-per-PCI function parameter field in hexadecimal provided
    by the platform. The meaning of this field is PCI function type specific.
    For NETH VFs a value of 0x01 indicates that the function supports
    promiscuous mode.

* /sys/firmware/clp/uid_checking:

  In addition to the per-device uid_is_unique attribute this presents a
  global indication of whether UID Checking is enabled. This allows users
  to check for UID Checking even when no PCI functions are configured.

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
Enumeration and hotplug
=======================

The PCI address consists of four parts: domain, bus, device and function,
<<<<<<< HEAD
and is of this form: DDDD:BB:dd.f.

* For a PCI function for which the platform does not expose the RID, the
  pci=norid kernel parameter is used, or a so-called isolated Virtual Function
  which does have RID information but is used without its parent Physical
  Function being part of the same PCI configuration:

  - There is only one function per domain.

  - The domain is set from the zPCI function's UID if UID Checking is on;
    otherwise the domain ID is generated dynamically and is not stable
    across reboots or hot plug.

* For a PCI function for which the platform exposes the RID and which
  is not an Isolated Virtual Function:

  - There is still only one bus per domain.

  - There can be up to 256 PCI functions per bus.

  - The domain part of the address of all functions within the same topology is
    that of the configured PCI function with the lowest devfn within that
    topology.

  - Virtual Functions generated by an SR-IOV capable Physical Function only
    become visible once SR-IOV is enabled.
=======
and is of this form: DDDD:BB:dd.f

* When not using multi-functions (norid is set, or the firmware does not
  support multi-functions):

  - There is only one function per domain.

  - The domain is set from the zPCI function's UID as defined during the
    LPAR creation.

* When using multi-functions (norid parameter is not set),
  zPCI functions are addressed differently:

  - There is still only one bus per domain.

  - There can be up to 256 functions per bus.

  - The domain part of the address of all functions for
    a multi-Function device is set from the zPCI function's UID as defined
    in the LPAR creation for the function zero.

  - New functions will only be ready for use after the function zero
    (the function with devfn 0) has been enumerated.
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
