#include <linux/dcache.h>
#include <linux/fs.h>
#include <linux/mount.h>
#include <linux/sched.h>
#include "haze_path.h"
/*
* Get full path by struct file in <linux/fs.h>
* Author :Shenjunwei
*/
char *sec_file_path(struct file *file, char *buf, int len)
{
	char *p = NULL;

	p = d_path(&(file->f_path), buf, len);
	if(IS_ERR(p)){
		return NULL;
	}
	return p;
}
/*
 * Get full path by struct task_struct in <linux/sched.h>
 * Author:Shenjunwei
 */
char *sec_task_path(struct task_struct *task, char *buf, int len)
{
	char *p = NULL;
	struct mm_struct *mm = task->mm;

	if(mm == NULL)
		return NULL;
	down_read(&(mm->mmap_sem));
	if(mm->exe_file != NULL){
		p = sec_file_path(mm->exe_file, buf, len);
	}
	up_read(&(mm->mmap_sem));
	return p;
}
