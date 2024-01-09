#include <stdio.h>
#include <stdarg.h>
#include <time.h>

void logger(const char *format, ...) {
    // 現在時刻を取得してフォーマット
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    char formatted_time[50];
    strftime(formatted_time, sizeof(formatted_time), "%Y-%m-%d %H:%M:%S", tm_now);

    // ログのプレフィックスを出力
    printf("[%s] ", formatted_time);

    // 可変長引数の処理
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // 改行を出力
    printf("\n");
}