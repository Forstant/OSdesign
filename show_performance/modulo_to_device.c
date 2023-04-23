/*
* chardev.c: Creates a read-only char device that says how many times
* you have read from the dev file
*/
#include <linux/atomic.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/timex.h>
#include <linux/fs.h>
#include <linux/jiffies.h>
#include <linux/init.h>
#include <linux/kernel.h> /* for sprintf() */
#include <linux/module.h>
#include <linux/fs.h>
// #include <strto.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/uaccess.h> /* for get_user and put_user */
#include <linux/string.h>
#include <asm/errno.h>
#include "process_monitor_list.h"
#include <linux/file.h>
#include <linux/fdtable.h>
#include <net/inet_ecn.h>
#include <linux/socket.h>
#include <linux/in6.h>

#define MAX_SIZE 100
//#define FILE_NAME "/home/tensinh/os_project/target.txt"
// char *kernel_fget(char* buf, int max_size, struct file *fp);

#define SUCCESS 0
#define DEVICE_NAME "chardev" /* Dev name as it appears in /proc/devices */
#define BUF_LEN 80 /* Max length of the message from the device */
/* Global variables are declared as static, so are global within the file. */
struct process_monitor *head = NULL;  // 头节点





// char *file_get(char* buffer, int max_size, struct file *fp)
// {
//   int i = 0;
//   int size;  
//   if (0 > max_size)
//   {
//     printk(KERN_EMERG "param max_size is error\n");
//     return NULL;
//   }
//   size = vfs_read(fp, buffer, max_size, &(fp->f_pos));
//   if (1 > size)
//   {
//     printk(KERN_EMERG "file is error\n");
//     return NULL;
//   }
//   while (buffer[i++] != '\n' && i < size);
//   buffer[i - 1] = '\0';  
//   fp->f_pos += i - size;  
//   return buffer;
// }




// int readfile(char* FILE_NAME)
// {
//   printk(KERN_INFO "****FILE_NAME****:%s\n" ,FILE_NAME);
//   char buffer[MAX_SIZE]; 
//   struct file *fp;
//   mm_segment_t fs;     
//   fp = filp_open(FILE_NAME, O_RDWR, 0);
//   if (IS_ERR(fp)) 
//   {
//     printk(KERN_EMERG "open file error\n");
//     return -1;
//   }
//   printk(KERN_INFO "****SUCCESSFUL OPEN****:%s\n" ,FILE_NAME);
//   // fs = get_fs();
//   // set_fs(KERNEL_DS); 
//   // memset(buffer, 0, sizeof(buffer));
//   // if (NULL == file_get(buffer, MAX_SIZE, fp))
//   // {
//   //   printk(KERN_EMERG "file_get error\n");
//   //   filp_close(fp, NULL);
//   // }
//   // printk(KERN_INFO "SUCCESSFUL buffer%s", buffer);
//   // filp_close(fp, NULL);
//   // set_fs(fs); 
//   char* const delim=" ";
//   char* token,*cur = buffer;
//   char buf[MAX_SIZE][MAX_SIZE];
//   memset(buf, 0, sizeof(buf));
//   int k=0;
//   while (token = strsep(&cur,delim))
//   {
//     printk(KERN_INFO "*******\n");
//     strcpy(buf[k],token);
//     printk(KERN_INFO "***%s\n" ,buf[k]);
//     k+=1;
//   }
//   char utime[MAX_SIZE];
//   char stime[MAX_SIZE];
//   char start_time[MAX_SIZE];
//   memset(utime, 0, sizeof(utime));
//   memset(stime, 0, sizeof(stime));
//   memset(start_time, 0, sizeof(start_time));
//   char* const delim1=" ";
//   char* token1,*cur1 = buf[0];
//   char content1[MAX_SIZE];
//   int count1=1;
//   memset(content1, 0, sizeof(content1));
//   while (token1 = strsep(&cur1,delim1)){
// //   {  if(count1==14)
// // 	{
// // 	    strcpy(utime,token1);
// // 	}
// //     if(count1==15)
// // 	{
// // 	    strcpy(stime,token1);
// // 	}
// // if(count1==22)
// // 	{
// // 	    strcpy(start_time,token1);
// // 	}
// //   if(count1>22) break;
// //     count1=count1+1;
//     printk(KERN_INFO "****count%d****:%s\n" ,count1,token1);
//     count1++;
//   } 
  
