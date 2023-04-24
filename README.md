# OSDesign

# File structure
```
├── read_files：读取 ∼/targets 文件。
├── record_files：记录 pid 或 prog 进程访问了哪些文件和 IP 地址，记录 file 文件被哪些进程所访问。
|   |   ├── get_ip_address(int pid)：读取进程访问的ip地址
|   |   ├── show_by_pid(int pid)：通过pid打印进程的id、进程名、进程打开文件、打开文件的模式、文件的最后一次修改时间
├── show_relationship：当进程或文件数量不大于 5 个，展示它们的关系.
├── show_process_performance：支持记录最多 20 个进程，给出进程分别为 5,10,20 个情况下的模块性能，包括 CPU 和内存使用情况。
|   ├── modulo_to_device：
|   |   ├── unsigned long bing_read_now(int pid)：读取现在时间
|   |   ├── static int cpu_mem_usage(int pid)：读取进程性能表现
|   |   ├── static int device_open(struct inode *inode, struct file *file)：打开文件
|   |   ├── static ssize_t device_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset)：读文件（限制20次以内）
└── README.md
```
