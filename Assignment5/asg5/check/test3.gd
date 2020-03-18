# $Id: rectilinear.gd,v 1.4 2019-06-03 14:29:14-07 - - $
# Some rectilinear symbols
define rect rectangle 100 50
define squ square 75
define card diamond 50 100
define tri triangle 0 0 100 50 0 60
define equi equilateral 100 
draw cyan rect 100 100
draw cyan squ 200 100
draw cyan card 300 100
draw magenta tri 100 300
draw magenta card 200 300
draw magenta equi 300 300
