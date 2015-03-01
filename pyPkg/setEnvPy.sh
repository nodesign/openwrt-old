export STAGING_DIR=$PWD/../staging_dir
export PYTHONXCPREFIX=${STAGING_DIR}/target-mips_34kc_uClibc-0.9.33.2/usr
export CROSS_COMPILE=${STAGING_DIR}/toolchain-mips_34kc_gcc-4.6-linaro_uClibc-0.9.33.2/bin/mips-openwrt-linux-uclibc-
export CC="${CROSS_COMPILE}gcc"
export LDSHARED="${CC} -shared"
export LDFLAGS="-L${STAGING_DIR}/target-mips_34kc_uClibc-0.9.33.2/lib"
export CPATH="${STAGING_DIR}/target-mips_34kc_uClibc-0.9.33.2/usr/include"
export CFLAGS="-I${STAGING_DIR}/target-mips_34kc_uClibc-0.9.33.2/usr/include/python2.7"
