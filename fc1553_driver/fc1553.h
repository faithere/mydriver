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

#define DMA_BD_SW_NUM_WORDS 16
#define MAX_DMA_ENGINES 64
#define MAX_BARS 6

/** Engine bitmask is 64-bit because there are 64 engines */
#define DMA_ENGINE_PER_SIZE     0x100                                      /*  Separation between engine regs */
#define DMA_OFFSET              0                                          /*  Starting register offset */
#define DMA_SIZE                (MAX_DMA_ENGINES * DMA_ENGINE_PER_SIZE)    /*  Size of DMA engine reg space */

typedef u32 Dma_Bd[DMA_BD_SW_NUM_WORDS];

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

typedef struct {
  u32 ChanBase;
  u32 IsRxChannel;        /**< Is this a receive channel ? */
  u32 RunState;           /**< Flag to indicate state of engine/ring */
  u32 FirstBdPhysAddr;    /**< Physical address of 1st BD in list */
  u32 FirstBdAddr;        /**< Virtual address of 1st BD in list */
  u32 LastBdAddr;         /**< Virtual address of last BD in the list */
  u32 Length;             /**< Total size of ring in bytes */
  u32 Separation;         /**< Number of bytes between the starting
                               address of adjacent BDs */
  Dma_Bd *FreeHead;       /**< First BD in the free group */
  Dma_Bd *PreHead;        /**< First BD in the pre-work group */
  Dma_Bd *HwHead;         /**< First BD in the work group */
  Dma_Bd *HwTail;         /**< Last BD in the work group */
  Dma_Bd *PostHead;       /**< First BD in the post-work group */
  Dma_Bd *BdaRestart;     /**< BD to load when channel is started */
  u32 FreeCnt;            /**< Number of allocatable BDs in free group */
  u32 PreCnt;             /**< Number of BDs in pre-work group */
  u32 HwCnt;              /**< Number of BDs in work group */
  u32 PostCnt;            /**< Number of BDs in post-work group */
  u32 AllCnt;             /**< Total Number of BDs for channel */

  u32 BDerrs;           /**< Total BD errors reported by DMA */
  u32 BDSerrs;          /**< Total TX BD short errors reported by DMA */
} Dma_BdRing;

typedef struct {
    struct pci_dev * pdev;  /**< PCIe Device handle */
    u32 RegBase;            /**< Virtual base address of DMA engine */

    u32 EngineState;        /**< State of the DMA engine */
    Dma_BdRing BdRing;      /**< BD container for DMA engine */
    u32 Type;               /**< Type of DMA engine - C2S or S2C */
    //UserPtrs user;          /**< User callback functions */
    int pktSize;            /**< User-specified usual size of packets */

#ifdef TH_BH_ISR
    int intrCount;          /**< Counter to control interrupt coalescing */
#endif

    dma_addr_t descSpacePA; /**< Physical address of BD space */
    u32 descSpaceSize;      /**< Size of BD space in bytes */
    u32 * descSpaceVA;      /**< Virtual address of BD space */
    u32 delta;              /**< Shift from original start for alignment */
} Dma_Engine;

struct privData
{
  struct pci_dev *pdev;
  u32 barMask;
  struct {
    unsigned long basePAddr;
    unsigned long baseLen;
    void __iomem *baseVAddr;
  }barInfo[MAX_BARS];
  u32 index;
  long long engineMask;
  Dma_Engine Dma[MAX_DMA_ENGINES];
  int userCount;
};

typedef struct
{
  unsigned char *pktBuf;
  unsigned char *bufInfo;
  unsigned int size;
  unsigned int flag;
  unsigned long long userInfo;
}PktBuf;

struct PktPool
{
  PktBuf *pbuf;
  struct PktPool *next;
};
/*
 *DMA Engine Driver Status
 */
#define UNINITIALIZED   0       /* Not yet come up */
#define INITIALIZED     1       /* But not yet set up for polling */
#define POLLING         2       /* But not yet registered with DMA */
#define REGISTERED      3       /* Registered with DMA */
#define CLOSED          4       /* Driver is being brought down */
/*
 *END DMA Engine Driver Status
 */

/*
 *COMMON DEFINATION
 */
#define XST_SUCCESS                     0L
#define XST_FAILURE                     1L
#define XST_DEVICE_NOT_FOUND            2L
#define XST_DEVICE_BLOCK_NOT_FOUND      3L
#define XST_INVALID_VERSION             4L
#define XST_DEVICE_IS_STARTED           5L
#define XST_DEVICE_IS_STOPPED           6L
#define XST_FIFO_ERROR                  7L  
/*
 *END COMMON DEFINATION
 */

/*
 *Debug Switch for Output
 */
#ifndef DEBUG_VERBOSE
#define DEBUG_VERBOSE
#endif

#ifndef DEBUG_NORMAL
#define DEBUG_NORMAL
#endif

#ifdef DEBUG_VERBOSE
#define log_verbose(args...) printk(args)
#define log_normal(args...) printk(args)
#elif defined DEBUG_NORMAL
#define log_verbose(x...)
#define log_normal(args...) printk(args)
#else
#define log_verbose(x...)
#define log_normal(x...)
#endif

/*******************DMA CONTROLLER REGISTER******************/
#define XIo_In32(addr)      (ioread32((unsigned char *)(addr)))
#define XIo_Out32(addr, data) (iowrite32((data), (unsigned char *)(addr)))

#endif
