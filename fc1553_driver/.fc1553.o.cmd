cmd_/home/wzl/driver_p5020/fc1553_pcie/mydriver/fc1553_driver/fc1553.o := powerpc64-fsl-linux-gcc -m64 -Wp,-MD,/home/wzl/driver_p5020/fc1553_pcie/mydriver/fc1553_driver/.fc1553.o.d  -nostdinc -isystem /opt/fsl/1.2/sysroots/i686-fslsdk-linux/usr/lib/ppc64e5500-fsl-linux/gcc/powerpc64-fsl-linux/4.6.2/include -I/home/wzl/linux-3.0.48/arch/powerpc/include -Iarch/powerpc/include/generated -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -Iarch/powerpc -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -msoft-float -pipe -Iarch/powerpc -mminimal-toc -mtraceback=no -mcall-aixdesc -mtune=power4 -mno-altivec -mno-spe -mspe=no -funit-at-a-time -fno-dwarf2-cfi-asm -mno-string -Wa,-me500 -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO  -DMODULE  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(fc1553)"  -D"KBUILD_MODNAME=KBUILD_STR(fc1553)" -c -o /home/wzl/driver_p5020/fc1553_pcie/mydriver/fc1553_driver/fc1553.o /home/wzl/driver_p5020/fc1553_pcie/mydriver/fc1553_driver/fc1553.c

source_/home/wzl/driver_p5020/fc1553_pcie/mydriver/fc1553_driver/fc1553.o := /home/wzl/driver_p5020/fc1553_pcie/mydriver/fc1553_driver/fc1553.c

