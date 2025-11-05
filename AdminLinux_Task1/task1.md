1 – Ubuntu – Debian -  Kali

2- shows the manual pages for commands (works only with commands that has manual documentation ) 

3- rm removes files and directories (with -r it removes  recursively).
	rm   ws/* -r       (removes content )
	rm   ws/   -r  	  (removes Directory)
  rmdir removes empty directories only

4- 
  A) got an error 
	rmdir: failed to remove 'dir1/dir11': Directory not empty
	over come by using      rm -r dir1/dir11   as it removes content recursively

  B)  rmdir: failed to remove 'Documents/OldFiles': Not a directory

  C) Absolute path: /home/user/dir1/dir12/mycv
          Relative path : dir1/dir12/mycv

5- cp  /etc/passwd ~/mypasswd

6- mv ~/mypasswd ~/oldpasswd

7-  cd ~
    cd /home/user 
    cd
    cd $HOME

8- :/usr/bin$ ls w*
w           wc      whereis            who                   word-list-compress
wall        wdctl   which              whoami                wpa_passphrase
watch       wget    which.debianutils  whoopsie              write
watchgnupg  whatis  whiptail           whoopsie-preferences  write.ul


9- type is used to find the whether the command is a builtin, alias, function, or external binary 

10-  file  /usr/bin/c*

11 - man -k read 

12 - searches the man page descriptions for a given keyword
    