//   // strcpy(total_file,content3);
//   // printk(KERN_INFO "****utime****:%s\n" ,utime);
//   // printk(KERN_INFO "****stime****:%s\n" ,stime);
//   // printk(KERN_INFO "****start_time****:%s\n" ,start_time);
 
  
//   return 0;
// }

unsigned long bing_read_now(void)
{
	struct file *f;
	char buf[128];
	mm_segment_t fs;
	int i;
	static  long now_time;
	/* Open /proc/pid/stat */
	sprintf(buf, "/proc/uptime");
	f = filp_open(buf, O_RDONLY, 0);
	if (f == NULL) 
	{
	    // printk(KERN_INFO "filp_open error!!.\n");
	    return -1;
	}
	// printk(KERN_INFO "filp_open success!!.\n");
	/* Read /proc/pid/stat */
	fs = get_fs();
	// set_fs(get_ds());
	set_fs(KERNEL_DS);
	i = f->f_op->read(f, buf, 128, &f->f_pos);
	set_fs(fs);

	/* Close /proc/pid/stat */
	filp_close(f, NULL);

	if (i <= 0) {
	    // printk(KERN_INFO "Read error.\n");
	    return -1;
	}
	// printk(KERN_INFO "Read now SUCCESS.\n");
	buf[i] = '\0';
	// printk(KERN_ALERT "success buffer%s.\n",buf);
	char* token1,*cur1 = buf;
	   int count1=1;
	unsigned long zhengshu,xiaoshu;
	    // const char *tmp = to;
	while (token1 = strsep(&cur1," ")){
	  if(count1==1){
	    int count2=1;
	    char* token2,*cur1 = token1;
	    while (token2 = strsep(&cur1,".")){
	      if(count2==1){
		kstrtol(token2,10,&zhengshu);
	      }
	      ifsi(count2==2){
		kstrtol(token2,10,&xiaoshu);
	      }
	      count2++;
	    }
	    // printk(KERN_INFO "%d****utime****:%lu\n" ,pid,utime);
	  }
	    count1++;
	  } 
	  now_time=zhengshu*100+xiaoshu;
	  return now_time;
}

/*
void bing_read_proc(int pid)
{
	struct file *f;
	char buf[128];
	mm_segment_t fs;
	int i;
	static  long now_time, virtual_mem_usage, physical_mem_usage;
	// Open /proc/pid/stat //
	sprintf(buf, "/proc/%d/stat", pid);
	f = filp_open(buf, O_RDONLY, 0);
	if (f == NULL) {
	    printk(KERN_INFO "filp_open error!!.\n");
	    return -1;
	}
	printk(KERN_INFO "filp_open success!!.\n");
	// Read /proc/pid/stat //
	fs = get_fs();
	// set_fs(get_ds());
	set_fs(KERNEL_DS);
	i = f->f_op->read(f, buf, 128, &f->f_pos);
	set_fs(fs);

	// Close /proc/pid/stat //
	filp_close(f, NULL);

	if (i <= 0) {
	    printk(KERN_INFO "Read error.\n");
	    return -1;
	}
	printk(KERN_INFO "Read SUCCESS.\n");
	buf[i] = '\0';
	printk(KERN_ALERT "success buffer%s.\n",buf);
	char* token1,*cur1 = buf;
	 char* const delim1=" ";
	   int count1=1;
	    // const char *tmp = to;
	while (token1 = strsep(&cur1,delim1)){
	  if(count1==14){
	 
	    kstrtol(token1,10,&utime_s);
	    printk(KERN_INFO "%d****utime****:%lu\n" ,pid,utime_s);
	  }
	  if(count1==15){
	   
	    kstrtol(token1,10,&stime_s);
	    printk(KERN_INFO "%d****stime****:%lu\n",pid ,stime_s);
	  }
	   if(count1==22){
	    // kstrtol(token1,10,&start_time);
		start_time = bing_read_now();
	    printk(KERN_INFO "%d****start_time****:%lu\n",pid ,start_time);
		now_time=bing_read_now();
	  printk(KERN_INFO "%d****now_time****:%lu\n" ,pid,now_time);
	// long cpu_usage=(utime+stime)*1000/(now_time-start_time);
		  // printk(KERN_INFO "%d****cpu_usage****:%lu/%lu=%lu%%%%\n",pid,(utime+stime),(now_time-		start_time),cpu_usage);
	  }
		if(count1==23){
	    kstrtol(token1,10,&virtual_mem_usage);
	    printk(KERN_INFO "%d****virtual_mem_usage****:%lu\n",pid , virtual_mem_usage);
	  }
		if(count1==24){
	    kstrtol(token1,10,&physical_mem_usage);
	    printk(KERN_INFO "%d****physical_mem_usage****:%lu\n",pid , physical_mem_usage);
	  }
		
	  
	    // printk(KERN_INFO "****count%d****:%s\n" ,count1,token1);
	    count1++;
	  } 
}
*/



