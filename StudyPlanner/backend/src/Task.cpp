#include "Task.h"

//默认构造在Task.h中给出,这里是能处理异常输入的构造函数
Task::Task(int t_priority, TaskContent t_content, bool is_completed, bool n_review, TaskManager& m, chrono::system_clock::time_point ddl)
    :priority(t_priority), task_content(t_content), deadline(ddl), completed(is_completed), needs_review(n_review) {
    try {
        id = m.getNextId();
    } catch (const std::exception& e) {
        throw std::runtime_error("Invalid input for Task constructor: " + std::string(e.what()));
    }
}

//将单个task转换为json
json Task::toJson(const Task& t) {
    json j;
    j["id"] = t.id;
    j["priority"] = t.priority;
    j["title"] = t.task_content.title;
    j["content"] = t.task_content.content;
    j["time_point"] = chrono::duration_cast<chrono::milliseconds>(t.deadline.time_since_epoch()).count();
    j["completed"] = t.completed;
    j["needs_review"] = t.needs_review;
    return j;
}

//从json中导出单个task的信息
Task Task::fromJson(const json& j) {
    Task t;
    t.id = j.at("id").get<int>();
    t.priority = j.at("priority").get<int>();
    t.task_content.title = j.at("title").get<std::string>();
    t.task_content.content = j.at("content").get<std::string>();
    t.deadline = chrono::system_clock::time_point(chrono::milliseconds(j.at("time_point").get<int64_t>()));
    t.completed = j.at("completed").get<bool>();
    t.needs_review = j.at("needs_review").get<bool>();
    return t;
}

void Task::markCompleted()
{
    completed = true;
}
