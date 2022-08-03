// #include<iostream>

// #define   \033[0m 关闭所有属性
// #define   \033[1m 高亮
// #define   \033[2m 亮度减半
// #define   \033[3m 斜体
// #define   \033[4m 下划线
// #define   \033[5m 闪烁
// #define   \033[6m 快闪
// #define   \033[7m 反显
// #define   \033[8m 消隐
// #define   \033[9m 中间一道横线
// #define   10-19 关于字体的
// #define   21-29 基本与1-9正好相反
// #define   30-37 设置前景色
// #define   40-47 设置背景色
#define    NONE        "\e[0m"
#define    L_GREY      "\e[1;30m"
#define    L_RED       "\e[1;31m"
#define    L_GREEN     "\e[1;32m"
#define    L_YELLOW    "\e[1;33m"
#define    L_BLUE      "\e[1;34m"
#define    L_PURPLE    "\e[1;35m"
#define    L_D_GREEN   "\e[1;36m"
#define    L_WHITE     "\e[1;37m"
// #define   38 打开下划线,设置默认前景色
// #define   39 关闭下划线,设置默认前景色
// #define   40 黑色背景
// #define   41 红色背景
// #define   42 绿色背景
// #define   43 棕色背景
// #define   44 蓝色背景
// #define   45 品红背景
// #define   46 孔雀蓝背景
// #define   47 白色背景
// #define   48 不知道什么东西
// #define   49 设置默认背景色
// #define   50-89 没用
// #define   90-109 又是设置前景背景的，比之前的颜色浅
// #define   \033[nA 光标上移n行
// #define   \033[nB 光标下移n行
// #define   \033[nC 光标右移n行
// #define   \033[nD 光标左移n行
// #define   \033[y;xH设置光标位置
// #define   \033[2J 清屏
// #define   \033[K 清除从光标到行尾的内容
// #define   \033[s 保存光标位置
// #define   \033[u 恢复光标位置
// #define   \033[?25l 隐藏光标
// #define   \033[?25h 显示光标
