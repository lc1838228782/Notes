## 延迟绑定

`call write@plt`

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/write_plt.png)

跳到got中保存的地址，第一次调用时，got中存储的是上图第二条指令地址。将write函数在rel.plt段中的偏移push，跳转到plt[0]。

再把`link_map=(GOT+4)`（即GOT[1]，链接器的标识信息）作为参数推入栈中，而(GOT+8)（即GOT[2]，动态链接器中的入口点）中保存的是`_dl_runtime_resolve`函数的地址。因此以上指令相当于执行了`_dl_runtime_resolve(link_map, reloc_arg)`，该函数会完成符号的解析，即将真实的write函数地址写入其GOT条目中，随后把控制权交给write函数。

## 32位

```c
typedef struct {
    Elf32_Addr r_offset;    // 对于可执行文件，此值为虚拟地址
    Elf32_Word r_info;      // 符号表索引
} Elf32_Rel;

#define ELF32_R_SYM(info) ((info)>>8)
#define ELF32_R_TYPE(info) ((unsigned char)(info))
#define ELF32_R_INFO(sym, type) (((sym)<<8)+(unsigned char)(type))
```

构造的`Elf32_Rel`地址减去`rel.plt`首地址作为`reloc_arg`。（即`write@plt`的第二条指令的push操作）

`r_offset`为write函数的got地址，即要写入新地址的地址。

`r_info`中存储了Elf32_Sym的index，和类型信息(低八位)。`index = (fake_sym_addr-dynsym) / 0x10`

```c
typedef struct
{
    Elf32_Word st_name;     // Symbol name(string tbl index)
    Elf32_Addr st_value;    // Symbol value
    Elf32_Word st_size;     // Symbol size
    unsigned char st_info;  // Symbol type and binding
    unsigned char st_other; // Symbol visibility under glibc>=2.2
    Elf32_Section st_shndx; // Section index
} Elf32_Sym;
```

`st_name`存储了构造字符串的偏移，`(fake_string_addr - dynstr)`。构造`Elf32_Sym`需要0x10对齐。

## 攻击步骤

1. 首先存放**函数字符串**，计算**dynstr**与**函数字符串的地址**的偏移。
2. 构造fake_sym，st_name存放上述偏移。fake_sym的存放位置需要0x10对齐。计算dynsym与构造sym的偏移，除以0x10，计算索引。
3. 将索引<<8，或上类型存放到r_info。构造fake_reloc。r_offset为函数的got地址。
4. 计算fake_reloc与rel.plt的偏移，作为plt0的第二个参数。（相当于在一般函数plt中，第二条指令所push的参数）