/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LIBATA_TRANSPORT_H
#define _LIBATA_TRANSPORT_H


<<<<<<< HEAD
extern struct scsi_transport_template ata_scsi_transportt;
=======
extern struct scsi_transport_template *ata_scsi_transport_template;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

int ata_tlink_add(struct ata_link *link);
void ata_tlink_delete(struct ata_link *link);

<<<<<<< HEAD
=======
struct scsi_transport_template *ata_attach_transport(void);
void ata_release_transport(struct scsi_transport_template *t);

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
__init int libata_transport_init(void);
void __exit libata_transport_exit(void);
#endif
