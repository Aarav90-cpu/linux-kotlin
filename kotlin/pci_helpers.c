#include <linux/pci.h>
#include <linux/module.h>
#include "pci_helpers.h"

int kotlin_helper_pci_register_driver(struct pci_driver *driver)
{
	return pci_register_driver(driver);
}

void kotlin_helper_pci_unregister_driver(struct pci_driver *driver)
{
	pci_unregister_driver(driver);
}