/* Prototypes - this would normally go in a .h file */
static int counter = 0;
static int last_pid;
static unsigned long last_sum,last_starttime;
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char __user *, size_t,
loff_t *);


static int major; /* major number assigned to our device driver */
enum {
CDEV_NOT_USED = 0,
CDEV_EXCLUSIVE_OPEN = 1,
};


/* Is device open? Used to prevent multiple access to device */
static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED);
static char msg[BUF_LEN + 1]; /* The msg the device will give when asked */
static struct class *cls;
static struct file_operations chardev_fops = {
.read = device_read,
.write = device_write,
.open = device_open,
.release = device_release,
};
static unsigned long last_time;
static int cpu_mem_usage(int pid)
{
    // static unsigned long now_time, utime,  stime, start_time,sum;
    static struct task_struct *task;
    static char FILENAME[MAX_SIZE];
    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (task == NULL) {
        printk(KERN_ALERT "无法找到PID为%d的进程\\n", pid);
        return -1;
    }

    // sprintf(FILENAME,"/proc/%d/stat",pid);
    // readfile(FILENAME);

	// bing_read_proc(pid);

//     // now_time =jiffies;  // 开始时间
//     // utime= cp(task->utime);
//     // stime = task->stime;
//     start_time=task->start_time;
    
//     // schedule_timeout_interruptible(1* HZ); // 停止一秒以计算占用率

//     // jiffies_end = get_jiffies_64();     // 结束的时间  
//     // utime_end = task->utime;
//     // stime_end = task->stime;
//     sum=task->se.sum_exec_runtime;
	unsigned long start_time, utime_s, stime_s;
	// bing_read_proc(pid, start_time, utime_s, stime_s);
	start_time = bing_read_now();
	unsigned long jiffies_start = get_jiffies_64();
	printk(KERN_INFO "%d****start_time****:%lu\n" ,pid, start_time);
	struct file *f;
	char buf[128];
	mm_segment_t fs;
	int i;
	static  long now_time, virtual_mem_usage, physical_mem_usage;
	/* Open /proc/pid/stat */
	sprintf(buf, "/proc/%d/stat", pid);
	f = filp_open(buf, O_RDONLY, 0);
	if (f == NULL) {
	    printk(KERN_INFO "filp_open error!!.\n");
	    return -1;
	}
	// printk(KERN_INFO "filp_open success!!.\n");
	/* Read /proc/pid/stat */
	fs = get_fs();
	// set_fs(get_ds());
	set_fs(KERNEL_DS);
	i = f->f_op->read(f, buf, 128, &f->f_pos);
	set_fs(fs);
	/* Close /proc/pid/stat */
	filp_close(f, NULL);
	if (i <= 0) {
	    printk(KERN_INFO "Read error.\n");
	    return -1;
	}
	// printk(KERN_INFO "Read SUCCESS.\n");
	buf[i] = '\0';
	// printk(KERN_ALERT "success buffer%s.\n",buf);
	char* token1,*cur1 = buf;
	 char* const delim1=" ";
	   int count1=1;
	    // const char *tmp = to;
	while (token1 = strsep(&cur1,delim1)){
	  if(count1==14){
	 
	    kstrtol(token1,10,&utime_s);
	    // printk(KERN_INFO "%d****utime_s****:%lu\n" ,pid,utime_s);
	  }
	  if(count1==15){
	   
	    kstrtol(token1,10,&stime_s);
	    // printk(KERN_INFO "%d****stime_s****:%lu\n" ,pid,utime_s);
	  }
	    // printk(KERN_INFO "****count%d****:%s\n" ,count1,token1);
	    count1++;
	  } 
	struct process_monitor *node = new_process_monitor(pid, jiffies_start, task->utime/8000000, task->stime/8000000, start_time, utime_s, stime_s);
    	insert_process_monitor(&head, node);
	printk(KERN_INFO "进程的开始时间，开始用户态时间和内核态时间分别是：%lu, %lu, %lu\n", node->jiffies_start,  node->utime_start, node->stime_start);
	printk(KERN_INFO "进程的开始时间，开始用户态时间和内核态时间分别是：%lu, %lu, %lu\n", node->start_time,  node->utime_s, node->stime_s);
    
//     // unsigned long cpu_usage = ((utime_end + stime_end) - (utime_start + stime_start)) * 100 / (jiffies_end - jiffies_start);
//     double cpu_usage = (utime+stime)/(now_time-start_time)* 100 ;
//     unsigned long mem_usage = task->mm->total_vm * 4;
//     // printk(KERN_INFO "进程%d,的CPU占用率为%lu\n", pid, cpu_usage);
//     // printk(KERN_INFO "进程%d,的虚拟内存大小为%lu KB\n", pid, mem_usage);
//     // sprintf(msg,"进程%d,utime_start%lu,utime_end%lu,stime_start%lu,stime_end%lu,jiffies_start%lu,jiffies_end%lu,jiffies_diff%lu\n", pid, utime_start,utime_end,stime_start,stime_end,jiffies_start,jiffies_end,jiffies_end-jiffies_start);
//     if(counter==1){
//     // sprintf(msg,"process %d utime %lu stime %lu start_time%lu now_time%lu sum%lu \n",pid,utime,stime,start_time,now_time,sum);
//     }
//     else{
//         unsigned long cpu_usage = (last_sum)* 1000/(start_time-last_starttime);

// // sprintf(msg,"process %d utime %lu stime %lu start_time%lu now_time%lu sum%lu \n",pid,utime,stime,start_time,now_time,sum);
// sprintf(msg,"last process %d last_sum %lu diff %lu cpu_usage %lu%%%%\n",last_pid,last_sum,start_time-last_starttime,cpu_usage);
//     }
//     last_pid=pid;
//     last_sum=sum;
//     last_starttime=start_time;
	
	// 获取进程的文件描述符表
	struct fdtable *fdt = files_fdtable(task->files);
	// char buf1[128];
	// 遍历文件描述符表
	unsigned int j = 0;
	printk(KERN_INFO "最大%u个文件指针\n", fdt->max_fds);
	for (j = 0; j < fdt->max_fds; j++) {
    	// 获取文件指针
    	struct file *file = fdt->fd[j];
		// printk(KERN_INFO "第%u个文件指针\n", j);
    	// 判断文件是否为socket
    	if (file && S_ISSOCK(file->f_path.dentry->d_inode->i_mode)) {
			printk(KERN_INFO "这个文件是socket文件");
        	// 获取socks结构体
			int err;
			struct socket *sock = sock_from_file(file, &err);
        	struct sock *sk = sock->sk;
        	// 判断协议类型
        	if (sk->sk_family == AF_INET) { // IPv4
            	// 获取ip地址和端口号（网络字节序）
            	__be32 src_ip = inet_sk(sk)->inet_saddr;
            	__be32 dst_ip = inet_sk(sk)->inet_daddr;
            	__be16 src_port = inet_sk(sk)->inet_sport;
            	__be16 dst_port = inet_sk(sk)->inet_dport;
            	// 转换为主机字节序并打印
            	printk(KERN_INFO "IPv4: %pI4:%d -> %pI4:%d\n", &src_ip, ntohs(src_port), &dst_ip, ntohs(dst_port));
        	} 
			else if (sk->sk_family == AF_INET6) { // IPv6
            	// 获取ip地址和端口号（网络字节序）
           		struct in6_addr src_ip = inet6_sk(sk)->saddr;
            	struct in6_addr dst_ip = sk->sk_v6_daddr;
            	__be16 src_port = inet_sk(sk)->inet_sport;
            	__be16 dst_port = inet_sk(sk)->inet_dport;
            	// 转换为主机字节序并打印
            	printk(KERN_INFO "IPv6: %pI6:%d -> %pI6:%d\n", &src_ip, ntohs(src_port), &dst_ip, ntohs(dst_port));
        	}
    	}
		else{
			printk(KERN_INFO "这不是一个socket文件\n");
		}
	}
	return 0;
}


