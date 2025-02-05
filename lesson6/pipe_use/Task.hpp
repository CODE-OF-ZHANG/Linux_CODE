/*
 * @Author: CODE_OF_ZHANG 3363967646@qq.com
 * @Date: 2025-02-05 14:32:26
 * @LastEditors: CODE_OF_ZHANG 3363967646@qq.com
 * @LastEditTime: 2025-02-05 20:58:05
 * @FilePath: /lesson6/pipe_use/Task.hpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#pragma once
#include <iostream>
#include <functional>
#include <vector>

typedef void (*task_t)();

void task1()
{
    std::cout << "lol 刷新日志" << std::endl;
}

void task2()
{
    std::cout << "lol 更新野区, 刷新野怪" << std::endl;
}

void task3()
{
    std::cout << "lol 检测软件更新, 提示用户" << std::endl;
}

void task4()
{
    std::cout << "lol 用户释放技能, 更新血蓝量" << std::endl;
}

void LoadTask(std::vector<task_t> *tasks)
{
    tasks->push_back(task1);
    tasks->push_back(task2);
    tasks->push_back(task3);
    tasks->push_back(task4);
}
