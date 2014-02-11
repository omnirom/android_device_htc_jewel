/*
Copyright (c) 2013, The Linux Foundation. All rights reserved.
Copyright (c) 2014, haus.xda@gmail.com

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <init/log.h>

#include <android/log.h>

#include "setfstab.h"

static int DEBUG = 1;

static int LEGACY;

//Binary copy files
//ToDo: handle large files
int cp(const char *filein, const char *fileout) {
    FILE * fpIn;
    FILE * fpOut;
    long size;
    int ret;

    if (DEBUG) { logi("Preparing to copy %s to %s", filein, fileout); }

    fpIn = fopen(filein, "rb");
    if (!fpIn) {
        loge("Unable to open %s for copying!", filein);
        return -1;
    }

    fseek(fpIn, 0L, SEEK_END);
    size = ftell(fpIn);
    rewind(fpIn);

    char buffer[size];
    ret = fread(&buffer, 1, size, fpIn);
    fclose(fpIn);
    if (ret != size) {
        loge("Error buffering %s! File size: %ld  Buffered size: %d", filein, size, ret);
        return -2;
    }

    fpOut = fopen(fileout, "wb");
    if (!fpOut) {
        loge("Unable to open %s to write to!", fileout);
        return -3;
    }

    ret = fwrite(buffer, 1, size, fpOut);
    fclose(fpOut);
    if (ret != size) {
        loge("Error while writing to %s! Original Size: %ld  Value Returned: %d", filein, size, ret);
        return -4;
    }

    if (DEBUG) { logi("Copying %s to %s completed successfully", filein, fileout); }

    return 0;
}

// make_link routine from init/utils.c sans the mkdir call
void ln(const char *oldpath, const char *newpath)
{
    int ret;
    char buf[256];
    char *slash;
    int width;

    slash = strrchr(newpath, '/');
    if (!slash)
        return;
    width = slash - newpath;
    if (width <= 0 || width > (int)sizeof(buf) - 1)
        return;
    memcpy(buf, newpath, width);
    buf[width] = 0;
//    ret = mkdir_recursive(buf, 0755);
//    if (ret)
//        ERROR("Failed to create directory %s: %s (%d)\n", buf, strerror(errno), errno);

    ret = symlink(oldpath, newpath);
    if (ret && errno != EEXIST)
        ERROR("Failed to symlink %s to %s: %s (%d)\n", oldpath, newpath, strerror(errno), errno);
}

int main(int argc, const char **argv) {

    int syspart;

    LEGACY = ((syspart = emmc_name_to_number("system")) == 35);

    if (DEBUG) { logi("System partition found at %d", syspart); }

    if (LEGACY) {
        cp("/fstab-old.qcom", "/fstab.qcom");
        cp("/remount-old.qcom", "/remount.qcom");
        cp("/system/etc/storage_list-old.xml", "/storage_list.xml");
        ln("/storage/sdcard0", "/mnt/sdcard");
        ln("/storage/sdcard0", "/sdcard");
    } else {
        cp("/fstab-new.qcom", "/fstab.qcom");
        cp("/remount-new.qcom", "/remount.qcom");
        cp("/system/etc/storage_list-new.xml", "/storage_list.xml");
        ln("/storage/emulated/legacy", "/mnt/sdcard");
        ln("/storage/emulated/legacy", "/sdcard");
    }

    exit(0);
}