deps_/home/wzl/driver_p5020/fc1553_pcie/mydriver/fc1553_driver/fc1553.o := \
  include/linux/module.h \
    $(wildcard include/config/symbol/prefix.h) \
    $(wildcard include/config/sysfs.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/debug/set/module/ronx.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/posix_types.h \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  include/linux/const.h \
  include/linux/stat.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/stat.h \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/compaction.h) \
  /opt/fsl/1.2/sysroots/i686-fslsdk-linux/usr/lib/ppc64e5500-fsl-linux/gcc/powerpc64-fsl-linux/4.6.2/include/stdarg.h \
  include/linux/linkage.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/linkage.h \
  include/linux/bitops.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/bitops.h \
    $(wildcard include/config/ppc64.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/asm-compat.h \
    $(wildcard include/config/power4/only.h) \
    $(wildcard include/config/ibm405/err77.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/ppc-opcode.h \
  include/linux/stringify.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/synch.h \
    $(wildcard include/config/ppc/e500mc.h) \
    $(wildcard include/config/e500.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/feature-fixups.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/find.h \
    $(wildcard include/config/generic/find/first/bit.h) \
  include/asm-generic/bitops/sched.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/typecheck.h \
  include/linux/printk.h \
    $(wildcard include/config/early/printk.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
  include/linux/init.h \
    $(wildcard include/config/hotplug.h) \
  include/linux/dynamic_debug.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/byteorder.h \
  include/linux/byteorder/big_endian.h \
  include/linux/swab.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/swab.h \
  include/linux/byteorder/generic.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
    $(wildcard include/config/preempt/rt/base.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/cache.h \
    $(wildcard include/config/8xx.h) \
    $(wildcard include/config/403gcx.h) \
    $(wildcard include/config/ppc32.h) \
    $(wildcard include/config/ppc/47x.h) \
  include/linux/seqlock.h \
    $(wildcard include/config/preempt/rt/full.h) \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/thread_info.h \
    $(wildcard include/config/ppc/256k/pages.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/processor.h \
    $(wildcard include/config/vsx.h) \
    $(wildcard include/config/ppc/prep.h) \
    $(wildcard include/config/task/size.h) \
    $(wildcard include/config/kernel/start.h) \
    $(wildcard include/config/booke.h) \
    $(wildcard include/config/ppc/adv/debug/regs.h) \
    $(wildcard include/config/ppc/adv/debug/iacs.h) \
    $(wildcard include/config/ppc/adv/debug/dvcs.h) \
    $(wildcard include/config/have/hw/breakpoint.h) \
    $(wildcard include/config/altivec.h) \
    $(wildcard include/config/spe.h) \
    $(wildcard include/config/kvm/book3s/32/handler.h) \
    $(wildcard include/config/kvm.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/reg.h \
    $(wildcard include/config/40x.h) \
    $(wildcard include/config/fsl/emb/perfmon.h) \
    $(wildcard include/config/ppc/book3s/64.h) \
    $(wildcard include/config/ppc/book3s/32.h) \
    $(wildcard include/config/6xx.h) \
    $(wildcard include/config/ppc/book3e/64.h) \
    $(wildcard include/config/e200.h) \
    $(wildcard include/config/ppc/cell.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/cputable.h \
    $(wildcard include/config/mpc10x/bridge.h) \
    $(wildcard include/config/ppc/83xx.h) \
    $(wildcard include/config/8260.h) \
    $(wildcard include/config/ppc/mpc52xx.h) \
    $(wildcard include/config/bdi/switch.h) \
    $(wildcard include/config/4xx.h) \
    $(wildcard include/config/power3.h) \
    $(wildcard include/config/power4.h) \
    $(wildcard include/config/ppc/book3e.h) \
    $(wildcard include/config/44x.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/reg_booke.h \
    $(wildcard include/config/debug/cw.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/reg_fsl_emb.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/ptrace.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/page.h \
    $(wildcard include/config/ppc/64k/pages.h) \
    $(wildcard include/config/ppc/16k/pages.h) \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/page/offset.h) \
    $(wildcard include/config/physical/start.h) \
    $(wildcard include/config/relocatable.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/ppc/std/mmu/64.h) \
    $(wildcard include/config/ppc/smlpar.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/kdump.h \
    $(wildcard include/config/crash/dump.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/page_64.h \
    $(wildcard include/config/ppc/mm/slices.h) \
  include/asm-generic/getorder.h \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/irqflags.h \
    $(wildcard include/config/ppc/lazy/ee.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/hw_irq.h \
  include/linux/errno.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/bottom_half.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/system.h \
    $(wildcard include/config/debugger.h) \
    $(wildcard include/config/kexec.h) \
    $(wildcard include/config/booke/wdt.h) \
  include/linux/spinlock_types.h \
  include/linux/spinlock_types_raw.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/spinlock_types.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/prove/rcu.h) \
  include/linux/spinlock_types_nort.h \
  include/linux/rwlock_types.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/spinlock.h \
    $(wildcard include/config/ppc/splpar.h) \
    $(wildcard include/config/ppc/iseries.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/paca.h \
    $(wildcard include/config/kvm/book3s/64/handler.h) \
    $(wildcard include/config/ppc/book3s.h) \
    $(wildcard include/config/kvm/book3s/handler.h) \
    $(wildcard include/config/kvm/book3s/pr.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/lppaca.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/mmu.h \
    $(wildcard include/config/ppc/fsl/book3e.h) \
    $(wildcard include/config/ppc/std/mmu/32.h) \
    $(wildcard include/config/ppc/book3e/mmu.h) \
    $(wildcard include/config/ppc/8xx.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/percpu.h \
  include/asm-generic/percpu.h \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/mmu-book3e.h \
    $(wildcard include/config/ppc/4k/pages.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/exception-64e.h \
    $(wildcard include/config/book3e/mmu/tlb/stats.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/hvcall.h \
    $(wildcard include/config/ppc/pseries.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/iseries/hv_call.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/iseries/hv_call_sc.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/iseries/hv_types.h \
  include/linux/rwlock.h \
  include/linux/spinlock_api_smp.h \
    $(wildcard include/config/inline/spin/lock.h) \
    $(wildcard include/config/inline/spin/lock/bh.h) \
    $(wildcard include/config/inline/spin/lock/irq.h) \
    $(wildcard include/config/inline/spin/lock/irqsave.h) \
    $(wildcard include/config/inline/spin/trylock.h) \
    $(wildcard include/config/inline/spin/trylock/bh.h) \
    $(wildcard include/config/inline/spin/unlock.h) \
    $(wildcard include/config/inline/spin/unlock/bh.h) \
    $(wildcard include/config/inline/spin/unlock/irq.h) \
    $(wildcard include/config/inline/spin/unlock/irqrestore.h) \
  include/linux/rwlock_api_smp.h \
    $(wildcard include/config/inline/read/lock.h) \
    $(wildcard include/config/inline/write/lock.h) \
    $(wildcard include/config/inline/read/lock/bh.h) \
    $(wildcard include/config/inline/write/lock/bh.h) \
    $(wildcard include/config/inline/read/lock/irq.h) \
    $(wildcard include/config/inline/write/lock/irq.h) \
    $(wildcard include/config/inline/read/lock/irqsave.h) \
    $(wildcard include/config/inline/write/lock/irqsave.h) \
    $(wildcard include/config/inline/read/trylock.h) \
    $(wildcard include/config/inline/write/trylock.h) \
    $(wildcard include/config/inline/read/unlock.h) \
    $(wildcard include/config/inline/write/unlock.h) \
    $(wildcard include/config/inline/read/unlock/bh.h) \
    $(wildcard include/config/inline/write/unlock/bh.h) \
    $(wildcard include/config/inline/read/unlock/irq.h) \
    $(wildcard include/config/inline/write/unlock/irq.h) \
    $(wildcard include/config/inline/read/unlock/irqrestore.h) \
    $(wildcard include/config/inline/write/unlock/irqrestore.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/atomic.h \
  include/asm-generic/atomic-long.h \
  include/linux/math64.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/div64.h \
  include/asm-generic/div64.h \
  include/linux/kmod.h \
  include/linux/gfp.h \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/arch/populates/node/map.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/cgroup/mem/res/ctlr.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/have/arch/pfn/valid.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  include/linux/wait.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/current.h \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/nodemask.h \
  include/linux/bitmap.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/string.h \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/generated/bounds.h \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/memory/hotremove.h) \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
  include/linux/notifier.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/have/arch/mutex/cpu/relax.h) \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/rwsem.h \
  include/linux/srcu.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
    $(wildcard include/config/sched/book.h) \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/smp.h \
    $(wildcard include/config/ppc/smp/muxed/ipi.h) \
  include/linux/irqreturn.h \
  include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
  include/linux/pfn.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/topology.h \
    $(wildcard include/config/pci.h) \
  include/asm-generic/topology.h \
  include/linux/mmdebug.h \
    $(wildcard include/config/debug/vm.h) \
    $(wildcard include/config/debug/virtual.h) \
  include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/jiffies.h \
  include/linux/timex.h \
  include/linux/param.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/param.h \
  include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/timex.h \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  include/linux/sysctl.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/rcu/torture/test.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/no/hz.h) \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/tiny/preempt/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
    $(wildcard include/config/preempt/rt.h) \
  include/linux/completion.h \
  include/linux/rcutree.h \
  include/linux/elf.h \
  include/linux/elf-em.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/elf.h \
    $(wildcard include/config/spu/base.h) \
  include/linux/sched.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/lockup/detector.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
    $(wildcard include/config/sched/autogroup.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/fanotify.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/cgroup/sched.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/rcu/boost.h) \
    $(wildcard include/config/compat/brk.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/wakeup/latency/hist.h) \
    $(wildcard include/config/missed/timer/offsets/hist.h) \
    $(wildcard include/config/have/unstable/sched/clock.h) \
    $(wildcard include/config/irq/time/accounting.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/debug/stack/usage.h) \
    $(wildcard include/config/mm/owner.h) \
  include/linux/capability.h \
  include/linux/rbtree.h \
  include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/want/page/debug/flags.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mmu/notifier.h) \
    $(wildcard include/config/transparent/hugepage.h) \
  include/linux/auxvec.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/auxvec.h \
  include/linux/prio_tree.h \
  include/linux/page-debug-flags.h \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/page/debug/something/else.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/kmap_types.h \
    $(wildcard include/config/debug/highmem.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/cputime.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/time.h \
    $(wildcard include/config/power.h) \
    $(wildcard include/config/8xx/cpu6.h) \
  include/linux/sem.h \
  include/linux/ipc.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/ipcbuf.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/sembuf.h \
  include/linux/signal.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/signal.h \
  include/asm-generic/signal-defs.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/siginfo.h \
  include/asm-generic/siginfo.h \
  include/linux/pid.h \
  include/linux/atomic.h \
    $(wildcard include/config/arch/has/atomic/or.h) \
  include/linux/proportions.h \
  include/linux/percpu_counter.h \
  include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/seccomp.h \
  include/linux/unistd.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/unistd.h \
  include/linux/rculist.h \
  include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  include/linux/resource.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/resource.h \
  include/asm-generic/resource.h \
  include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
    $(wildcard include/config/timerfd.h) \
  include/linux/timerqueue.h \
  include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  include/linux/latencytop.h \
  include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/user/ns.h) \
  include/linux/key.h \
    $(wildcard include/config/sysctl.h) \
  include/linux/selinux.h \
    $(wildcard include/config/security/selinux.h) \
  include/linux/hardirq.h \
  include/linux/ftrace_irq.h \
    $(wildcard include/config/ftrace/nmi/enter.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/hardirq.h \
  include/linux/irq.h \
    $(wildcard include/config/s390.h) \
    $(wildcard include/config/irq/release/method.h) \
    $(wildcard include/config/generic/pending/irq.h) \
  include/linux/irqnr.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/irq.h \
    $(wildcard include/config/nr/irqs.h) \
  include/linux/radix-tree.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/irq_regs.h \
  include/asm-generic/irq_regs.h \
  include/linux/irqdesc.h \
    $(wildcard include/config/irq/preflow/fasteoi.h) \
    $(wildcard include/config/sparse/irq.h) \
  include/linux/aio.h \
  include/linux/aio_abi.h \
  include/linux/uio.h \
  include/linux/kobject.h \
  include/linux/sysfs.h \
  include/linux/kobject_ns.h \
  include/linux/kref.h \
  include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
  include/linux/tracepoint.h \
  include/linux/jump_label.h \
    $(wildcard include/config/jump/label.h) \
    $(wildcard include/config/preempt/base.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/module.h \
    $(wildcard include/config/dynamic/ftrace.h) \
  include/trace/events/module.h \
  include/trace/define_trace.h \
  include/linux/pci.h \
    $(wildcard include/config/pci/iov.h) \
    $(wildcard include/config/pcieaspm.h) \
    $(wildcard include/config/pci/msi.h) \
    $(wildcard include/config/pcieportbus.h) \
    $(wildcard include/config/pcieaer.h) \
    $(wildcard include/config/pcie/ecrc.h) \
    $(wildcard include/config/ht/irq.h) \
    $(wildcard include/config/pci/domains.h) \
    $(wildcard include/config/pci/quirks.h) \
    $(wildcard include/config/pci/mmconfig.h) \
    $(wildcard include/config/hotplug/pci.h) \
  include/linux/pci_regs.h \
  include/linux/mod_devicetable.h \
  include/linux/ioport.h \
  include/linux/device.h \
    $(wildcard include/config/debug/devres.h) \
    $(wildcard include/config/devtmpfs.h) \
    $(wildcard include/config/sysfs/deprecated.h) \
  include/linux/klist.h \
  include/linux/pm.h \
    $(wildcard include/config/pm.h) \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/pm/runtime.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/device.h \
    $(wildcard include/config/swiotlb.h) \
  include/linux/pm_wakeup.h \
  include/linux/io.h \
    $(wildcard include/config/has/ioport.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/io.h \
    $(wildcard include/config/ra.h) \
    $(wildcard include/config/rd.h) \
    $(wildcard include/config/ppc/indirect/io.h) \
    $(wildcard include/config/eeh.h) \
    $(wildcard include/config/ppc/indirect/pio.h) \
    $(wildcard include/config/ppc/indirect/mmio.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/delay.h \
  include/asm-generic/iomap.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/io-defs.h \
  include/linux/pci_ids.h \
  include/linux/pci-dma.h \
  include/linux/dmapool.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/scatterlist.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/dma.h \
  include/asm-generic/scatterlist.h \
    $(wildcard include/config/debug/sg.h) \
    $(wildcard include/config/need/sg/dma/length.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/pci.h \
  include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/failslab.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/debug/slab.h) \
    $(wildcard include/config/slab.h) \
  include/linux/slab_def.h \
  include/trace/events/kmem.h \
  include/trace/events/gfpflags.h \
  include/linux/kmalloc_sizes.h \
  include/linux/dma-mapping.h \
    $(wildcard include/config/has/dma.h) \
    $(wildcard include/config/have/dma/attrs.h) \
    $(wildcard include/config/need/dma/map/state.h) \
  include/linux/err.h \
  include/linux/dma-attrs.h \
  include/linux/bug.h \
  include/linux/scatterlist.h \
  include/linux/mm.h \
    $(wildcard include/config/ksm.h) \
    $(wildcard include/config/debug/pagealloc.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/hugetlbfs.h) \
  include/linux/debug_locks.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  include/linux/range.h \
  include/linux/bit_spinlock.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/pgtable.h \
    $(wildcard include/config/pte/64bit.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/pgtable-ppc64.h \
    $(wildcard include/config/ppc/has/hash/64k.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/pgtable-ppc64-4k.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/pte-book3e.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/pte-common.h \
    $(wildcard include/config/ppc/std/mmu.h) \
    $(wildcard include/config/kgdb.h) \
    $(wildcard include/config/xmon.h) \
    $(wildcard include/config/kprobes.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/tlbflush.h \
    $(wildcard include/config/ppc/mmu/nohash.h) \
    $(wildcard include/config/fsl/booke.h) \
  include/asm-generic/pgtable.h \
  include/linux/page-flags.h \
    $(wildcard include/config/pageflags/extended.h) \
    $(wildcard include/config/arch/uses/pg/uncached.h) \
    $(wildcard include/config/memory/failure.h) \
    $(wildcard include/config/swap.h) \
  include/linux/huge_mm.h \
  include/linux/vmstat.h \
    $(wildcard include/config/vm/event/counters.h) \
  include/linux/vm_event_item.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/dma-mapping.h \
    $(wildcard include/config/not/coherent/cache.h) \
  include/linux/dma-debug.h \
    $(wildcard include/config/dma/api/debug.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/swiotlb.h \
  include/linux/swiotlb.h \
  include/asm-generic/dma-mapping-common.h \
  include/linux/kmemcheck.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/machdep.h \
    $(wildcard include/config/ppc/has/feature/calls.h) \
    $(wildcard include/config/suspend.h) \
    $(wildcard include/config/arch/cpu/probe/release.h) \
    $(wildcard include/config/ppc/pmac.h) \
  include/linux/seq_file.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/setup.h \
  include/asm-generic/setup.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/prom.h \
  include/linux/of.h \
    $(wildcard include/config/sparc.h) \
    $(wildcard include/config/of.h) \
    $(wildcard include/config/of/dynamic.h) \
  include/linux/of_fdt.h \
    $(wildcard include/config/of/flattree.h) \
    $(wildcard include/config/blk/dev/initrd.h) \
  include/linux/of_address.h \
  include/linux/of_irq.h \
    $(wildcard include/config/of/irq.h) \
  include/linux/platform_device.h \
    $(wildcard include/config/hibernate/callbacks.h) \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/pci-bridge.h \
  include/asm-generic/pci-dma-compat.h \
  include/linux/fs.h \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/ima.h) \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/debug/writecount.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/fs/xip.h) \
    $(wildcard include/config/migration.h) \
  include/linux/limits.h \
  include/linux/ioctl.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/ioctl.h \
  include/asm-generic/ioctl.h \
  include/linux/blk_types.h \
    $(wildcard include/config/blk/dev/integrity.h) \
  include/linux/kdev_t.h \
  include/linux/dcache.h \
  include/linux/rculist_bl.h \
  include/linux/list_bl.h \
  include/linux/path.h \
  include/linux/semaphore.h \
  include/linux/fiemap.h \
  include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  include/linux/dqblk_xfs.h \
  include/linux/dqblk_v1.h \
  include/linux/dqblk_v2.h \
  include/linux/dqblk_qtree.h \
  include/linux/nfs_fs_i.h \
  include/linux/nfs.h \
  include/linux/sunrpc/msg_prot.h \
  include/linux/inet.h \
  include/linux/fcntl.h \
  /home/wzl/linux-3.0.48/arch/powerpc/include/asm/fcntl.h \
  include/asm-generic/fcntl.h \
  include/linux/cdev.h \
  include/../arch/powerpc/include/asm/uaccess.h \
  include/linux/version.h \
  /home/wzl/driver_p5020/fc1553_pcie/mydriver/fc1553_driver/fc1553.h \
  /home/wzl/driver_p5020/fc1553_pcie/mydriver/fc1553_driver/xdma_hw.h \

/home/wzl/driver_p5020/fc1553_pcie/mydriver/fc1553_driver/fc1553.o: $(deps_/home/wzl/driver_p5020/fc1553_pcie/mydriver/fc1553_driver/fc1553.o)

$(deps_/home/wzl/driver_p5020/fc1553_pcie/mydriver/fc1553_driver/fc1553.o):
