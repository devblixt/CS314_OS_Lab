cp link.c /usr/src/minix/servers/vfs/link.c
cp open.c /usr/src/minix/servers/vfs/open.c
cp read.c /usr/src/minix/servers/vfs/read.c
cd /usr/src && make build MKUPDATE=yes >log.txt 2>log.txt