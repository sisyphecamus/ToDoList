#include "TaskManager.h"
#include <fstream>

/*
    程序启动时从.json file中读取已保存数据
    约定json格式:
    {
        tasks: [
           {"id": intType_number , "priority" : intType_number, "title":stringType, "content":stringType,
            "time_point": ddl, "completed" : boolType, "needs_review": boolType},                                                                            }
           ...
        ],
        "next_taskId" : intType_number
    } 
*/
void TaskManager::loadTask()
{
    ifstream i("TaskMananger.json");
    if(!i.is_open())
    {
        throw ios_base::failure("Failed to open file: TaskManager.json");
    } 
    json j;
    i>>j;
    tasks.clear();

    //读取task至map
    for(const auto& item:j.at("tasks"))
    {
        try {
            Task t = Task::fromJson(item);//读取单个task
        } catch (const std::exception& e) {
            throw std::runtime_error("Error loading task from JSON: " + std::string(e.what()));
        }
    }

    //读取next_taskId
    try {
        next_taskId = j.at("next_taskId").get<int>();
    } catch (const std::exception& e) {
        throw std::runtime_error("Error loading next_taskId from JSON: " + std::string(e.what()));
    }
}

//程序终止时导出到json文件,约定格式同上
void TaskManager::dumpTask() const
{
    json j;
    j["tasks"] = json::array();
    
    //将tasks中的每个task转换为json格式
    for(const auto& [id, task] : tasks)
    {
        j["tasks"].push_back(Task::toJson(task));
    }

    //保存next_taskId
    j["next_taskId"] = next_taskId;

    //写入文件
    ofstream o("TaskManager.json");
    if(!o.is_open())
    {
        throw ios_base::failure("Failed to open file: TaskManager.json");
    }
    o << j.dump(4); // 4 is the indentation level for pretty printing
}

//创建新任务,id由TaskManager分配
void TaskManager::createTask(const TaskContent &t_content, chrono::system_clock::time_point ddl, int p, bool n_review)
{
    Task new_task(p, t_content, false, n_review, *this, ddl);
    tasks[new_task.getId()] = new_task; // 使用任务的id作为键
}

//修改任务
bool TaskManager::updateTask(int id, const TaskContent new_content, const chrono::system_clock::time_point new_ddl, const int new_p,
const bool new_n_review)
{
    auto it = tasks.find(id);
    if (it == tasks.end()) {
        return false; // 任务未找到
    }
    Task& task = it->second;

    // 更新任务内容
    task.updateContent(new_content);
    task.updateDeadline(new_ddl);
    task.updatePriority(new_p);
    task.updateNeedsReview(new_n_review);
    return true;
}
