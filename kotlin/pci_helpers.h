#ifndef _KOTLIN_PCI_HELPERS_H
#define _KOTLIN_PCI_HELPERS_H

#include <linux/pci.h>

int kotlin_helper_pci_register_driver(struct pci_driver *driver);
void kotlin_helper_pci_unregister_driver(struct pci_driver *driver);

#endif
