/*Task 任务实体*/
#ifndef TASK_H
#define TASK_H

#include<string>
#include<chrono>
#include "json.hpp"
#include "TaskManager.h"

using namespace std;
using json = nlohmann::json;

struct TaskContent{
    std::string title;
    std::string content;
};

class Task
{
    private:
        int id;//以id为唯一标识
        int priority;
        TaskContent task_content;
        chrono::system_clock::time_point deadline;
/*
    完成时刻和创建时刻，可选，主要用于打卡展示
        system_clock::time_point completed_time; 
        system_clock::time_point created_time; 
*/
        bool completed;
        bool needs_review;
    public:
        //第一个构造函数用于从json中恢复数据,id不由TaskManager分配
        Task(int t_id = 0, int p = 1, TaskContent t_content = {"no title", "no content"}, chrono::system_clock::time_point ddl = chrono::system_clock::now(), bool is_completed = false, bool n_review = false)
        : id(t_id), priority(p), task_content(t_content), deadline(ddl), completed(is_completed), needs_review(n_review) {}

        //能处理异常输入的构造函数,实现在 Task.cpp
        //第二个构造函数创建新任务,id由TaskManager分配
        Task(int t_priority, TaskContent t_content, bool is_completed, bool n_review, TaskManager& m, chrono::system_clock::time_point ddl);

        //将单个task转换为json对象,方便写入
        static json toJson(const Task& t);
        //从json读取的构造函数
        static Task fromJson(const json&);
        
        //获取任务id
        int getId()const { return id; }

        void markCompleted();

        //更新任务内容
        void updateContent(const TaskContent& new_content) { task_content = new_content; }
        void updateDeadline(chrono::system_clock::time_point new_deadline) { deadline = new_deadline; }
        void updatePriority(int new_priority) { priority = new_priority; }
        void updateNeedsReview(bool new_needs_review) { needs_review = new_needs_review; }

        //提醒调度方面
        bool isOverDue()const;
        chrono::minutes minutesUntilDeadline()const;//计算距离截至还有多少分钟,用于提醒调度 
};


#endif