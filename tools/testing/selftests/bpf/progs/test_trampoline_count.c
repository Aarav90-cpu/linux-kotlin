// SPDX-License-Identifier: GPL-2.0
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

<<<<<<< HEAD
SEC("fentry/bpf_testmod_trampoline_count_test")
int BPF_PROG(fentry_test)
=======
SEC("fentry/bpf_modify_return_test")
int BPF_PROG(fentry_test, int a, int *b)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	return 0;
}

<<<<<<< HEAD
SEC("fmod_ret/bpf_testmod_trampoline_count_test")
int BPF_PROG(fmod_ret_test, int ret)
=======
SEC("fmod_ret/bpf_modify_return_test")
int BPF_PROG(fmod_ret_test, int a, int *b, int ret)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	return 0;
}

<<<<<<< HEAD
SEC("fexit/bpf_testmod_trampoline_count_test")
int BPF_PROG(fexit_test, int ret)
=======
SEC("fexit/bpf_modify_return_test")
int BPF_PROG(fexit_test, int a, int *b, int ret)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	return 0;
}

char _license[] SEC("license") = "GPL";
