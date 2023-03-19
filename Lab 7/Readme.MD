Part 3: 

Linear Page table with number of bits: 

paging-linear-size.py -v 16 -c
paging-linear-size.py -v 32 -c
paging-linear-size.py -v 64 -c

Linear Page table with page size: 

paging-linear-size.py -p 16 -c
paging-linear-size.py -p 32 -c
paging-linear-size.py -p 64 -c

Linear Page table size with PTE size: 

paging-linear-size.py -e 4 -c
paging-linear-size.py -e 8 -c
paging-linear-size.py -e 16 -c

Address Translation with Linear Page table:

1:

Address Space grows: 

-P 1k -a 1m -p 512m -v -n 0
-P 1k -a 2m -p 512m -v -n 0
-P 1k -a 4m -p 512m -v -n 0

Page Size grows: 

-P 1k -a 1m -p 512m -v -n 0
-P 2k -a 1m -p 512m -v -n 0
-P 4k -a 1m -p 512m -v -n 0

