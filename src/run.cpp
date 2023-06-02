#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define OK 0
#define WRONG_ANSWER 1

void run_judge() {
    // 打开管道文件。注意打开顺序，否则会造成死锁！
    int in  = open("u2j.fifo", O_RDONLY);
    int out = open("j2u.fifo", O_WRONLY);

    // 重定向标准输入输出
    dup2(in, 0);
    dup2(out, 1);
    close(in);
    close(out);

    // 执行裁判程序
    // clang-format off
    execl("/root/SEU_MapDiscovery/bin/oj.exe",
            "/root/SEU_MapDiscovery/bin/oj.exe",
          "/root/SEU_MapDiscovery/res/input.txt",
          "/root/SEU_MapDiscovery/res/output.txt",
          "/root/SEU_MapDiscovery/res/log.txt",
          NULL);
    // clang-format on
}

void run_user() {
    // 打开管道文件。注意打开顺序，否则会造成死锁！
    int out = open("u2j.fifo", O_WRONLY);
    int in  = open("j2u.fifo", O_RDONLY);

    // 重定向标准输入输出
    dup2(in, 0);
    dup2(out, 1);
    close(in);
    close(out);

    // 执行用户程序
    execl("/root/SEU_MapDiscovery/bin/1048.exe", "", NULL);
}

void verdict(int stat_j, int stat_u) {
    if (WIFEXITED(stat_u) || (WIFSIGNALED(stat_u) && WTERMSIG(stat_u) == SIGPIPE)) {
        // 用户程序正常退出，或由于 SIGPIPE 退出，需要裁判程序判定
        if (WIFEXITED(stat_j)) {
            // 裁判程序正常退出
            switch (WEXITSTATUS(stat_j)) {
            case OK:
                printf("Accepted\n");
                break;
            case WRONG_ANSWER:
                printf("Wrong answer\n");
                break;
            default:
                printf("Invalid judge exit code\n");
                break;
            }
        }
        else if (WIFSIGNALED(stat_j) && WTERMSIG(stat_j) == SIGPIPE) {
            // 裁判程序由于 SIGPIPE 退出
            printf("Wrong answer\n");
        }
        else {
            // 裁判程序异常退出
            printf("Judge exit abnormally\n");
        }
    }
    else {
        // 用户程序运行时错误
        printf("Runtime error\n");
    }
}

int main() {
    // 创建管道文件
    mkfifo("j2u.fifo", 0644);
    mkfifo("u2j.fifo", 0644);

    pid_t pid_j, pid_u;

    // 创建裁判进程
    pid_j = fork();
    if (pid_j < 0) {
        printf("Fail to create judge process.\n");
        return 1;
    }
    else if (pid_j == 0) {
        run_judge();
        return 0;
    }

    // 创建用户进程
    pid_u = fork();
    if (pid_u < 0) {
        printf("Fail to create user process.\n");
        return 1;
    }
    else if (pid_u == 0) {
        run_user();
        return 0;
    }

    // 等待进程运行结束，并判定结果
    int stat_j, stat_u;
    waitpid(pid_j, &stat_j, 0);
    waitpid(pid_u, &stat_u, 0);
    verdict(stat_j, stat_u);

    return 0;
}