#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <locale.h>
#include <wchar.h>

#define MAX_STATIONS 500
#define MAX_NAME_LEN 50

typedef struct
{
    wchar_t name[MAX_NAME_LEN];
    int line_number;
    int prev_line_number; // 新增字段，用于存储前一个站点的线路号
} Station;

Station stations[MAX_STATIONS];
int graph[MAX_STATIONS][MAX_STATIONS];
int dist[MAX_STATIONS];
int visited[MAX_STATIONS];
Station prev[MAX_STATIONS];
int station_counts;

int get_station_index(wchar_t *name, int line_number);
void add_edge(int from, int to, int line_number);
int get_next_station();
void dijkstra(int start);
void print_path(int start, int end);

int get_station_index(wchar_t *name, int line_number)
{
    for (int i = 0; i < station_counts; i++)
    {
        if (wcscmp(stations[i].name, name) == 0)
        {
            return i;
        }
    }
    wcscpy(stations[station_counts].name, name);
    stations[station_counts].line_number = line_number;
    return station_counts++;
}

void add_edge(int from, int to, int line_number)
{
    graph[from][to] = line_number;
    graph[to][from] = line_number;
}

int get_next_station()
{
    int min_dist = INT_MAX;
    int next_station = -1;
    for (int i = 0; i < station_counts; i++)
    {
        if (!visited[i] && dist[i] < min_dist)
        {
            min_dist = dist[i];
            next_station = i;
        }
    }
    return next_station;
}

void dijkstra(int start)
{
    for (int i = 0; i < station_counts; i++)
    {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;
    for (int i = 0; i < station_counts; i++)
    {
        int next_station = get_next_station();
        visited[next_station] = 1;
        for (int j = 0; j < station_counts; j++)
        {
            if (graph[next_station][j] && dist[next_station] + 1 < dist[j])
            {
                dist[j] = dist[next_station] + 1;
                prev[j] = stations[next_station];
                prev[j].prev_line_number = graph[next_station][j]; // 更新前一个站点的线路号
                if (next_station == start)                         // 如果从起始站出发找到了一条路径
                {
                    prev[start].prev_line_number = graph[next_station][j]; // 更新起始站的线路号
                }
            }
        }
    }
}

void print_path(int start, int end)
{
    if (dist[end] == INT_MAX)
    {
        wprintf(L"无法到达\n");
        return;
    }
    int path[MAX_STATIONS];
    int path_count = 0;
    path[path_count++] = end;
    int current = end;
    while (current != start)
    {
        path[path_count++] = get_station_index(prev[current].name, prev[current].line_number);
        current = path[path_count - 1];
    }
    // print start station
    wprintf(L"%ls", stations[start].name);
    int count = 0;
    for (int i = path_count - 2; i >= 0; i--)
    {
        int current_line_number = prev[path[i]].prev_line_number;
        if (current_line_number != prev[path[i + 1]].prev_line_number)
        {
            // print previous station
            wprintf(L"-%d(%d)-", prev[path[i + 1]].prev_line_number, count);
            wprintf(L"%ls", stations[path[i + 1]].name);
            count = 1;
        }
        else
        {
            count++;
        }
    }
    wprintf(L"-%d(%d)-", prev[path[0]].prev_line_number, count);
    wprintf(L"%ls", stations[path[0]].name);
    wprintf(L"\n");
}

int main()
{
    setlocale(LC_ALL, "zh_CN.UTF-8");

    FILE *file = fopen("bgstations.txt", "r");
    if (file == NULL)
    {
        wprintf(L"Cannot open file\n");
        return 1;
    }

    int line_count;
    fwscanf(file, L"%d", &line_count);

    for (int i = 0; i < line_count; i++)
    {
        int line_number, station_count;
        fwscanf(file, L"%d %d", &line_number, &station_count);

        int prev_station = -1;
        for (int j = 0; j < station_count; j++)
        {
            wchar_t station_name[MAX_NAME_LEN];
            int transfer_status;
            fwscanf(file, L"%ls %d", station_name, &transfer_status);

            int station_index = get_station_index(station_name, line_number);
            if (prev_station != -1)
            {
                add_edge(prev_station, station_index, line_number);
            }
            prev_station = station_index;
        }
    }

    fclose(file);

    wchar_t start_name[MAX_NAME_LEN], end_name[MAX_NAME_LEN];
    wscanf(L"%ls", start_name);
    wscanf(L"%ls", end_name);
    int start = get_station_index(start_name, 0);
    int end = get_station_index(end_name, 0);
    dijkstra(start);
    print_path(start, end);
    return 0;
}