#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#include "28114-AliParlakci_q.h"

using namespace std;

void open_input_file(string path, ifstream &file);
void open_output_file(string path, ofstream &file);
void get_config(ifstream &config_file, int &queues, int &processes, int &s);
void open_processes(vector<char *> &processes, string path);
void initialize_mlf_queues(vector<Q *> &queues);
void register_processes(Q &q, int number_of_processes);
bool are_all_processes_dead(const vector<char *> &processes);
void move_processes_to_topmost_q(vector<Q *> &q);
void iterate_process(char *&process);
int get_first_nonempty_q(const vector<Q *> &q);
void schedule_a_process(vector<Q *> &q, vector<char *> &processes);

ofstream output;

int main()
{

    string folder_name;
    cout << "Please enter the process folder name: ";
    cin >> folder_name;

    cout << "When all the processes are completed, you can find execution sequence in \"" << folder_name << "/output.txt\"" << endl;

    open_output_file(folder_name + "/output.txt", output);

    ifstream config_file;
    open_input_file(folder_name + "/configuration.txt", config_file);

    int queue_number, process_number, size_s;
    get_config(config_file, queue_number, process_number, size_s);

    vector<char *> processes(process_number, nullptr);
    open_processes(processes, folder_name);

    vector<Q *> mlf_queues(queue_number, nullptr);
    initialize_mlf_queues(mlf_queues);

    register_processes(*mlf_queues[queue_number-1], process_number);

    int time_slice_count = 0;
    while (!are_all_processes_dead(processes))
    {
        schedule_a_process(mlf_queues, processes);

        if ((time_slice_count % size_s) == size_s - 1)
        {
            move_processes_to_topmost_q(mlf_queues);
        }

        time_slice_count++;
    }

    return 0;
}

void open_input_file(string path, ifstream &file)
{
    file.open(path.c_str());
}

void open_output_file(string path, ofstream &file)
{
    file.open(path.c_str());
}

void get_config(ifstream &config_file, int &queues, int &processes, int &s)
{
    string line = "";
    {
        getline(config_file, line);
        istringstream ss(line);
        ss >> queues;
    }
    {
        getline(config_file, line);
        istringstream ss(line);
        ss >> processes;
    }
    {
        getline(config_file, line);
        istringstream ss(line);
        ss >> s;
    }
}

void open_processes(vector<char *> &processes, string path)
{
    for (int i = 0; i < processes.size(); i++)
    {
        vector<char> commands;

        ostringstream file_path;
        file_path << path << "/p" << i + 1 << ".txt";
        
        ifstream file;
        open_input_file(file_path.str(), file);
        
        string line = "";
        while(getline(file, line))
        {
            istringstream lineStream(line);

            char command;
            lineStream >> command;
            commands.push_back(command); 
        }

        processes[i] = new char[commands.size()];

        for (int j = 0; j < commands.size(); j++)
        {
            processes[i][j] = commands[j];
        }
    }
}

void initialize_mlf_queues(vector<Q *> &queues)
{
    for (int i = 0; i < queues.size(); i++)
    {
        queues[i] = new Q;
    }
}

void register_processes(Q &q, int number_of_processes)
{
    for (int i = 0; i < number_of_processes; i++)
        q.enqueue(i);
}

bool are_all_processes_dead(const vector<char *> &processes)
{
    for (auto process : processes)
        if (*process != '-')
            return false;

    return true;
}

void schedule_a_process(vector<Q *> &q, vector<char *> &processes)
{
    int q_to_be_used_index = get_first_nonempty_q(q);

    int process;
    q[q_to_be_used_index]->dequeue(process);

    char event_type = *processes[process];
    iterate_process(processes[process]);

    if (*processes[process] == '-')
        event_type = 'E';

    output << event_type << ", "
           << "PC" << process + 1 << ", ";

    if (event_type == '0')
    {
        q[q_to_be_used_index]->enqueue(process);
        output << "Q" << q_to_be_used_index + 1;
    }
    else if (event_type == '1')
    {
        if (q_to_be_used_index > 0)
        {
            q[q_to_be_used_index - 1]->enqueue(process);
            output << "Q" << q_to_be_used_index;
        }
        else
        {
            q[q_to_be_used_index]->enqueue(process);
            output << "Q" << q_to_be_used_index + 1;
        }
    }
    else if (event_type == 'E')
    {
        output << "QX";
        ; // Don't enqueue again
    }
    else
    {
        cout << "Process file has an empty line. This output must not be printed!" << endl;
    }

    output << endl;
}

void iterate_process(char *&process)
{
    if (*process != '-')
    {
        process++;
    }
}

int get_first_nonempty_q(const vector<Q *> &q)
{
    for (int i = q.size() - 1; i > 0; i--)
    {
        if (!q[i]->isEmpty())
            return i;
    }

    return -1;
}

void move_processes_to_topmost_q(vector<Q *> &q)
{
    for (int i = q.size() - 2; i > 0; i--)
    {
        int process;
        while (!q[i]->isEmpty())
        {
            q[i]->dequeue(process);
            q[q.size() - 1]->enqueue(process);

            output << "B, "
                   << "PC" << process + 1 << ", "
                   << "Q" << q.size() << endl;
        }
    }
}