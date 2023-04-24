void show_by_pid(int pid)
{
	struct task_struct *pcb;
        struct pid * PID;
        PID=find_get_pid(pid);
        pcb = pid_task(PID, PIDTYPE_PID);
        
         struct files_struct *current_files; 
         struct fdtable *files_table;
         unsigned int *fds;
         int i=0;
         struct path files_path;
         char *cwd;
         char *buf = (char *)kmalloc(GFP_KERNEL,100*sizeof(char));



         printk(KERN_INFO "pid is : %d    ",pcb->pid);
         printk(KERN_INFO "the name of the process is : %s",pcb->comm);
 
         //files
         current_files = pcb->files;
         files_table = files_fdtable(current_files);
         
         while(files_table->fd[i] != NULL) { 
              //files
              files_path = files_table->fd[i]->f_path;
              cwd = d_path(&files_path,buf,100*sizeof(char));
              printk(KERN_ALERT "Open file with fd %d  %s", i,cwd);
              //mode
              mode_t mode=files_table->fd[i]->f_inode->i_mode;
              char mode_str[11]={0};
              for(int j=0;j<11;j++) mode_str[j]=' ';
              switch(mode & S_IFMT)
              {
                  case S_IFREG:
                  mode_str[0] = '-';
                  break;
                  case S_IFDIR:
                  mode_str[0] = 'd';
                  break;
              }
              printk(KERN_INFO "0: %c", mode_str[0]);
              mode_str[1] = (mode & S_IRUSR) ? 'r' : '-';
              mode_str[2] = (mode & S_IWUSR) ? 'w' : '-';
              mode_str[3] = (mode & S_IXUSR) ? 'x' : '-';
              mode_str[4] = (mode & S_IRGRP) ? 'r' : '-';
              mode_str[5] = (mode & S_IWGRP) ? 'w' : '-';
              mode_str[6] = (mode & S_IXGRP) ? 'x' : '-';
              mode_str[7] = (mode & S_IROTH) ? 'r' : '-';
              mode_str[8] = (mode & S_IWOTH) ? 'w' : '-';
              mode_str[9] = (mode & S_IXOTH) ? 'x' : '-';
              mode_str[10] = '\0';
              printk(KERN_INFO "File mode: %s\n", mode_str);

             struct timespec64 mod_time = files_table->fd[i]->f_inode->i_mtime;
            char timestamp[32];
            struct tm tm;
           time64_t time = timespec64_to_ns(&mod_time) / 1000000000;
           time64_to_tm(time, 0, &tm);
            snprintf(timestamp, sizeof(timestamp), "%04d-%02d-%02d %02d:%02d:%02d",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
            printk(KERN_INFO "File last modified at: %s.%09ld\n", timestamp, mod_time.tv_nsec);


              
              i++;}
}
