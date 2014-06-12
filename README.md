bypassmount
===========
Contents
--------

1. Motivation
2. Usage
3. Support


1. Motivation
-------------

This utility was been written to make "du" bypass mountpoint to be able
to compare used space values of "du" and "df".

However you can use "unshare" and "umount" combination instead of this
utility. So the "bypassmount" may be useful for:
* bypassing broken mountpoints (it doesn't do "stat()" and a lot of other
garbage before the "umount2()");
* bypassing mountpoints in one-line shell commands.

2. Usage
--------

You can run shell with bypassing the mountpoint with

    bypassmount /path/to/the/mountpoint

Or you can run some command with bypassing the mountpoint, for example:

    bypassmount -q /mnt/backup -- du --depth 1 -s /mnt/backup/


3. Support
----------

To get support, you can contact with me this ways:
- IRC:SSL+UTF-8 irc.campus.mephi.ru:6695#mephi,xaionaro,xai
- And e-mail: <dyokunev@ut.mephi.ru>, <xaionaro@gmail.com>; PGP pubkey: 0x8E30679C


