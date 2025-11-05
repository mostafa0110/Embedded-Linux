1) history > /tmp/commands.list

2) echo 'date' >> ~/.profile

3) count words in a file 
wc -w filename
Count number of files in current directory
ls -1 | wc -l

a- history | wc -l

4) a- output is the contents of filename2 only.
   b- get error rm: missing operand
   c- prints the path /etc/passwd then wc -l counts lines of that output and returns 1

5) sudo find / -type f -iname ".profile"

6) ls -id / /etc /etc/hosts

7) sudo ln -s /etc/passwd /boot/passwd

8) sudo ln /etc/passwd /boot/passwd
will fail in case of /boot is in a separate partition (different device)

9) Bash gives secondary prompt PS2 (default is >) to indicate it expects more input.
  to modify edit it in the bashrc   "export PS2=': '"



  
