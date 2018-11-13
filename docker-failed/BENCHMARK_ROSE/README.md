# ROSE

## Build Errors

Dependencies linked by the install script are no longer available

```
Package gcc-4.9 is not available, but is referred to by another package.
This may mean that the package is missing, has been obsoleted, or
is only available from another source

Package g++-4.9 is not available, but is referred to by another package.
This may mean that the package is missing, has been obsoleted, or
is only available from another source

E: Package 'gcc-4.9' has no installation candidate
E: Package 'g++-4.9' has no installation candidate
E: Unable to locate package gfortran-4.9
E: Couldn't find any package by glob 'gfortran-4.9'
E: Couldn't find any package by regex 'gfortran-4.9'
update-alternatives: error: alternative path /usr/bin/gcc-4.9 doesn't exist
update-alternatives: error: alternative path /usr/bin/g++-4.9 doesn't exist
update-alternatives: error: alternative path /usr/bin/gfortran-4.9 doesn't exist
bash: line 22: cd: /root/Downloads: No such file or directory
--2018-11-13 09:41:28--  http://download.oracle.com/otn-pub/java/jdk/7u51-b13/jdk-7u51-linux-x64.tar.gz
Resolving download.oracle.com (download.oracle.com)... 2.20.92.183
Connecting to download.oracle.com (download.oracle.com)|2.20.92.183|:80... connected.
HTTP request sent, awaiting response... 302 Moved Temporarily
Location: https://edelivery.oracle.com/otn-pub/java/jdk/7u51-b13/jdk-7u51-linux-x64.tar.gz [following]
--2018-11-13 09:41:28--  https://edelivery.oracle.com/otn-pub/java/jdk/7u51-b13/jdk-7u51-linux-x64.tar.gz
Resolving edelivery.oracle.com (edelivery.oracle.com)... 92.123.21.223, 2a02:26f0:11a:482::2d3e, 2a02:26f0:11a:4b2::2d3e
Connecting to edelivery.oracle.com (edelivery.oracle.com)|92.123.21.223|:443... connected.
HTTP request sent, awaiting response... 302 Moved Temporarily
Location: http://download.oracle.com/otn-pub/java/jdk/7u51-b13/jdk-7u51-linux-x64.tar.gz?AuthParam=1542102208_3101db332a542f62eb2442146ad6748b [following]
--2018-11-13 09:41:29--  http://download.oracle.com/otn-pub/java/jdk/7u51-b13/jdk-7u51-linux-x64.tar.gz?AuthParam=1542102208_3101db332a542f62eb2442146ad6748b
Connecting to download.oracle.com (download.oracle.com)|2.20.92.183|:80... connected.
HTTP request sent, awaiting response... 301 Moved Permanently
Location: https://download.oracle.com/otn-pub/java/jdk/7u51-b13/jdk-7u51-linux-x64.tar.gz?AuthParam=1542102208_3101db332a542f62eb2442146ad6748b [following]
--2018-11-13 09:41:29--  https://download.oracle.com/otn-pub/java/jdk/7u51-b13/jdk-7u51-linux-x64.tar.gz?AuthParam=1542102208_3101db332a542f62eb2442146ad6748b
Connecting to download.oracle.com (download.oracle.com)|2.20.92.183|:443... connected.
HTTP request sent, awaiting response... 404 Not Found
2018-11-13 09:41:31 ERROR 404: Not Found.
```
