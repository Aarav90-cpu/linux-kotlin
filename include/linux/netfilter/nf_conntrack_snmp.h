/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NF_CONNTRACK_SNMP_H
#define _NF_CONNTRACK_SNMP_H

#include <linux/netfilter.h>
#include <linux/skbuff.h>

<<<<<<< HEAD
typedef int
nf_nat_snmp_hook_fn(struct sk_buff *skb,
		    unsigned int protoff,
		    struct nf_conn *ct,
		    enum ip_conntrack_info ctinfo);

extern nf_nat_snmp_hook_fn __rcu *nf_nat_snmp_hook;
=======
extern int (__rcu *nf_nat_snmp_hook)(struct sk_buff *skb,
				unsigned int protoff,
				struct nf_conn *ct,
				enum ip_conntrack_info ctinfo);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

#endif /* _NF_CONNTRACK_SNMP_H */