static int __init chardev_init(void)
{
	major = register_chrdev(0, DEVICE_NAME, &chardev_fops);
	if (major < 0) {
	pr_alert("Registering char device failed with %d\n", major);
	return major;
	}
	pr_info("I was assigned major number %d.\n", major);
	cls = class_create(THIS_MODULE, DEVICE_NAME);
	device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
	pr_info("Device created on /dev/%s\n", DEVICE_NAME);
	return SUCCESS;
}
static void __exit chardev_exit(void)
{
	device_destroy(cls, MKDEV(major, 0));
	class_destroy(cls);
	/* Unregister the device */
	unregister_chrdev(major, DEVICE_NAME);
}
/* Methods */
/* Called when a process tries to open the device file, like
* "sudo cat /dev/chardev"
*/
static int device_open(struct inode *inode, struct file *file)
{

	if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN))
	return -EBUSY;
	int pid_n = pid_nr(task_pid(current));
	printk(KERN_INFO "%d times pid is %d\n", ++counter,pid_n);

	// char str2[20];
	// sprintf(str2, "%d", pid_n);
	// char str1[100] = "top -p ";
	// char str3[100] = "  | tee /home/kyrie/kernel-learn/proc_learn/test.txt";
	// strcat(str1,str2);
	// strcat(str1,str3);

	//use top command
	//     char *argv[] = { "/bin/bash", "-c", "top -b -n 2  | tee /home/kyrie/kernel-learn/proc_learn/test.txt", NULL };
	// // char *argv[] = { "/bin/bash", "-c", str1, NULL };
	//     char *envp[] = { "HOME=/", "TERM=linux", "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };
	//     int ret;

	//     printk(KERN_INFO "Starting module\n");

	//     ret = call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC);

	//     if (ret != 0)
	//         printk(KERN_ERR "error in call to usermodehelper: %i\n", ret);
	//     else
	//         printk(KERN_INFO "usermodehelper executed successfully\n");

	// sprintf(msg, "I already told you %d times Hello world! you are %d \n", counter++,pid_n);
	cpu_mem_usage(pid_n);
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

