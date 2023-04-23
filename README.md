# OSDesign

# File structure
```
├── read_files：读取 ∼/targets 文件。格式如下，pid: 100, 234 | prog: program,
another program | file: a filename; another filename
├── recode_files：记录 pid 或 prog 进程访问了哪些文件和 IP 地址，记录 file 文件被
哪些进程所访问。记录内容至少包括进程 pid 和程序名、日期时间、
访问模式。
├── show_relationship：当进程或文件数量不大于 5 个，展示它们的关系，例如，进程之间
的父子关系，某个文件被哪些进程并发访问。
├── show_process_performance：支持记录最多 20 个进程，给出进程分别为 5,10,20 个情况下的模块
性能，包括 CPU 和内存使用情况。
└── README.md
```
