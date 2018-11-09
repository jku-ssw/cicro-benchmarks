# HowTo Build

Due to license restrictions we cannot simply distribute the enterprise edition of GraalVM.

Please download the version you want to benchmark from [www.graalvm.org/downloads](https://www.graalvm.org/downloads/)
and place the uncompressed archive into this directory.

Now you can build the docker images:

## Example (using GRAALVM Community-Edition)

```
wget https://github.com/oracle/graal/releases/download/vm-1.0.0-rc9/graalvm-ce-1.0.0-rc9-linux-amd64.tar.gz
tar -xvzf graalvm-ce-1.0.0-rc9-linux-amd64.tar.gz

sudo make build GRAALVM_DIR=graalvm-ce-1.0.0-rc9
```