static void cpu_mem_usage_end(int pid)
{
	
    struct task_struct *task;
    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    unsigned long jiffies_end, utime_end, stime_end;

    if (task == NULL) {
        printk(KERN_ALERT "无法找到PID为%d的进程\\n", pid);
        return;
    }
  
    utime_end = task->utime/8000000;
    stime_end = task->stime/8000000;
    struct process_monitor *node = find(&head, pid);
	unsigned long end_time, utime_e, stime_e, cpu_usage_end;
    	jiffies_end = get_jiffies_64();     // 结束的时间
	end_time=bing_read_now();
	printk(KERN_INFO "%d****end_time****:%lu\n" ,pid, end_time);
	// bing_read_proc(pid, end_time, utime_e, stime_e);
	struct file *f;
	char buf[128];
	mm_segment_t fs;
	int i;
	static  long now_time, virtual_mem_usage, physical_mem_usage;
	/* Open /proc/pid/stat */
	sprintf(buf, "/proc/%d/stat", pid);
	f = filp_open(buf, O_RDONLY, 0);
	if (f == NULL) {
	    printk(KERN_INFO "filp_open error!!.\n");
	    return -1;
	}
	// printk(KERN_INFO "filp_open success!!.\n");
	/* Read /proc/pid/stat */
	fs = get_fs();
	// set_fs(get_ds());
	set_fs(KERNEL_DS);
	i = f->f_op->read(f, buf, 128, &f->f_pos);
	set_fs(fs);

	/* Close /proc/pid/stat */
	filp_close(f, NULL);

	if (i <= 0) {
	    printk(KERN_INFO "Read error.\n");
	    return -1;
	}
	// printk(KERN_INFO "Read SUCCESS.\n");
	buf[i] = '\0';
	printk(KERN_ALERT "success buffer%s.\n",buf);
	char* token1,*cur1 = buf;
	 char* const delim1=" ";
	   int count1=1;
	    // const char *tmp = to;
	while (token1 = strsep(&cur1,delim1)){
	  if(count1==14){
	 
	    kstrtol(token1,10,&utime_e);
	    // printk(KERN_INFO "%d****utime_e****:%lu\n" ,pid,utime_e);
	  }
	  if(count1==15){
	   
	    kstrtol(token1,10,&stime_e);
	    // printk(KERN_INFO "%d****stime_e****:%lu\n",pid ,stime_e);
	  }

	    // printk(KERN_INFO "****count%d****:%s\n" ,count1,token1);
	    count1++;
	  } 
	cpu_usage_end = (utime_e+stime_e-node->utime_s-node->stime_s)*1000/(end_time - node->start_time);


    unsigned long cpu_usage = ((utime_end + stime_end) - (node->utime_start + node->stime_start)) * 1000 / ((jiffies_end/1 - node->jiffies_start/1)/2);
    unsigned long mem_usage = task->mm->total_vm * 4;
	printk(KERN_INFO "进程的退出时间，退出用户态时间和内核态时间分别是：%lu, %lu, %lu\n", jiffies_end,  utime_end, stime_end);
	printk(KERN_INFO "进程的退出时间，退出用户态时间和内核态时间分别是：%lu, %lu, %lu\n", end_time,  utime_e, stime_e);
	printk(KERN_INFO "%d****cpu_usage = %lu/%lu=%lu%%%%****", pid, (utime_e+stime_e-node->utime_s-node->stime_s)*1000, (end_time - node->start_time), cpu_usage_end);
    printk(KERN_INFO "进程%d的CPU占用率为%lu/%lu=%lu%%%%\\\\n", pid, ((utime_end + stime_end) - (node->utime_start + node->stime_start))*1000, ((jiffies_end/1 - node->jiffies_start/1)/2), cpu_usage);

    printk(KERN_INFO "进程%d的虚拟内存大小为%lu KB\\n", pid, mem_usage);
    delete_process_monitor(&head, pid);
    return;
}



