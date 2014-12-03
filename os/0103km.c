#include <linux/init.h>
#include <linux/module.h>

// for_each_process
#include <linux/sched.h>
// msleep
#include <linux/delay.h>
// rcu
#include <linux/rcupdate.h>

static int yaps_init(void)
{
    // declare task_struct
    struct task_struct *p;

    static int cnt = 0;
    static int zb_n = 0;
    static int dd_n = 0;
    static int it_n = 0;
    static int ui_n = 0;
    static int pk_n = 0;
    static int ru_n = 0;
    static int st_n = 0;

    rcu_read_lock();
    for_each_process (p) {

        printk(KERN_INFO "pid: %5d, parent: %5d, "
               "name: %13s, parent_name: %13s"
               "\n", p->pid, p->parent->pid, p->comm, p->parent->comm);
        cnt ++;
        switch (p->exit_state) {
            case EXIT_ZOMBIE: zb_n++;
                break;
            case EXIT_DEAD: dd_n++;
                break;
            default: break;
        }
        switch (p->state) {
            case TASK_RUNNING: ru_n++;
                break;
            case TASK_INTERRUPTIBLE: it_n++;
                break;
            case TASK_UNINTERRUPTIBLE: ui_n++;
                break;
            case TASK_PARKED: pk_n++;
                break;
            case TASK_STOPPED: st_n++;
                break;
            default: break;
        }

    }
    rcu_read_unlock();
    printk(KERN_INFO "\nTotal: %d\n"
            "Running: %d\n"
            "Zombie : %d\n"
            "Stopped: %d\n"
            "Intable: %d\n"
            "Uintble: %d\n"
            "Deadd  : %d\n"
            "Parked : %d\n"
            , cnt, ru_n, zb_n, st_n, it_n, ui_n, dd_n, pk_n);

    return 0;
}

static void yaps_exit(void)
{
    printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(yaps_init);
module_exit(yaps_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tao J");
MODULE_DESCRIPTION("A kernel module, print process info when load. And say hello to this cruel world");
MODULE_VERSION("v0.1");
