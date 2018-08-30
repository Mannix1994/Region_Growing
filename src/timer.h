/**
 * Timer是一个计时器类。
 * @code
 * Timer timer;//无名timer
 * Timer timer1("name")//有名timer
 * //耗时操作
 * double time = timer.elapsed();//获取时间，不重启计时器
 * double time1 = timer1.elapsed();//获取时间，重启计时器
 * //耗时操作
 * timer.rlog("在时间之前的文字“);//打印时间之后重启计时器
 * timer1.log();//打印时间，有很多选项，可自行查看。
 * //耗时操作
 * timer.restart();//重启计时器
 * @endcode
 */
#ifndef W_TIMER_H
#define W_TIMER_H

#include <iostream>
#include <string>
#include <chrono>

class Timer {
public:

    explicit Timer(bool enabled = true) {
        _name = "Time elapsed:";
        _enabled = enabled;
        restart();
    }

    explicit Timer(const std::string &name,bool enabled = false) {
        _name = name + ":";
        _enabled = enabled;
        restart();
    }

    /**
    * 启动计时
    */
    inline void restart() {
        _start_time = std::chrono::steady_clock::now();
    }

    /**
    * 结束计时
    * @return 返回ms数
    */
    inline double elapsed(bool restart = false) {
        _end_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = _end_time-_start_time;
        if(restart)
            this->restart();
        return diff.count()*1000;
    }

    /**
     * 打印时间并重启计时器。rlog是restart log的缩写
     * @param tip 提示
     */
    void rlog(const std::string &tip){
        if(!_enabled)
            return;
        log(true,tip,true,false);
    }

    /**
    * 打印时间
    * @param reset 输出之后是否重启计时器，true重启，false不重启
    * @param unit_ms true是ms，false是s
    * @param tip 输出提示
    * @param kill 输出之后是否退出程序，true退出，false不退出
    */
    void log(bool reset = false, const std::string &tip = "",
             bool unit_ms = true, bool kill = false
    ) {
        if(!_enabled)
            return;
        if (unit_ms) {
            if (tip.length() > 0)
                std::cout << tip+":" << elapsed() << "ms" << std::endl;
            else
                std::cout << _name << elapsed() << "ms" << std::endl;
        } else {
            if (tip.length() > 0)
                std::cout << tip+":" << elapsed() / 1000.0 << "s" << std::endl;
            else
                std::cout << _name << elapsed() / 1000.0 << "s" << std::endl;
        }

        if (reset)
            this->restart();

        if (kill)
            exit(5);
    }


private:
    std::chrono::steady_clock::time_point _start_time;
    std::chrono::steady_clock::time_point _end_time;
    std::string _name;
    bool _enabled;
}; // timer

#endif //W_TIMER_H