/* Called when a process closes the device file. */
static int device_release(struct inode *inode, struct file *file)
{
	/* We're now ready for our next caller */
	atomic_set(&already_open, CDEV_NOT_USED);
	/* Decrement the usage count, or else once you opened the file, you will
	* never get rid of the module.
	*/
	int pid = pid_nr(task_pid(current));
	cpu_mem_usage_end(pid);
	module_put(THIS_MODULE);
	return SUCCESS;
}

/* Called when a process, which already opened the dev file, attempts to
* read from it.
*/
static ssize_t device_read(struct file *filp, /* see include/linux/fs.h */
char __user *buffer, /* buffer to fill with data */
size_t length, /* length of the buffer */
loff_t *offset)
{
	/* Number of bytes actually written to the buffer */
	int bytes_read = 0;
	const char *msg_ptr = msg;
	if (!*(msg_ptr + *offset)) { /* we are at the end of message */
	*offset = 0; /* reset the offset */
	return 0; /* signify end of file */
	}
	msg_ptr += *offset;
	/* Actually put the data into the buffer */
	while (length && *msg_ptr) {
		/* The buffer is in the user data segment, not the kernel
		* segment so "*" assignment won't work. We have to use
		* put_user which copies data from the kernel data segment to
		* the user data segment.
		*/
		put_user(*(msg_ptr++), buffer++);
		length--;
		bytes_read++;
	}
	*offset += bytes_read;
	/* Most read functions return the number of bytes put into the buffer. */
	return bytes_read;
}
/* Called when a process writes to dev file: echo "hi" > /dev/hello */
static ssize_t device_write(struct file *filp, const char __user *buff,
size_t len, loff_t *off)
{
	pr_alert("Sorry, this operation is not supported.\n");
	return -EINVAL;
}
module_init(chardev_init);
module_exit(chardev_exit);
MODULE_LICENSE("GPL");
