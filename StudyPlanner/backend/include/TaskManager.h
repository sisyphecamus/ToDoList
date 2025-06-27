#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "Task.h"
#include "ReminderScheduler.h"

class TaskManager
{
    private:
        map<int,Task>tasks;
        int next_taskId;
        shared_ptr<ReminderScheduler>reminder_scheduler; 
    public:
        void loadTask();//从json读入task
        json dumpTask()const;//程序终止时导出到task
        Task createTask(const TaskContent &t_content, chrono::system_clock::time_point ddl, int p, bool n_review);

        bool updateTask(int id, const TaskContent* new_content, const chrono::system_clock::time_point* new_ddl, const int* new_p,
        const bool* new_n_review);//修改任务

        bool deleteTask(int id);//从tasks中移除，取消对应的提醒调度
        bool completeTask(int id);//查找task，markCompleted()后生成一个Record放入RecordManager，取消提醒调度此任务

        //查询,抽象为一个函数，实现时再细化
        void get_tasks_info()const;

        /*日期变更处理,清空tasks,取消已有提醒,根据传入的review创建新的task条目(复习),启动新一天的调度提醒*/
        void onDateChanged(const TaskContent & new_content, vector<Task>& review_task_from_yesterday);

        //调度提醒:由ReminderSchedule触发
        void startReminderForTask(const Task& task);
        void stopReminderForTask(int taskId);
};

#endif