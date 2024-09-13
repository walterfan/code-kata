from collections import deque


def binary_search(arr, target):
    low, high = 0, len(arr) - 1

    while low <= high:
        mid = (low + high) // 2
        if arr[mid] == target:
            return mid
        elif arr[mid] < target:
            low = mid + 1
        else:
            high = mid - 1

    return -1

def shell_sort(arr):
    n = len(arr)
    gap = n // 2

    while gap > 0:
        for i in range(gap, n):
            temp = arr[i]
            j = i
            while j >= gap and arr[j - gap] > temp:
                arr[j] = arr[j - gap]
                j -= gap
            arr[j] = temp
        gap //= 2

    return arr

def bfs(graph, start_node):
    visited = set()  # 用于跟踪已访问的节点
    queue = deque([start_node])  # 使用队列来存储需要访问的节点顺序
    result = []  # 用于保存遍历顺序

    while queue:
        node = queue.popleft()  # 访问队列中的第一个节点
        if node not in visited:
            visited.add(node)  # 标记该节点为已访问
            result.append(node)  # 将该节点添加到结果中
            
            # 将该节点的未访问邻居添加到队列中
            for neighbor in graph[node]:
                if neighbor not in visited:
                    queue.append(neighbor)
    
    return result
