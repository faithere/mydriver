#ifndef _FC_1553_H_
#define _FC_1553_H_

#define FC1553_VENDOR 0x10ee
#define FC1553_DEVICE 0x7028

#ifndef FC1553_MAJOR
#define FC1553_MAJOR 250
#endif

#ifndef FC1553_MINOR
#define FC1553_MINOR 0
#endif

#ifndef OK
#define OK 0
#endif

struct pci_bar
{
  u32 res_start;
  u32 res_end;
  u32 res_length;
  u32 virt_res_start;
  u32 virt_res_end;
};

/*
struct buf_write
{
  int offset;
  int value;
};
*/

struct fc1553_pcie_card 
{
  struct pci_bar resource;
  
  u32 int_num;
  void __iomem *addr_map;
  u32 *bar0_start;
  u32 *mem_buf;
  
  struct pci_dev *pdev;
  struct semaphore sem;
  struct cdev cdev;
};

#endif
