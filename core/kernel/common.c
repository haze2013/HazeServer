#include "common.h"
char *replace(char *buf, int len, char org, char dest)
{
                char *p=buf, *end=buf+len;
                while(p<end){
                                if(*p == org){
                                                *p = dest;
                                }
                                p++;
                }
                return buf;
}

#define for_each_line(_start,_len,_split,_strp,_slen) \
        for(_slen=0,_strp=(replace((_start),_len,(_split),0x00));\
    (((_slen)=strlen(_strp))) && ((_strp)<((_start)+(_len)));\
    (_strp)+=((_slen)+1))

unsigned long  clear_and_return_cr0(void)
{
	unsigned long cr0 = 0;
	unsigned long ret;
#ifndef __64_BIT_
	asm volatile ("movq %%cr0, %%rax"
			: "=a"(cr0)
		     );
	ret = cr0;
	/* clear the 20 bit of CR0, a.k.a WP bit */
	cr0 &= 0xfffeffff;
	asm volatile ("movq %%rax, %%cr0"
			:
			: "a"(cr0)
		     );
#else
	asm volatile ("movl %%cr0, %%eax"
                        : "=a"(cr0)
                     );
        ret = cr0;
        /* clear the 20 bit of CR0, a.k.a WP bit */
        cr0 &= 0xfffeffff;
        asm volatile ("movl %%eax, %%cr0"
                        :
                        : "a"(cr0)
                     );
#endif
	return ret;
}

void  setback_cr0(unsigned long val)
{
#if 0
#ifndef __64_BIT__
        asm volatile ("movq %%rax, %%cr0"
                        :
                        : "a"(val)
                     );
#else
	asm volatile ("movl %%eax, %%cr0"
                        :
                        : "a"(val)
                     );

#endif
#endif
asm volatile ("movl %%eax, %%cr0"
                        :
                        : "a"(val)
                     );


}

/************************
 *if find sercurity hook 
 *	flag set 0
 *if find socket hook
 *	flag set 1
 ************************/
unsigned long find_symbol_addr(char *symname)
{
	int len;
	unsigned long ret = 0x0000UL;
	struct file *fp = NULL;
	char *p,*mem = NULL;
	int size,linelen;
	char *linep;
	loff_t back_pos;
	mm_segment_t oldfs;
	int namelen = 0;

	char path[128] = {0};
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 19)
	struct new_utsname *uts = init_utsname();
#else
	struct new_utsname *uts = utsname();
#endif
	sprintf(path, "/boot/System.map-%s",uts->release);
	printk("path[%s]\n",path);
	namelen = strlen(symname);
	oldfs = get_fs();
	set_fs(KERNEL_DS);
	fp = filp_open(path, O_RDONLY, 0);
	if(IS_ERR(fp)){
		printk("open file error\n");
		goto end;
	}
	size = fp->f_dentry->d_inode->i_size;
	if(size == 0){
		goto end;
	}
	back_pos = fp->f_pos;
	fp->f_pos = 0;
	mem = vmalloc(size);
	if(mem == NULL){
		printk("%s kmalloc %d\n",__FUNCTION__,size);
		goto end;
	}
	len = fp->f_op->read(fp, mem, size, &fp->f_pos);
	if(len != size){
		printk("%s:%d\n",__FUNCTION__,len);
		goto end;
	}
	for_each_line(mem, size, '\n', linep, linelen){
		p = linep;
		strsep(&p," ");
		strsep(&p," ");
		if(memcmp(p, symname, namelen) == 0){
			ret = simple_strtol(linep,NULL,16);
			goto end;
		}
	}
end:
	set_fs(oldfs);
	if(mem){
		vfree(mem);
	}
	if(fp){
		filp_close(fp, 0);
	}
	return ret;
}

void *find_struct_address(char *func_name ,int flag)
{
        ulong *p = NULL;
        ulong hook_address = 0x00UL;

        hook_address = find_symbol_addr(func_name);
        if(hook_address == 0x00UL || hook_address == (~0x00UL)){
                printk("[%s] hook address error\n",func_name);
                return NULL;
        }

        p = (ulong *)hook_address;

        if(flag){
                return p;
        }
        return (*p);
}
