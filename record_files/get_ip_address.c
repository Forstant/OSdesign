void get_ip_address(int pid)
{
    struct task_struct *task = pid_task(find_vpid(pid), PIDTYPE_PID);
	// 获取文件描述符表
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
}